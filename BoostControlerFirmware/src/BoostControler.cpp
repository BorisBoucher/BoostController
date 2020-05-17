#include <FilterDerivative.h>
#include <FilterOnePole.h>
#include <Filters.h>
#include <FilterTwoPole.h>
#include <FloatDefine.h>
#include <RunningStatistics.h>

/*

  GTO/3000GT/Stealth Boost controler
  
  Boost controler read the following signals:
    - RPM (Hz)
    - Speed (Hz)
    - MAP (Analog), 0..5V, 1V/bar
    - Throttle (Analog), 0..5V (0..100%)
    - OBD-1 ECU diagnostic link
    
  Internal value:
    - Current gear is deduced from speed and RPM values. 
    - The target load (or boost) is computed based on:
       * throttle position,
       * current gear to limit torque on transmission in first gear,!
       * RPM value (WOT load target can be set by step of 500 RPM) 
    
  From these inputs, the boost controler compute the boost set point (% or duty cycle).
  Then a PWM signal for the waste gate solenoid is generated on output.

  Configuration and monitoring:
    - The controler communicated with a bluetooth link with a PC
    program that allow to:
      * Configure the boost controler parameter (target boost, correction table, servo parameters...)
      * Receive real time data collected by the boost controler to display and/or record for later analysis.

  Electrical connections:
  =======================
  
	1 - GND
	2 - switched Vbat (12V)
	3 - RPM input (5v pulse)
	4 - Speed input (~9V square pulse)
	5 - MAP input (5V analog)
	6 - Throttle input (5V analog)
	7 - Solenoid output (low side switching)
	8 - Air flow HZ (for high range air flow measuremen)
  
  General structure:
  =================
  
  Data Acquisition:
  ----------------
  As we need to sample for 3 frequency values, we make use of pin change interrupt to
  trigger an interrupt when either RPM or speed signal.

  For RPM, the signal is composed of tree 5V pulse per revolution:
  The maximum frequency @8000RPM is therefore :
    8000 / 60 * 3 = 400Hz
  This lead to an interrupt frequency of 800Hz (pin change interrupt is triggered on each
  change, either raising or falling edge).
	
  Speed input is a square signal ranging from 0 to 220Hz (@300km/h).
  This lead to an interrupt frequency of 440Hz.
  
  Air flow HZ input is a square signal ranging from 0 to 2800+Hz.
  This lead to an interrupt frequency of 5600Hz.
  
  The combined worst case interrupt load is therefore 6840Hz, which leave 2330 cycle per interrupt.

  MAP is captured on an analog input. A little filtering may be needed to denoise the signal.
  
  Throttle position is captured on an analog input. A little filtering may be needed to 
  denoise the signal.

  Fuel pump is captured on an analog input.
  
  Gear table
  ----------
  In order to determine the current gear, a gear table must be provider that allow to deduce 
  the current gear from the current RPM and SPEED.
  
  The table contains 1 entry for each gear with the expected gear ratio expressed as
	gearRatio = (RPM / 60) / ((SPEED / 3600) * 1000) * Circum, with Circum = 2,02m
  
  From Stealth 316 pages, the gear ratios for the 5 speed gearbox are :
	1st	12.200
	2nd	 6.908
	3rd	 4.383
	4th	 3.271
	5th	 2.620
  
  Speed and RMP shall be expressed in the same time reference before computing the ratio:
	SPEED in km/h shall be converted to m/s, and RPM shall be converted to R/s.
	
  Sample calculation : 130km/h @ 3500RPM
	(3500 / 60) = 58.33 rev/s
	(130 / 3600) * 1000 = 36.1m/s
	
	gearRatio = 58.33 / 36.1 * 2.02 = 3.264 => We are in 4th gear
  
  Boost tables
  ------------
  Boost set point is specified in 1D tables, one table for each gear. This allow to 
  reduce the boost in first gear (to lower stress on transmission) and to adjust the
  boost in 5th gear to reduce engine stress during cruising.
  
  Each boost table give a boost set point for a given RPM.
  Each cellule contains the % of reference boost to reach. Therefore, is is simple to
  change the overal boost setting by changing the reference boost.
  Cells may contain values greater than 100%. This allow easy testing of greater 
  boost on some domain, without changing the overall boost reference.
  
  Eg. :
	RPM/Boost target (%)
	1000			100%
	2000      100%
	3000      100%
	4000      100%
	5000      80%
	6000			60%
	7000			50%
  
  For 5 gears, this gives 7*5 = 30 values, stored on 1 byte, easily stored in the arduino
  nano EEPROM memory. We can even  store a 6 speeds gearbox setting.

  Throttle boost tables
  ---------------------
  Two 1D tables are used to compute the final boost target by taking the throttle position and 
  throttle movement into account.

  This table allow to reduce the boost target when the throttle plate is closed or closing, and to 
  increase the target when the throttle is open or opening.

  Throttle pos (%)/Boost target (%)
  0%      0%
  20%     0%
  40%     20%
  60%     50%
  80%     100%
  100%    100%

  Throttle move (%/s)/Boost target (%)
  -1000%/s    0%
   -500%/S    0%
   -250%/S   20%
      0%/S  100%
   +250%/S  100%
   +500%/S  110%
  +1000%/S  120%

  
  PID control
  -----------
  Boost is controled with a PID closed loop.
  The PID have three parameters that are store in a 1D table:
	P	Proportional term, give the basic behavior of the controler
	I	Integral term, correct long term residual error
	D	Derivative term, amortise the controler
	
  Conversion of PID boost to solenoid duty cycle
  ----------------------------------------------
  Once the PID controler computed the 'boost' value to inject in the system, we need a way
  to obtain a solenoid duty cycle value.
  
  To this end, we can use a small 2D table that associates a boost value with an engine load 
  to find a duty cycle.
  
  Example table:
	Boost\load	25%		50%		75%		100%
	0.25		  0%	  0%	  0%	  0%
	0.5			 40%	 30%	 20%	 10%
	0.75		 80%	 60%	 40%	 20%
	1.0			100%	 80%	 60%	 40%
	1.25		100%	100%	 80%	 50%
	
  For any value in between, interpolation is done to obtain the final duty cycle to apply on the 
  waste gate selenoide.

  Automatic throttle position calibration
  ---------------------------------------
  In order to account for throttle position measurement not being @100%, an automatic calibration procedure
  is provided :
    
	* With RPM @0, if the throttle measurement is greater then 80% and steady during 3s, the throttle value
	will be considered as the maximum throttle position (i.e. 100% scale).
	* The BC confirm the new calibration by producing 3 pulses of 1/4s @50% DC on the BC sol.


  Configuration & logging protocol (serial)
  -----------------------------------------
  Conf&Log protocol allow to read acquisition data or internal states and read or write the parameter tables.
  
  There are 4 commands:
	Read byte, command 0
	write byte, command 1
	Read float, command 2
	Write float, command 3.
	
  Each command start with start of heading (SOH, 0x01), followed by a command ID, 
  followed by parameter address on 16 bits, write value (for write command,
  1 or 4 bytes, MSB first), and a terminating marker byte end of transmission (EOT, 0x04).

	[0x01][CMD][ADDR0][ADDR1][VALUE0][VALUE1][VALUE2][VALUE3][0x04]
  
  Address table
  -------------
0x000 - 0x1ff	2048 single byte memory zone

	Addr	Format		Factor	Desc
	0x000	u8			50		MAP, bar, 0..5,12
	0x001	u8			1		Throttle pos, %, 0..100
	0x002	u8			1		Solenoid duty cycle, %, 0..100
	0x003	u8			1		Gear, 0..6 (0 unknown)
	0x004	u8			1		Engine load, %, 0..256
	0x005	u8			1		CPU load, %, 0..100
	0x006	u8			50		Target boost, bar, 0..5,12
	0x007	u8			50		Target output, bar, 0..5,12
								
	0x008	u8[6][4]	1		Boost x Load to Sol DC, %, 0..100, 24 items
	0x020	u8[6][7]	1		Gear x RPM boost correction table, %, 0..200, 42 items
	0x04A	u8[6]		1		Throttle adjust table, %, 0..200
	0x050	u8[7]		1		Throttle deriv adjust table, %, 0..200
								
	0x057	u8			1		Solenoid duty cycle test command, 0..100 (0 : test off)
	0x058	u8			1		Firmware major version
	0x059	u8			1		Firmware minor version
	
	0x05a	u8			1		Throttle Max calibration to allow 100% scale even if sensor report only 95%. 0..100 
	0x05b	u8			50		Fuel presssure, bar, 0..5,12
	0x05c	u8			1		Fuel pump prime, seconds

0x200 - 0x3ff	2048 16 bits words

	Addr	Format	Factor	Desc
	0x800	u16		1000	Tire circumference, m, 0..65,535m
	0x801	u16		1000	1st gear ratio, 0.001, 0..65,535
	0x802	u16		1000	2nd gear ratio, 0.001, 0..65,535
	0x803	u16		1000	3rd gear ratio, 0.001, 0..65,535
	0x804	u16		1000	4th gear ratio, 0.001, 0..65,535
	0x805	u16		1000	5th gear ratio, 0.001, 0..65,535
	0x806	u16		1000	6th gear ratio, 0.001, 0..65,535
	0x807	u16		1000	Speed conversion factor, hz/km/h, 0.001, 0..65,535
	0x808	u16		1000	Reference boost, bar, 0.001, 0..65,535

	0x809	u16		1000	PID P factor, 0.001, 0..65,535
	0x80a	u16		1000	PID I factor, 0.001, 0..65,535
	0x80b	u16		1000	PID D factor, 0.001, 0..65,535

	0x80c	u16		1		RPM, 1, 0..65535
	0x80d	u16		1		SPEED, km/h, 0..65535
	0x80e	s16		1		Throttle pos deriv, %/s, -32768..32767
	0x80f	u16		1		Air flow, Hz, 0..65535
	
  Pin mapping
  -----------
  
	RPM input		  :	PIN 12, Port B4
	Speed input		  :	PIN 11, Port B3
	MAF Hz			  : PIN 10, Port B2
	Throttle input    :	PIN A7, ADC7
	MAP input	 	  :	PIN A6, ADC6
	
	Solenoid output	:	PIN 13, PB5
    Debug RPM replay:   PIN 10, PB2
	Serial TX		:	PIN 1, PD1
	Serial RX		:	PIN 0, PD0
	
  Frequency mesurement
  --------------------
	Frequency is measured by the pin change interrupt that measure the time between each
	rising edge for RPM and SPEED.
	
	The interrupt use the micros() arduino function to read the current time. This function has
	a resolution of 4 µs. For the worst case scenario of measuring a 400Hz RPM frequency, this lead to
	a minimum period of 250µs, i.e 62 timer ticks.
	
	For each parameter, a 4 values circular buffer is used to provide a cheap low pass filter.
	
 */

#include <filters.h>
#include <EEPROM.h>


FilterOnePole boostHighpassFilter( HIGHPASS, 0.1f ); 
FilterOnePole targetLowpassFilter( LOWPASS, 2.0f );
FilterOnePole throttleLowpassFilter( LOWPASS, 5.0f );
FilterOnePole throttleDerHighpassFilter( HIGHPASS, 2.0f );
// Low pss filter for Air flow data. Should correct the measure aliasing
// of counting MAG HZ in short period of 10ms (which may lead to very quantified result
// at low HZ).
FilterOnePole gAirflowLPF(LOWPASS, 5.0f ); 

#define VERSION_MAJOR	1
#define VERSION_MINOR	0
 
// I/O Pins 
#define RPM_IN	12
#define SPEED_IN 11
#define AIRFLOW_IN 10
#define SOLENOID_OUT 13
#define FUEL_PUMP_OUT 9
#define DEBUG_OUT 8
//#define SOLENOID_OUT 10
//#define MAP_IN A6		
//#define THROTTLE_IN A7	
#define MAP_IN A5    
#define THROTTLE_IN A6  
#define FUEL_PRESS_IN A7  

// Debug PIN
#define SPEED_OUT 16
#define RPM_OUT 15

// Other constants
#define LOOP_PERIOD 10	// loop period in ms

#define CONFIG_VER 2

// Configuration tables
// Config version 0
struct Config0
{
	// Type circumference in meter
	float	tyreCircum = 2.02f;
	// conversion factor from speed sensor Hz to km/h
	float	speedFactor = 0.73f;
	float	gearRatios[6] = 
	{
		12.200f,
		 6.908f,
		 4.383f,
		 3.271f,
		 2.620f,
		 1.0f
	};
	
/*	float	pidParam[3] = 
	{
		0.5f,
		0.0f,
		0.0f
	};
  */
	float pidParam[3] = 
	{
		0.8f,
		2.7f,
		0.2f
	};
	
	// 6 row (1 per gear), 7 RPM column values (1000 to 7000 RPM => boost %)
	uint8_t	boostTable[6][7] = 
	{
		{  70,  70,  70,  70,  70,  70,  70},
		{  70, 100, 100, 100, 100, 100,  80},
		{  70, 100, 100, 100, 100, 100,  80},
		{  70, 100, 100, 100, 100, 100,  80},
		{  70, 100, 100, 100, 100, 100,  80},
		{  70, 100, 100, 100, 100, 100,  80},
	};

	// Boost throttle table, 6 values (0 to 100% throttle => boost %)
	uint8_t throttleBoostTable[6] = 
	{
		0, 20, 40, 60, 80, 100
	};
  
	// Boost throttle' table, 7 values (-1000%/s ti 1000%/s)
	uint8_t throttleDerivBoostTable[7] =
	{
		0, 0, 0, 100, 110, 120, 130
	};
  
	// Boost reference for boost table, relative pressure
	float	boostReference = 0.7f;
	
	// Duty cycle table in function of boost vs engine load
	// 6 rows for 0.0, 0.25,0.5,0.75,1.0,1.25bar (relative pressure).
	// 4 columns for 25, 50, 75, 100% load
	uint8_t	dutyCycleTable[6][4] =
	{
		{ 0, 0, 0, 0 },
		{25, 25, 25, 25},
		{40, 40, 40, 40},
		{70, 70, 70, 70},
		{90, 90, 90, 90},
		{100, 100, 100, 100},
	};
};

// Config ver 1
struct Config1 : public Config0
{
	// Max scaling of throttle sensor to report 100% throttle position.
	// This allow to compensate for small adjustement of throttle (e.g. sensor reporting 95% @WOT)
	uint8_t	throttleScale = 100;
};

struct Config : public Config1
{
	/// Fuel pump prime in second after power up
	uint8_t fuelPumpPrime = 5;
};

Config	gConfig;

/// Indicator set when the conf need to be saved
bool	gConfChanged = false;

// Measurement and computation table
struct Measurement
{
	float	RPM = 0.0f;
	// Speed in km/h
	float	SPEED = 0.0f;
	// MAP in Bar  
	float	MAP = 0.0f;
	// Fuel pressure in BAR
	float 	FUEL_PRESS = 0.0f;
	// Throttle in % (0..100)
	float	THROTTLE = 0.0f;
	// Throttle deriv in %/s (-1000..1000%/s)
	float	THROTTLE_DERIV = 0.0f;
	// Air flow HZ
	float AIR_FLOW = 0.0f;
	// Solenoid duty cycle in % (0..100)
	float	SOL_DC = 0.0f;
	// Computed gear (0..5 or 6)
	uint8_t	GEAR = 0;
	// Target boost in Bar (relative)
	float	TARGET_BOOST = 0.0f;
	// Computed target output boost (output of PID corrector)
	float TARGET_OUTPUT = 0.0f;
	// Engine load (%)
	float	LOAD = 0.0f;
	// Boost controler chip load (%)
	float	CHIP_LOAD = 0.0f;
};
Measurement	gMeasures;

struct Simulation
{
  // Force Waste gate output between 0..100%. 0 means no simulation.
  uint8_t  FORCE_WG = 0;
};
Simulation  gSimulation;

// constant info table
struct Info
{
	uint8_t	versionMajor = VERSION_MAJOR;
	uint8_t	versionMinor = VERSION_MINOR;
};
Info	gInfo;
 
// PIN change interrupt variables
uint8_t	 	gLastInputPinState = 0;
uint32_t	gLastRPMDate = 0;
uint32_t	gRPMBuffer[4] = {0,0,0,0};
uint8_t		gRPMBufferHead = 0; 
uint32_t	gRPMAccum = 0;
uint32_t	gLastSPEEDDate = 0;
uint32_t	gSPEEDBuffer[4] = {0,0,0,0};
uint8_t		gSPEEDBufferHead = 0; 
uint32_t	gSPEEDAccum = 0;
// Count the number of raising edge on MAF input
uint8_t		gAirFlowAccum = 0;
// RPM period in µs, 0 means no signal.
volatile uint32_t	gRPMPeriod = INT32_MAX;
// SPEED period in µs, 0 means no signal.
volatile uint32_t	gSPEEDPeriod = INT32_MAX;

// Main loop variables
uint32_t	gLastEvalCycle = 0;

bool debugState = false;
bool rpmOut = false;
bool speedOut = false;
#define TOGGLE_DEBUG     digitalWrite(DEBUG_OUT, debugState);    debugState = !debugState;


// Pin Change ISR
ISR (PCINT0_vect)
{
	// Read port B status
	uint8_t b = PINB;

	// get current time
	uint32_t now = micros();
	// check for changed event
	uint8_t changed = b ^ gLastInputPinState;
	// save last nown state of port B
	gLastInputPinState = b;
  
//  if (changed & 0x0f)
//  if (changed)
//  {
//    TOGGLE_LED
//  }

	//  check RPM raising edge
	if ((changed & 0x10) && (b & 0x10))
	{
//    	digitalWrite(13,digitalRead(RPM_IN) and digitalRead(SPEED_IN));
//    	TOGGLE_DEBUG
//    	digitalWrite(RPM_OUT, rpmOut);    
    	rpmOut = !rpmOut;

		// RPM changed !
		uint32_t period = now - gLastRPMDate;
		gLastRPMDate = now;
		gRPMAccum -= gRPMBuffer[gRPMBufferHead];
		gRPMBuffer[gRPMBufferHead++] = period;
		gRPMBufferHead &= 0x3; 
		gRPMAccum += period;
		
		gRPMPeriod = gRPMAccum>>2;
	}
	// Check SPEED raising edge
	if ((changed & 0x08) && (b & 0x08))
	{
//    	digitalWrite(SPEED_OUT, speedOut);
    	speedOut = !speedOut;

		// Speed changed !
		uint32_t period = now - gLastSPEEDDate;
		gLastSPEEDDate = now;
		gSPEEDAccum -= gSPEEDBuffer[gSPEEDBufferHead];
		gSPEEDBuffer[gSPEEDBufferHead++] = period;
		gSPEEDBufferHead &= 0x3; 
		gSPEEDAccum += period;
		
		gSPEEDPeriod = gSPEEDAccum>>2;
//    gSPEEDPeriod = period;
	}
	// Check AIR FLOW raising edge
	if ((changed & 0x04) && (b & 0x04))
	{
		++gAirFlowAccum;
	}
}

void pciSetup(byte pin)
{
    *digitalPinToPCMSK(pin) |= bit (digitalPinToPCMSKbit(pin));  // enable pin
    PCIFR  |= bit (digitalPinToPCICRbit(pin)); // clear any outstanding interrupt
    PCICR  |= bit (digitalPinToPCICRbit(pin)); // enable interrupt for the group
}

struct VersionInfo
{
	char mVersionString[10] = "SNX_BC_vx";

  bool operator ==(const VersionInfo& other) const
  {
    bool ok = true;
    for (size_t i=0; i<sizeof(mVersionString) && ok; ++i)
    {
      ok &= mVersionString[i] == other.mVersionString[i];
    }
    return ok;
  }
  bool operator !=(const VersionInfo& other) const
  {
    return !(*this == other);
  }
};

void saveConfig()
{
	VersionInfo vi;
	vi.mVersionString[8] = CONFIG_VER;
	EEPROM.put(0, vi);
	
	EEPROM.put(16, gConfig);
}

void loadConfig()
{
	VersionInfo refVi;
  	refVi.mVersionString[8] = CONFIG_VER;
	VersionInfo vi;
	EEPROM.get(0, vi);

	Serial.write("Version info loaded : ");
	Serial.write(vi.mVersionString);
	Serial.print(int(vi.mVersionString[8]));
	Serial.write("\n");

	Serial.write("Loading conf\n");
	if (vi.mVersionString[8] == 0)
	{
		EEPROM.get(16, static_cast<Config0&>(gConfig));
		// Need to write the conf back
		gConfChanged = true;
	}
	else if (vi.mVersionString[8] == 1)
	{
		EEPROM.get(16, static_cast<Config1&>(gConfig));
		// Need to write the conf back
		gConfChanged = true;
	}
	else if (vi.mVersionString[8] == CONFIG_VER)
 	{
		// Load last version of config, no convertion required
		EEPROM.get(16, gConfig);
	}
	else
	{
		// no data, or need conversion
		Serial.write("Unknwon conf version found, ignoring\n");
		return;
	}
	Serial.write("Conf loaded\n");
}

void setup()
{
  	// Init serial port @57600 baud, 8N1
	Serial.begin( 57600);
//	Serial.begin(115200);

	Serial.write("init\n");
	Serial.write("load config\n");

	// load conf
	loadConfig();

	Serial.write("set pinMode\n");

	// Configure IO pins
	pinMode(RPM_IN, INPUT_PULLUP);
	pinMode(SPEED_IN, INPUT_PULLUP);
	pinMode(AIRFLOW_IN, INPUT_PULLUP);
	pinMode(SOLENOID_OUT, OUTPUT);
	pinMode(FUEL_PUMP_OUT, OUTPUT);
	pinMode(DEBUG_OUT, OUTPUT);

	// Debug pin
	pinMode(SPEED_OUT, OUTPUT);
	pinMode(RPM_OUT, OUTPUT);

	// Configure analog input
	//TODO

	Serial.write("pci setup\n");

	// activate pin change interrupt
	pciSetup(RPM_IN);
	pciSetup(SPEED_IN);
	pciSetup(AIRFLOW_IN);
}

// PID state variables
float gErrorInteg = 0.0f;
float gErrorDeriv = 0.0f;
float gLastMAP = 0.0f;
float gLastError = 0.0f;

void computeGear()
{
	// compute gear 
	// Eval is done by computing the current speed to RPM ratio and founding the nearest 
	// corresponding gear ratio
	float engineHz = gMeasures.RPM / 60.0f;
	float speedMs = (gMeasures.SPEED / 3600.0f) * 1000.0f;
	float revPerMeter = engineHz / speedMs;
	float gearRatio = revPerMeter
						* gConfig.tyreCircum;
	
	int gear = 1;
	float bestDelta = fabs(gConfig.gearRatios[0] - gearRatio);
	for (int i=1; i<6; ++i)
	{
		float gearDelta = fabs(gConfig.gearRatios[i] - gearRatio);
		if (gearDelta < bestDelta)
		{
			gear = i+1;
			bestDelta = gearDelta;
		}
	}
	
	gMeasures.GEAR = gear;
}

float interp(byte* table, float indexf)
{
  int index1 = (int) floor(indexf);
  int index2 = (int) ceil(indexf);
  float alpha = indexf - index1;
  float delta = float(table[index2]) - float(table[index1]);
  return table[index1] + delta * alpha;
}

float interpf(float* table, float indexf)
{
  int index1 = (int) floor(indexf);
  int index2 = (int) ceil(indexf);
  float alpha = indexf - index1;
  float delta = table[index2] - table[index1];
  return table[index1] + delta * alpha;
}

void manageFuelPump()
{
	static bool primePump = true;
	if (primePump and millis() >= gConfig.fuelPumpPrime * 1000)
	{
		primePump = false;
	}

	digitalWrite(FUEL_PUMP_OUT, primePump);
}

float pistonPos = 0.0f;
float spool = 0.0f;

void evalCycle()
{
	static uint32_t previousLoopDate = 0;

	// read input and process inputs
	//------------------------------
	uint8_t oldSREG = SREG;
	cli();
	uint32_t RPMPeriod = gRPMPeriod;
	uint32_t SPEEDPeriod = gSPEEDPeriod;
	uint32_t lastRPMDate = gLastRPMDate;
	uint32_t lastSPEEDDate = gLastSPEEDDate;
	int8_t lastAirflowAccum = gAirFlowAccum;
	// reset air flow accum for next loop
	gAirFlowAccum = 0;
	SREG = oldSREG;

	uint32_t now = micros();
  
	// Analog read : very costly when done without interrupt : 100µs per read !
	// Read MAP. 1V per bar, 0V @ 0bar, 1V@1 bar (atmospheric pressure)...
	gMeasures.MAP = analogRead(MAP_IN) * (5.0f / 1023.f);

	// Read Fuel pressure, relative. 2.58Bar per V, 0.5V..4.5V => 0..150PSI (10.38Bar), 0.5V @ 0bar
	gMeasures.FUEL_PRESS = (analogRead(FUEL_PRESS_IN) * (5.0f / 1023.f) - 0.5f) * 2.58f;

	// Throttle analog read. 0..100% => 0..5V
	gMeasures.THROTTLE = analogRead(THROTTLE_IN) * (100.0f / 1023.f);
	gMeasures.THROTTLE = throttleLowpassFilter.input(gMeasures.THROTTLE);
	// Scale throttle value with max scale correction
	if (gMeasures.THROTTLE > gConfig.throttleScale)
	{
		// We found a greater value, update max scale
		gConfig.throttleScale = uint8_t(round(gMeasures.THROTTLE));
		// Make sure to save it
		gConfChanged = true;
	}
	gMeasures.THROTTLE = gMeasures.THROTTLE / float(gConfig.throttleScale) * 100.0f;
	// Get high pass filter for throttle derivative
	gMeasures.THROTTLE_DERIV = throttleDerHighpassFilter.input(gMeasures.THROTTLE) * 1000.0f;

	// Detect 0 RPM : last pulse > 100RPM period : 100RPM=>1.6666Hz, *3=>5Hz, 1/5Hz=0.2s=>200.000µs
	if (RPMPeriod > 0 and (now - lastRPMDate) > 200000)
	{
		// 0 stand for 0 RPM
		RPMPeriod = 0;
	}
	// Detect 0 Speed : last pulse > 1km/h period : 1Kmh/0.73 = 1.36s=1.360.000µs
	if (SPEEDPeriod > 0 and (now - lastSPEEDDate) > 1360000)
	{
		// 0 stand for 0 speed
		SPEEDPeriod = 0;
	}

	// Convert µs period into Hz
	// RPM need an additional ratio of 3 to account for the number of cylinder fired each revolution.
	if (RPMPeriod > 0)
	{
		gMeasures.RPM = 60.0f * 1000000.0f / (RPMPeriod * 3.0f);
	}
	else
	{
		gMeasures.RPM = 0.0f;
	}
	if (SPEEDPeriod > 0)
	{
		gMeasures.SPEED = 1000000.0f / (SPEEDPeriod * gConfig.speedFactor);
	}
	else
	{
		gMeasures.SPEED = 0.0;
	}

	// Compute airflow HZ
	uint32_t dtus = now - previousLoopDate;
	float dts = dtus / 1000000.0f;
	if (dts > 0.0f)
	{
		float airFlowHz = lastAirflowAccum / dts; 
		// Filter HZ value
		gMeasures.AIR_FLOW = gAirflowLPF.input(airFlowHz);
	}
	else
	{
		gMeasures.AIR_FLOW = 0.0;
	}

  	computeGear();
	
	// Compute engine load
	// We don't have MAF or VE info, so the load is a simple aproximation based on MAP :
	// 	0 bar MAP => 0% load,
	//	2 bar MAP => 100% load (2 bar MAP means 1 bar turbo pressure)
	gMeasures.LOAD = constrain((gMeasures.MAP * 0.5f) * 100.0f, 0.0f, 100.0f);

	// compute solenoid DC 
	//--------------------
	// 1st, choose the target boost in the gear table
	float targetBoost = 0.0f;
	{
		float rpmIndex = (gMeasures.RPM / 1000.0f) - 1;
		rpmIndex = constrain(rpmIndex, 0, 6);
		
		targetBoost = 
				interp(gConfig.boostTable[gMeasures.GEAR-1], rpmIndex) * 0.01f
				* (gConfig.boostReference);
	}

	// Apply throttle correction
	{
	// throttle position correction
		float throttleIndex = gMeasures.THROTTLE * (sizeof(gConfig.throttleBoostTable)-1) * 0.01f;
		throttleIndex = constrain(throttleIndex, 0, sizeof(gConfig.throttleBoostTable)-1);

		targetBoost = targetBoost * interp(gConfig.throttleBoostTable, throttleIndex) * 0.01f;

		// Throttle pos variation correction
		
		float throttleDerivIndex = 3 + gMeasures.THROTTLE_DERIV * (sizeof(gConfig.throttleDerivBoostTable)-1) * 0.001f;
		throttleDerivIndex = constrain(throttleDerivIndex, 0, sizeof(gConfig.throttleDerivBoostTable)-1);

		targetBoost = targetBoost * interp(gConfig.throttleDerivBoostTable, throttleDerivIndex) * 0.01f;

		// Convert boost value into MAP value, just add the atmo pressure ;)
		targetBoost += 1.0f;
	}

	// And finally get target boost filtered
	gMeasures.TARGET_BOOST = targetLowpassFilter.input(targetBoost);
	
	// 2nd, apply PID filter to compute boost error
	float error = gMeasures.TARGET_BOOST - (max(0.0f, gMeasures.MAP));
	float P = error * gConfig.pidParam[0];
	// Integral windup check
	if (gMeasures.RPM > 2000 && fabs(error) < 0.4f)
	{
		gErrorInteg += error * gConfig.pidParam[1] * (LOOP_PERIOD * 0.001f);
		gErrorInteg = constrain(gErrorInteg, -0.3f, 0.3f);
	}
	else
	{
		// too much error, windup integral
		gErrorInteg = 0.0f;
	}
	gErrorDeriv = ((error - gLastError) * gConfig.pidParam[2]) / (LOOP_PERIOD * 0.001f);
	gErrorDeriv = boostHighpassFilter.input(gErrorDeriv);
	gErrorDeriv = constrain(gErrorDeriv, -0.5f, 0.5f);
	gLastMAP = gMeasures.MAP;
	gLastError = error;
	
	gMeasures.TARGET_OUTPUT = P + gErrorInteg + gErrorDeriv;
	gMeasures.TARGET_OUTPUT = constrain(gMeasures.TARGET_OUTPUT, 0.0f, 2.0f);
	
	// Convert target output to soneloid DC
	// We need a convertion constant to map a boost value to a solenoid DC
	// target output is in absolut pressure, convert if to relative pressure.
	// DC is updated only once per sol cycle
	static uint8_t  cycleIndex = 0;
//  static uint8_t  testIndex = 0;
	if (cycleIndex == 0)
	{
		float relativeTarget = gMeasures.TARGET_OUTPUT;
		float rawIndex = constrain(relativeTarget / 1.25f * 5.0f, 0.0f, 5.0f);
		float columnIndex = constrain(gMeasures.LOAD / 100.0f * 3.0f, 0.0f, 3.0f);
		
		// interpolate DC values from table
		uint8_t r1 = (int)floor(rawIndex);
//    	uint8_t r2 = constrain(r1 + 1, 0, 5);
//		uint8_t c1 = (int)floor(columnIndex);

		float ic[2];
		ic[0] = interp(gConfig.dutyCycleTable[r1], columnIndex);
		ic[1] = interp(gConfig.dutyCycleTable[r1+1], columnIndex);

		// and finaly compute the solenoid DC! 
		float dc = interpf(ic, rawIndex - r1);
		gMeasures.SOL_DC = dc;

//  	gMeasures.SOL_DC = testIndex * 20;
//  	testIndex += 1;
//  	if (testIndex > 5)
//    		testIndex = 0;
	}

	// At last, prevent the solenoid to activate if the speed if low
	if (gMeasures.SPEED < 8.0f)
	{
		gMeasures.SOL_DC = 0.0f;
	}

	// Take simulation into account
	float effectiveSolDc = gMeasures.SOL_DC;
	if (gSimulation.FORCE_WG != 0)
	{
		effectiveSolDc = gSimulation.FORCE_WG;
	}
  
	// produce output value
	// --------------------
	// This first version use a simple software approch to implement the PWM: 
	// Based on a 20Hz cycle, the PWD is managed by the main loop running @100Hz.
	// This leaves 5 loops per cycle, leading to 6 PWM levels :
	//  0, 20, 40, 60, 80, 100%
	if ((effectiveSolDc + 40) / 20.0f > cycleIndex+2)
	{
		// switch solenoid on
		digitalWrite(SOLENOID_OUT, true);
	}
	else
	{
		// switch selonoid off
		digitalWrite(SOLENOID_OUT, false);
	}

	// Manage fuel pump prime
	manageFuelPump();
	
	cycleIndex++;
	if (cycleIndex >= 5)
	{
		cycleIndex = 0;
	}

	previousLoopDate = now;
}

// Fast comm protocol.
// Based on 1 or 2 bytes data only.
class FastComm
{
private:

	enum STATE
	{
		RECEIVING,
		SENDING
	};

	STATE mState = RECEIVING;

	// RX/TX buffer
	char mBuffer[4];
	uint8_t mRxSize = 0;
	uint8_t mTxSize = 0;
	uint8_t mTxCounter = 0;

	void reset()
	{
		mRxSize = 0;
		mTxSize = 0;
		mState = RECEIVING;
	}

	uint8_t& getByteParam(uint16_t addr)
	{
		static uint8_t placeHolder;
		if (addr == 0x003)
		{
			return gMeasures.GEAR;
		}
		else if (addr >= 0x008 and addr < 0x008 + 6*4)
		{
			// Boost x Load to WGDC 
			return ((uint8_t*)gConfig.dutyCycleTable)[addr - 0x008];
		}
		else if (addr >= 0x020 and addr < 0x020 + 6*7)
		{
			// Gear x RPM boost correction table
			return ((uint8_t*)gConfig.boostTable)[addr - 0x020];
		}
		else if (addr >= 0x04a and addr < 0x04a + 6)
		{
			// Throttle boost correction table
			return ((uint8_t*)gConfig.throttleBoostTable)[addr - 0x04a];
		}
		else if (addr >= 0x050 and addr < 0x050 + 7)
		{
			// Throttle deriv boost correction table
			return ((uint8_t*)gConfig.throttleDerivBoostTable)[addr - 0x050];
		}
		else if (addr == 0x057)
		{
			return gSimulation.FORCE_WG;
		}
		else if (addr == 0x058)
		{
			return gInfo.versionMajor;
		}
		else if (addr == 0x059)
		{
			return gInfo.versionMinor;
		}
		else if (addr == 0x05a)
		{
			return gConfig.throttleScale;
		}
		else if (addr == 0x05c)
		{
			return gConfig.fuelPumpPrime;
		}

		return placeHolder;
	}

	float& getFloatParam(uint16_t addr)
	{
		static float placeHolder;
 		switch(addr)
		{
		case 0x000:
			return gMeasures.MAP;
		case 0x001:
			return gMeasures.THROTTLE;
		case 0x002:
			return gMeasures.SOL_DC;
		case 0x004:
			return gMeasures.LOAD;
		case 0x005:
			return gMeasures.CHIP_LOAD;
		case 0x006:
			return gMeasures.TARGET_BOOST;
		case 0x007:
			return gMeasures.TARGET_OUTPUT;
		case 0x05b:
			return gMeasures.FUEL_PRESS;
		case 0x800:
			return gConfig.tyreCircum;
		case 0x801:
			return gConfig.gearRatios[0];
		case 0x802:
			return gConfig.gearRatios[1];
		case 0x803:
			return gConfig.gearRatios[2];
		case 0x804:
			return gConfig.gearRatios[3];
		case 0x805:
			return gConfig.gearRatios[4];
		case 0x806:
			return gConfig.gearRatios[5];
		case 0x807:
			return gConfig.speedFactor;
		case 0x808:
			return gConfig.boostReference;
		case 0x809:
			return gConfig.pidParam[0];
		case 0x80a:
			return gConfig.pidParam[1];
		case 0x80b:
			return gConfig.pidParam[2];
		case 0x80c:
			return gMeasures.RPM;
		case 0x80d:
			return gMeasures.SPEED;
		case 0x80e:
			return gMeasures.THROTTLE_DERIV;
		case 0x80f:
			return gMeasures.AIR_FLOW;
		}

		return placeHolder;
	}

	struct Accessor
	{
		bool mIsFloat;
		uint16_t mAddr;
		float mFactor;
		bool mSigned;
		bool mSavedInConf;
	};

#define BYTE_TABLE_PARAM_LINE4(addr) \
	{false, addr+0, 1.0f, false, true}, \
	{false, addr+1, 1.0f, false, true}, \
	{false, addr+2, 1.0f, false, true}, \
	{false, addr+3, 1.0f, false, true}, 

#define BYTE_TABLE_PARAM_LINE6(addr) \
	{false, addr+0, 1.0f, false, true}, \
	{false, addr+1, 1.0f, false, true}, \
	{false, addr+2, 1.0f, false, true}, \
	{false, addr+3, 1.0f, false, true}, \
	{false, addr+4, 1.0f, false, true}, \
	{false, addr+5, 1.0f, false, true},

#define BYTE_TABLE_PARAM_LINE7(addr) \
	{false, addr+0, 1.0f, false, true}, \
	{false, addr+1, 1.0f, false, true}, \
	{false, addr+2, 1.0f, false, true}, \
	{false, addr+3, 1.0f, false, true}, \
	{false, addr+4, 1.0f, false, true}, \
	{false, addr+5, 1.0f, false, true}, \
	{false, addr+6, 1.0f, false, true},

	static const constexpr Accessor mByteAccessors[] =
	{
		//float? addr  factor signed? saved?
		{true,  0x000,  50.0f, false, false},	// MAP
		{true,  0x001,   1.0f, false, false},	// Throttle
		{true,  0x002,   1.0f, false, false},	// WFDC
		{false, 0x003,   1.0f, false, false},	// Gear
		{true,  0x004,   1.0f, false, false},	// Engine load
		{true,  0x005,   1.0f, false, false},	// CPU load
		{true,  0x006,  50.0f, false, false},	// Target boost
		{true,  0x007,  50.0f, false, false},	// Target output

		// Sol DC table : boost x load (6x4)
		BYTE_TABLE_PARAM_LINE4(0x008+4*0)
		BYTE_TABLE_PARAM_LINE4(0x008+4*1)
		BYTE_TABLE_PARAM_LINE4(0x008+4*2)
		BYTE_TABLE_PARAM_LINE4(0x008+4*3)
		BYTE_TABLE_PARAM_LINE4(0x008+4*4)
		BYTE_TABLE_PARAM_LINE4(0x008+4*5)

		// Boost cor table : Gear x RPM (6x7)
		BYTE_TABLE_PARAM_LINE7(0x020+7*0)
		BYTE_TABLE_PARAM_LINE7(0x020+7*1)
		BYTE_TABLE_PARAM_LINE7(0x020+7*2)
		BYTE_TABLE_PARAM_LINE7(0x020+7*3)
		BYTE_TABLE_PARAM_LINE7(0x020+7*4)
		BYTE_TABLE_PARAM_LINE7(0x020+7*5)

		// Boost correction table : throttle 
		BYTE_TABLE_PARAM_LINE6(0x04a)
		// Boost correction table : throttle deriv 
		BYTE_TABLE_PARAM_LINE7(0x050)

		{false,  0x057,  1.0f, false, false},	// Test WG
		{false,  0x058,  1.0f, false, false},	// Version major
		{false,  0x059,  1.0f, false, false},	// Version minor
		{false,  0x05a,  1.0f, false, true},	// Throttle sensor adjust
		{true,   0x05b, 25.0f, false, false},	// Fuel pressure
		{false,  0x05c,  1.0f, false, true},	// Fuel pump prime (s)
	};

	static const constexpr Accessor mWordAccessors[] =
	{
		//float? addr  factor signed? saved?
		{true,  0x800, 1000.0f, false, true},	// Tire circ

		{true,  0x801, 1000.0f, false, true},	// 1st gear ratio
		{true,  0x802, 1000.0f, false, true},	// 2nd gear ratio
		{true,  0x803, 1000.0f, false, true},	// 3rd gear ratio
		{true,  0x804, 1000.0f, false, true},	// 4th gear ratio
		{true,  0x805, 1000.0f, false, true},	// 5th gear ratio
		{true,  0x806, 1000.0f, false, true},	// 6th gear ratio

		{true,  0x807, 1000.0f, false, true},	// Speed correction
		{true,  0x808, 1000.0f, false, true},	// Reference boost

		{true,  0x809, 1000.0f, false, true},	// P
		{true,  0x80a, 1000.0f, false, true},	// I
		{true,  0x80b, 1000.0f, false, true},	// D

		{true,  0x80c,    1.0f, false, false},	// RPM
		{true,  0x80d,    1.0f, false, false},	// Speed
		{true,  0x80e,    1.0f, true,  false},	// Throttle deriv
		{true,  0x80f,    1.0f, false, false},	// Air flow Hz
	};

	uint8_t readByte(uint16_t addr)
	{
		const auto& accessor = mByteAccessors[addr];
		if (accessor.mIsFloat)
		{
			if (accessor.mSigned)
			{
				return uint8_t(constrain(getFloatParam(accessor.mAddr) * accessor.mFactor, -128, 127));
			}
			else
			{
				return uint8_t(constrain(getFloatParam(accessor.mAddr) * accessor.mFactor, 0, 255));
			}
		}
		else
		{
			if (accessor.mSigned)
			{
				return uint8_t(constrain(int8_t(getByteParam(accessor.mAddr)) * accessor.mFactor, -128, 127));
			}
			else
			{
				return uint8_t(constrain(getByteParam(accessor.mAddr) * accessor.mFactor, 0, 255));
			}		
		}
	}

	void writeByte(uint16_t addr, uint8_t value)
	{
		const auto& accessor = mByteAccessors[addr];
		if (accessor.mIsFloat)
		{
			if (accessor.mSigned)
			{
				getFloatParam(accessor.mAddr) = int8_t(value) / accessor.mFactor;
			}
			else
			{
				getFloatParam(accessor.mAddr) = value / accessor.mFactor;
			}
		}
		else
		{
			if (accessor.mSigned)
			{
				getByteParam(accessor.mAddr) = static_cast<uint8_t>(int8_t(value) / accessor.mFactor);
			}
			else
			{
				getByteParam(accessor.mAddr) = value / accessor.mFactor;
			}
		}

		if (accessor.mSavedInConf)
		{
			// Trigger a save in EEPROM
			gConfChanged = true;
		}
	}

	uint16_t readWord(uint16_t addr)
	{
		const auto& accessor = mWordAccessors[addr - 0x800];
		if (accessor.mIsFloat)
		{
			if (accessor.mSigned)
			{
				return static_cast<uint16_t>(constrain(getFloatParam(accessor.mAddr) * accessor.mFactor, -32768, 32767));
			}
			else
			{
				return static_cast<uint16_t>(constrain(getFloatParam(accessor.mAddr) * accessor.mFactor, 0, 65535));
			}
		}
		else
		{
			if (accessor.mSigned)
			{
				return static_cast<uint16_t>(constrain(int8_t(getByteParam(accessor.mAddr)) * accessor.mFactor, -32768, 32767));
			}
			else
			{
				return static_cast<uint16_t>(constrain(getByteParam(accessor.mAddr) * accessor.mFactor, 0, 65535));
			}
		}
	}

	void writeWord(uint16_t addr, uint16_t value)
	{
		const auto& accessor = mWordAccessors[addr - 0x800];
		if (accessor.mIsFloat)
		{
			if (accessor.mSigned)
			{
				getFloatParam(accessor.mAddr) = int16_t(value) / accessor.mFactor;
			}
			else
			{
				getFloatParam(accessor.mAddr) = int32_t(uint32_t(value)) / accessor.mFactor;
			}
		}
		else
		{
			getByteParam(accessor.mAddr) = value / accessor.mFactor;
		}
		
		if (accessor.mSavedInConf)
		{
			// Trigger a save in EEPROM
			gConfChanged = true;
		}
	}

	void processReceiving()
	{
		auto getAddr = [this] () -> uint16_t
		{
			return ((uint16_t(mBuffer[0]) & 0x0f) << 8) + uint8_t(mBuffer[1]);
		};

		auto startSend = [this](uint8_t size)
		{
			mTxSize = size;
			mTxCounter = 0;
			mState = SENDING;
		};

		while (Serial.available() > 0)
		{
			// receive one byte
			mBuffer[mRxSize++] = Serial.read();

			if ((mBuffer[0] & 0xf0) != 0x50 and (mBuffer[0] & 0xf0) != 0xa0)
			{
				// framing error
				reset();
			}
			else if ((mBuffer[0] & 0xf8) == 0x50 && mRxSize == 2)
			{
				// 1 byte read request
				mBuffer[0] = readByte(getAddr());
				startSend(1);
			}
			else if ((mBuffer[0] & 0xf8) == 0x58 && mRxSize == 2)
			{
				// 2 bytes word read request
				uint16_t word = readWord(getAddr());
				mBuffer[0] = static_cast<uint8_t>((word >> 8) & 0xff);
				mBuffer[1] = static_cast<uint8_t>(word & 0xff);
				startSend(2);
			}
			else if ((mBuffer[0] & 0xf8) == 0xA0 && mRxSize == 3)
			{
				// 1 byte write request
				writeByte(getAddr(), mBuffer[2]);
				mBuffer[0] = mBuffer[2];
				startSend(1);
			}
			else if ((mBuffer[0] & 0xf8) == 0xA8 && mRxSize == 4)
			{
				// 2 bytes word write request
				writeWord(getAddr(), (uint16_t(mBuffer[2]) << 8) + uint8_t(mBuffer[3]));
				uint16_t word = readWord(getAddr());
				mBuffer[0] = static_cast<uint8_t>((word >> 8) & 0xff);
				mBuffer[1] = static_cast<uint8_t>(word & 0xff);
				startSend(2);
			}
			else if (mRxSize >= 4)
			{
				// Framing error, reset
				reset();
			}
		};
	}

	void processSending()
	{
		while (Serial.availableForWrite() > 0 && mTxCounter < mTxSize)
		{
			Serial.write(mBuffer[mTxCounter++]);
		}
		if (mTxCounter == mTxSize)
		{
			mState = RECEIVING;
			mRxSize = 0;
			mTxCounter = 0;
			mTxSize = 0;
		}
	}

public:
	void manageSerial()
	{
		if (mState == RECEIVING)
		{
			processReceiving();
		}
		if (mState == SENDING)
		{
			processSending();
		}
	}
};

const constexpr FastComm::Accessor FastComm::mByteAccessors[];
const constexpr FastComm::Accessor FastComm::mWordAccessors[];

FastComm gFastCom;

uint32_t gLastSaveCheck = 0;

void loop()
{
	// 100 eval cycles per second
	uint32_t now = micros();
	
	if (now - gLastEvalCycle > LOOP_PERIOD * 1000)
	{
		evalCycle();
	 	gLastEvalCycle = now;
		
	 	uint32_t after = micros();
		
	 	// compute CPU load
	 	gMeasures.CHIP_LOAD = (after - now) * 100 / (LOOP_PERIOD * 1000.0f);
	}
	
	if (now - gLastSaveCheck > 1000000)
	{
	 	// every second, look for saving the conf
	 	if (gConfChanged)
	 	{
	 		saveConfig();
	 		gConfChanged = false;
	 	}
 	    gLastSaveCheck = now;
	}

	// manage serial interface
	gFastCom.manageSerial();
}
