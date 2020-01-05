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
  8 - 
	
  
  General structure:
  =================
  
  Data Acquisition:
  ----------------
  As we need to sample for 2 frequency values, we make use of pin change interrupt to
  trigger an interrupt when either RPM or speed signal.

  For RPM, the signal is composed of tree 5V pulse per revolution:
  The maximum frequency @8000RPM is therefore :
    8000 / 60 * 3 = 400Hz
  This lead to an interrupt frequency of 800Hz (pin change interrupt is triggered on each
  change, either raising or falling edge).
	
  Speed input is a square signal ranging from 0 to 220Hz (@300km/h).
  This lead to an interrupt frequency of 440Hz.
  
  The combined worst case interrupt load is therefore 1240Hz, which seams pretty easy to
  sustain with the arduino running @16Mhz.

  MAP is captured on an analog input. A little filtering may be needed to denoise the signal.
  
  Throttle position is captured on an analog input. A little filtering may be needed to 
  denoise the signal.
  
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

  Configuation & logging protocol (I2C)
  -------------------------------------
  BC is I2C slave, it support read or write command.

  Read :
  [I2C ADDR] [
  
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
	Addr		Type	Default	Comment
	0x0000		float	 2.02	Tyre circumference 
	0x0001		float	12.200	1st gear ratio
	0x0002		float	 6.908	2nd gear ratio
	0x0003		float	 4.383	3rd gear ratio
	0x0004		float	 3.271	4th gear ratio
	0x0005		float	 2.620	5th gear ratio
	0x0006		float	 1.000	6th gear ratio
	0x0007		float	 0.73	Hz/km/h, speed sensor conversion factor
	0x0008		float	 0.7	Reference boost for bost table (bar)
	
	0x0010		float	 0.5	PID P parameter
	0x0011		float	 0.05	PID I parameter
	0x0012		float	 0.005	PID D parameter
	
	0x0020		float	 x		Current RPM
	0x0021		float	 x		Current SPEED (km/h)
	0x0022		float	 x		Current MAP (bar) 
	0x0023		float	 x		Current throotle position (%)
	0x0024		float	 x		Current solenoid DC (%)
	0x0025		byte	 x		Current gear
	0x0026		float	 x		Current engine load (%)
	0x0027		float  x    CPU load (%)
	0x0028		float  x    Target Boost (bar)
	0x0029		float  x    Target output (bar)
	0x002a		float  x    Throttle pos deriv (%/s)
	0x002b		float  x    Air flow (Hz)

	0x0030		byte[5][4] x	Boost vs load table used to convert the target boost into solenoid duty cycle
	0x0044
 	
	0x0100		byte[7] 1st gear boost table
	0x0110		byte[7] 2nd gear boost table
	0x0120		byte[7] 3rd gear boost table
	0x0130		byte[7] 4th gear boost table
	0x0140		byte[7] 5th gear boost table
	0x0150		byte[7] 6th gear boost table

	0x160		byte[6] Throttle boost adj
	0x170		byte[7] Throttle' boost adj

  # Testing command
  0x200   byte    Force waste gate output ratio (0..100)

	0x1000		byte		x	Firmware version, major
	0x1001		byte		x	Firmware version, minor
	
  Pin mapping
  -----------
  
	RPM input		  :	PIN 12, Port B4
	Speed input		:	PIN 11, Port B3
	Throttle input:	PIN A7, ADC7
	MAP input	 	  :	PIN A6, ADC6
	
	Solenoid output	:	PIN 13, PB5
  Debug RPM replay: PIN 10, PB2
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


FilterOnePole pistonFilter( LOWPASS, 5.0f ); 
FilterOnePole lowpassFilter( LOWPASS, 10.0f ); 
FilterOnePole targetLowpassFilter( LOWPASS, 2.0f ); 

#define VERSION_MAJOR	0
#define VERSION_MINOR	1
 
// I/O Pins 
#define RPM_IN	12
#define SPEED_IN 11
#define AIRFLOW_IN 10
#define SOLENOID_OUT 13
#define DEBUG_OUT 9
//#define SOLENOID_OUT 10
//#define MAP_IN A6		
//#define THROTTLE_IN A7	
#define MAP_IN A5    
#define THROTTLE_IN A6  

// Debug PIN
#define SPEED_OUT 16
#define RPM_OUT 15

// Other constants
#define LOOP_PERIOD 10	// loop period in ms


#define CONFIG_VER 0

// Configuration tables
struct Config
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
  
	// Boost reference for boost table, relative presure
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
uint32_t  gLastAirFlowDate = 0;
uint32_t  gAirFlowBuffer[4] = {0,0,0,0};
uint8_t   gAirFlowBufferHead = 0;
uint32_t  gAirFlowAccum = 0;
// RPM period in µs, 0 means no signal.
volatile uint32_t	gRPMPeriod = INT32_MAX;
// SPEED period in µs, 0 means no signal.
volatile uint32_t	gSPEEDPeriod = INT32_MAX;
// Air flow period in µs, 0 means no signal.
volatile uint32_t  gAirFlowPeriod = INT32_MAX;

// Main loop variables
uint32_t	gLastEvalCycle = 0;
float gLastThrottle = 0.0f;
float gThrottleDerivFilter[16] = {0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0};
int gThrottleDerivFilterIndex = 0;

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
//    digitalWrite(13,digitalRead(RPM_IN) and digitalRead(SPEED_IN));
//    TOGGLE_DEBUG
//    digitalWrite(RPM_OUT, rpmOut);    
    rpmOut = !rpmOut;

		// RPM changed !
		uint32_t period = now - gLastRPMDate;
		gLastRPMDate = now;
		gRPMAccum -= gRPMBuffer[gRPMBufferHead];
		gRPMBuffer[gRPMBufferHead++] = period;
		gRPMBufferHead &= 0x3; 
		gRPMAccum += period;
		
		gRPMPeriod = gRPMAccum>>2;
//    gRPMPeriod = period;
	}
	// Check SPEED raising edge
	if ((changed & 0x08) && (b & 0x08))
	{
//    digitalWrite(SPEED_OUT, speedOut);
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
    // Air flow changed !
    uint32_t period = now - gLastAirFlowDate;
    gLastAirFlowDate = now;
    gAirFlowAccum -= gAirFlowBuffer[gAirFlowBufferHead];
    gAirFlowBuffer[gAirFlowBufferHead++] = period;
    gAirFlowBufferHead &= 0x3;
    gAirFlowAccum += period;
    
    gAirFlowPeriod = gAirFlowAccum>>2;
//    gAirFlowPeriod = period;
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
  Serial.write("\n");

	if (refVi != vi)
	{
		// no data, or need conversion
		return;
	}
  
  Serial.write("Loading conf\n");
	EEPROM.get(16, gConfig);
  Serial.write("Conf loaded\n");
}

void setup()
{
  // Init serial port @57600 baud, 8N1
  Serial.begin(57600);

  Serial.write("init\n");
  Serial.write("load config\n");

	// load conf
	loadConfig();

  Serial.write("set pinMode\n");

	// Configure IO pins
//	pinMode(8, INPUT_PULLUP);
//	pinMode(9, INPUT_PULLUP);
//	pinMode(10, INPUT_PULLUP);
//	pinMode(11, INPUT_PULLUP);
//	pinMode(12, INPUT_PULLUP);
//	pinMode(13, INPUT_PULLUP);
	pinMode(RPM_IN, INPUT_PULLUP);
	pinMode(SPEED_IN, INPUT_PULLUP);
  pinMode(AIRFLOW_IN, INPUT);
	pinMode(SOLENOID_OUT, OUTPUT);
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

float pistonPos = 0.0f;
float spool = 0.0f;

void evalCycle()
{
	// read input and process inputs
	//------------------------------
	uint8_t oldSREG = SREG;
	cli();
	uint32_t RPMPeriod = gRPMPeriod;
	uint32_t SPEEDPeriod = gSPEEDPeriod;
  uint32_t airFlowPeriod = gAirFlowPeriod;
  uint32_t lastRPMDate = gLastRPMDate;
  uint32_t lastSPEEDDate = gLastSPEEDDate;
  uint32_t lastAirFlowDate = gLastAirFlowDate;
	SREG = oldSREG;

  uint32_t now = micros();
  
	// Analog read : very costly when done without interrupt : 100µs per read !
	// Read MAP. 1V per bar, 0V @ 0bar, 1V@1 bar (atmospheric presure)...
//	if (true)
//	{
		gMeasures.MAP = analogRead(MAP_IN) * (5.0f / 1023.f);
//	}
#if 0
	else
	{
    float dt = LOOP_PERIOD * 0.001f;
		// MAP simulation
    // simulate piston movement
    pistonPos += (gMeasures.MAP - 1.4f) * (1.0f - (gMeasures.SOL_DC * 0.01f)) * dt / 0.2f;
    pistonPos -= (gMeasures.SOL_DC * 0.01f) * dt / 0.2f;
//    pistonPos = pistonFilter.input(pistonPos);
    pistonPos = constrain(pistonPos, 0.0f, 1.0f);

    // turbo spool simulation
    spool += 200000.0f * gMeasures.LOAD * 0.01f * (1.0 - pistonPos * 0.9f) * dt;
    spool -= spool * 0.5f * dt - 100.0f;
    spool = constrain(spool, 0.0f, 170000.0f);
   
		gMeasures.MAP = gMeasures.MAP  - (gMeasures.MAP * 0.8f * dt) - 0.001f
//		  + gMeasures.THROTTLE * 0.01f * (1.0f - pistonPos) * gMeasures.LOAD * 0.01f * 0.8f * (LOOP_PERIOD * 0.001f);
//      + gMeasures.THROTTLE * 0.01f * (1.0f - pistonPos) * ((gMeasures.LOAD + 1.0f) * 0.01f * 2.0f) * ((gMeasures.LOAD + 1.0f) * 0.01f * 2.0f) * dt;
      + (1.0f - pistonPos) * (spool / 100000.0) * (spool / 100000.0) * dt;
//    gMeasures.MAP = constrain(gMeasures.MAP, gMeasures.THROTTLE * 0.01f, gMeasures.MAP);
    gMeasures.MAP *= gMeasures.THROTTLE * 0.01f;
    gMeasures.MAP = max(gMeasures.MAP, gMeasures.THROTTLE * 0.01f);
	} 
#endif

  // Throttle analog read. 0..100% => 0..5V
	gMeasures.THROTTLE = analogRead(THROTTLE_IN) * (100.0f / 1023.f);
	gThrottleDerivFilter[gThrottleDerivFilterIndex] = (gMeasures.THROTTLE - gLastThrottle) / (LOOP_PERIOD * 0.001f);
	gThrottleDerivFilterIndex += 1;
	gThrottleDerivFilterIndex = gThrottleDerivFilterIndex % 16;
	gMeasures.THROTTLE_DERIV = 0.0f;
	for (int i=0; i<16; ++i)
	{
		gMeasures.THROTTLE_DERIV += gThrottleDerivFilter[i];
	}
	gLastThrottle = gMeasures.THROTTLE;

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
  // Detect 0 Airflow: last pulse > 10Hz period (0.1s): 100.000µs
  if (airFlowPeriod > 0 and (now - lastAirFlowDate) > 100000)
  {
    // 0 stand for 0 speed
    airFlowPeriod = 0;
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
  if (airFlowPeriod > 0)
  {
    gMeasures.AIR_FLOW = 1000000.0f / (airFlowPeriod);
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
	{
		float rpmIndex = (gMeasures.RPM / 1000.0f) - 1;
		rpmIndex = constrain(rpmIndex, 0, 6);
		
		gMeasures.TARGET_BOOST = 
				interp(gConfig.boostTable[gMeasures.GEAR-1], rpmIndex) * 0.01f
				* (gConfig.boostReference);
	}

  // Apply throttle correction
  {
    // throttle position correction
    float throttleIndex = gMeasures.THROTTLE * (sizeof(gConfig.throttleBoostTable)-1) * 0.01f;
    throttleIndex = constrain(throttleIndex, 0, sizeof(gConfig.throttleBoostTable)-1);

    gMeasures.TARGET_BOOST = gMeasures.TARGET_BOOST * interp(gConfig.throttleBoostTable, throttleIndex) * 0.01f;

    // Throttle pos variation correction
    float throttleDerivIndex = 3 + gMeasures.THROTTLE_DERIV * (sizeof(gConfig.throttleDerivBoostTable)-1) * 0.001f;
    throttleDerivIndex = constrain(throttleDerivIndex, 0, sizeof(gConfig.throttleDerivBoostTable)-1);

    gMeasures.TARGET_BOOST = gMeasures.TARGET_BOOST * interp(gConfig.throttleDerivBoostTable, throttleDerivIndex) * 0.01f;
//    gMeasures.TARGET_BOOST = interp(gConfig.throttleDerivBoostTable, throttleDerivIndex);
//    gMeasures.TARGET_BOOST = throttleDerivIndex;

    // Convert boost value into MAP value, just add the atmo pressure ;)
    gMeasures.TARGET_BOOST += 1.0f;
  }

  gMeasures.TARGET_BOOST = targetLowpassFilter.input(gMeasures.TARGET_BOOST);
	
	// 2nd, apply PID filter to compute boost error
	float error = gMeasures.TARGET_BOOST - (gMeasures.MAP);
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
//	gErrorDeriv += ((gMeasures.MAP - gLastMAP) * gConfig.pidParam[2]) / (LOOP_PERIOD * 0.001f);
  gErrorDeriv = ((error - gLastError) * gConfig.pidParam[2]) / (LOOP_PERIOD * 0.001f);
  gErrorDeriv = lowpassFilter.input(gErrorDeriv);
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
    ic[0]= interp(gConfig.dutyCycleTable[r1], columnIndex);
    ic[1] = interp(gConfig.dutyCycleTable[r1+1], columnIndex);

    // and finaly compute the solenoid DC! 
    float dc = interpf(ic, rawIndex - r1);
    gMeasures.SOL_DC = dc;

//  gMeasures.SOL_DC = testIndex * 20;
//  testIndex += 1;
//  if (testIndex > 5)
//    testIndex = 0;
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
	
	cycleIndex++;
	if (cycleIndex >= 5)
  {
		cycleIndex = 0;
  }
}

// Fast comm protocol.
// Base on 1 or 2 bytes data only.
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
		if (addr == 0x0003)
		{
			return gMeasures.GEAR;
		}
		else if (addr >= 0x0008 and addr < 0x0008 + 24)
		{
			// Boost x Load to WGDC 
			return ((uint8_t*)gConfig.dutyCycleTable)[addr - 0x0008];
		}
		else if (addr >= 0x0020 and addr < 0x0020 + 42)
		{
			// Gear x RPM boost correction table
			return gConfig.boostTable[0][addr - 0x0020];
		}
		else if (addr >= 0x004a and addr < 0x004a + 6)
		{
			// Throttle boost correction table
			return ((uint8_t*)gConfig.throttleBoostTable)[addr - 0x004a];
		}
		else if (addr >= 0x0050 and addr < 0x0050 + 7)
		{
			// Throttle deriv boost correction table
			return ((uint8_t*)gConfig.throttleDerivBoostTable)[addr - 0x0050];
		}
		else if (addr == 0x0057)
		{
			return gSimulation.FORCE_WG;
		}
		else if (addr == 0x0058)
		{
			return gInfo.versionMajor;
		}
		else if (addr == 0x0059)
		{
			return gInfo.versionMinor;
		}
	}

	float& getFloatParam(uint16_t addr)
	{
 		switch(addr)
		{
		case 0x0000:
			return gMeasures.MAP;
		case 0x0001:
			return gMeasures.THROTTLE;
		case 0x0002:
			return gMeasures.SOL_DC;
		case 0x0004:
			return gMeasures.LOAD;
		case 0x0005:
			return gMeasures.CHIP_LOAD;
		case 0x0006:
			return gMeasures.TARGET_BOOST;
		case 0x0007:
			return gMeasures.TARGET_OUTPUT;
		case 0x0800:
			return gConfig.tyreCircum;
		case 0x0801:
			return gConfig.gearRatios[0];
		case 0x0802:
			return gConfig.gearRatios[1];
		case 0x0803:
			return gConfig.gearRatios[2];
		case 0x0804:
			return gConfig.gearRatios[3];
		case 0x0805:
			return gConfig.gearRatios[4];
		case 0x0806:
			return gConfig.gearRatios[5];
		case 0x0807:
			return gConfig.speedFactor;
		case 0x0808:
			return gConfig.boostReference;
		case 0x0809:
			return gConfig.pidParam[0];
		case 0x080a:
			return gConfig.pidParam[1];
		case 0x080b:
			return gConfig.pidParam[2];
		case 0x080c:
			return gMeasures.RPM;
		case 0x080d:
			return gMeasures.SPEED;
		case 0x080e:
			return gMeasures.THROTTLE_DERIV;
		case 0x080f:
			return gMeasures.AIR_FLOW;
		}
	}

	struct Accessor
	{
		bool mIsFloat;
		uint16_t mAddr;
		float mFactor;
		bool mSigned;
	};

	static const constexpr Accessor mByteAccessors[] =
	{
		{true,  0x000, 50.0f, false},	// MAP
		{true,  0x001, 1.0f,  false},	// Throttle
		{true,  0x002, 1.0f,  false},	// WFDC
		{false, 0x003, 1.0f,  false},	// Gear
		{true,  0x004, 1.0f,  false},	// Engine load
		{true,  0x005, 1.0f,  false},	// CPU load
		{true,  0x006, 50.0f, false},	// Target boost
		{true,  0x007, 50.0f, false},	// Target output

		{false,  0x008, 1.0f, false},	// Boost x load to WGDC
		{false,  0x009, 1.0f, false},	// Boost x load to WGDC
		{false,  0x00a, 1.0f, false},	// Boost x load to WGDC
		{false,  0x00b, 1.0f, false},	// Boost x load to WGDC
		{false,  0x00c, 1.0f, false},	// Boost x load to WGDC
		{false,  0x00d, 1.0f, false},	// Boost x load to WGDC
		{false,  0x00e, 1.0f, false},	// Boost x load to WGDC
		{false,  0x00f, 1.0f, false},	// Boost x load to WGDC
		{false,  0x010, 1.0f, false},	// Boost x load to WGDC
		{false,  0x011, 1.0f, false},	// Boost x load to WGDC
		{false,  0x012, 1.0f, false},	// Boost x load to WGDC
		{false,  0x013, 1.0f, false},	// Boost x load to WGDC
		{false,  0x014, 1.0f, false},	// Boost x load to WGDC
		{false,  0x015, 1.0f, false},	// Boost x load to WGDC
		{false,  0x016, 1.0f, false},	// Boost x load to WGDC
		{false,  0x017, 1.0f, false},	// Boost x load to WGDC
		{false,  0x018, 1.0f, false},	// Boost x load to WGDC
		{false,  0x019, 1.0f, false},	// Boost x load to WGDC
		{false,  0x01a, 1.0f, false},	// Boost x load to WGDC
		{false,  0x01b, 1.0f, false},	// Boost x load to WGDC
		{false,  0x01c, 1.0f, false},	// Boost x load to WGDC
		{false,  0x01d, 1.0f, false},	// Boost x load to WGDC
		{false,  0x01e, 1.0f, false},	// Boost x load to WGDC
		{false,  0x01f, 1.0f, false},	// Boost x load to WGDC
	};

	static const constexpr Accessor mWordAccessors[] =
	{
		{true,  0x800, 1000.0f, false},	// Tire circ

		{true,  0x801, 1000.0f, false},	// 1st gear ratio
		{true,  0x802, 1000.0f, false},	// 2nd gear ratio
		{true,  0x803, 1000.0f, false},	// 3rd gear ratio
		{true,  0x804, 1000.0f, false},	// 4th gear ratio
		{true,  0x805, 1000.0f, false},	// 5th gear ratio
		{true,  0x806, 1000.0f, false},	// 6th gear ratio

		{true,  0x807, 1000.0f, false},	// Speed correction
		{true,  0x809, 1000.0f, false},	// Reference boost

		{true,  0x80a, 1000.0f, false},	// P
		{true,  0x80a, 1000.0f, false},	// I
		{true,  0x80b, 1000.0f, false},	// D

		{true,  0x80c, 1.0f, false},	// RPM
		{true,  0x80d, 1.0f, false},	// Speed
		{true,  0x80e, 1.0f, true},		// Throttle deriv
		{true,  0x80f, 1.0f, false},	// Air flow Hz
	};

	uint8_t readByte(uint16_t addr)
	{
		const auto& accessor = mByteAccessors[addr];
		if (accessor.mIsFloat)
		{
			if (accessor.mSigned)
			{
				return static_cast<uint8_t>(constrain(getFloatParam(accessor.mAddr) * accessor.mFactor, -128, 127));
			}
			else
			{
				return static_cast<uint8_t>(constrain(getFloatParam(accessor.mAddr) * accessor.mFactor, 0, 255));
			}
		}
		else
		{
			if (accessor.mSigned)
			{
				return static_cast<uint8_t>(constrain(int8_t(getByteParam(accessor.mAddr)) * accessor.mFactor, -128, 127));
			}
			else
			{
				return static_cast<uint8_t>(constrain(getByteParam(accessor.mAddr) * accessor.mFactor, 0, 255));
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
				getFloatParam(accessor.mAddr) = value / accessor.mFactor;
			}
			
		}
		else
		{
			getByteParam(accessor.mAddr) = value / accessor.mFactor;
		}
	}

	void processReceiving()
	{
		auto getAddr = [this] () -> uint16_t
		{
			return ((uint16_t(mBuffer[0]) & 0x0f) << 8) + mBuffer[1];
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
				writeWord(getAddr(), (uint16_t(mBuffer[2]) << 8) + mBuffer[3]);
				mBuffer[0] = mBuffer[2];
				mBuffer[1] = mBuffer[3];
				startSend(2);
			}
			else if (mRxSize == 4)
			{
				// Framing error, reset
				reset();
			}
		};
	}

	void processSending()
	{
		if (Serial.availableForWrite() > 0)
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

class CommManager
{
	enum State
	{
		SYNCING,
		WAIT_CMD,
		WAIT_END,
	};
	
	State	mState = SYNCING;
	
	int		mRxCounter = 0;
	uint8_t	 mBuffer[10];
	uint32_t mLastRx = 0;
	int		mWaitRx = 0;

	const int mMsgSize[4] = {5, 5+1, 5, 5+4};
	

	void reset()
	{
		mRxCounter = 0;
		mState = SYNCING;
	}
	
	uint8_t* getByteRef(uint16_t addr)
	{
		if (addr >= 0x0030 and addr < 0x0048)
		{
			return &((uint8_t*)gConfig.dutyCycleTable)[addr - 0x0030];
		}
		else if (addr >= 0x100 and addr < 0x100+7)
		{
//			return &((uint8_t*)gConfig.boostTable[0])[addr - 0x0100];
			return &gConfig.boostTable[0][addr - 0x0100];
		}
		else if (addr >= 0x110 and addr < 0x110+7)
		{
			return &((uint8_t*)gConfig.boostTable[1])[addr - 0x0110];
		}
		else if (addr >= 0x120 and addr < 0x120+7)
		{
			return &((uint8_t*)gConfig.boostTable[2])[addr - 0x0120];
		}
		else if (addr >= 0x130 and addr < 0x130+7)
		{
			return &((uint8_t*)gConfig.boostTable[3])[addr - 0x0130];
		}
		else if (addr >= 0x140 and addr < 0x140+7)
		{
			return &((uint8_t*)gConfig.boostTable[4])[addr - 0x0140];
		}
		else if (addr >= 0x150 and addr < 0x150+7)
		{
			return &((uint8_t*)gConfig.boostTable[5])[addr - 0x0150];
		}
		else if (addr >= 0x160 and addr < 0x160+6)
		{
			return &((uint8_t*)gConfig.throttleBoostTable)[addr - 0x0160];
		}
		else if (addr >= 0x170 and addr < 0x170+7)
		{
			return &((uint8_t*)gConfig.throttleDerivBoostTable)[addr - 0x0170];
		}
 
		switch(addr)
		{
		case 0x0025:
			return &gMeasures.GEAR;
	  case 0x200:
      return &gSimulation.FORCE_WG;
   	case 0x1000:
			return &gInfo.versionMajor;
		case 0x1001:
			return &gInfo.versionMinor;
		}

		return nullptr;
	}
	
	float* getFloatRef(uint16_t addr)
	{
		switch (addr)
		{
		case 0x0000:
			return &gConfig.tyreCircum;

		case 0x0001:
			return &gConfig.gearRatios[0];
		case 0x0002:
			return &gConfig.gearRatios[1];
		case 0x0003:
			return &gConfig.gearRatios[2];
		case 0x0004:
			return &gConfig.gearRatios[3];
		case 0x0005:
			return &gConfig.gearRatios[4];
		case 0x0006:
			return &gConfig.gearRatios[5];
		case 0x0007:
			return &gConfig.speedFactor;
		case 0x0008:
			return &gConfig.boostReference;
		case 0x0010:
			return &gConfig.pidParam[0];
		case 0x0011:
			return &gConfig.pidParam[1];
		case 0x0012:
			return &gConfig.pidParam[2];
		
		case 0x0020:
			return &gMeasures.RPM;
		case 0x0021:
			return &gMeasures.SPEED;
		case 0x0022:
			return &gMeasures.MAP;
		case 0x0023:
			return &gMeasures.THROTTLE;
		case 0x0024:
			return &gMeasures.SOL_DC;
		case 0x0026:
			return &gMeasures.LOAD;
    case 0x0027:
      return &gMeasures.CHIP_LOAD;
    case 0x0028:
      return &gMeasures.TARGET_BOOST;
    case 0x0029:
      return &gMeasures.TARGET_OUTPUT;
    case 0x002a:
      return &gMeasures.THROTTLE_DERIV;
    case 0x002b:
      return &gMeasures.AIR_FLOW;
}

    static float failed = NAN;
		return &failed;
	}
	
	void processCommand()
	{
		uint16_t addr = uint16_t(mBuffer[3]) * 0x100 + mBuffer[2];
		uint8_t* pi;
		float*	pf;
		
//		uint8_t respBuffer[10];
		
		switch (mBuffer[1])
		{
		case 0:
			// read byte
			pi = getByteRef(addr);
			// write response to serial
      if (pi != NULL)
      {
  			mBuffer[1] = 1;
  			mBuffer[4] = *pi;
  			mBuffer[5] = 0x04;
  			Serial.write(mBuffer, 6);
      }
      else
      {
        mBuffer[1] = 4;
        mBuffer[4] = 0x04;
        Serial.write(mBuffer, 5);
      }
			break;
		case 1:
			// write byte
			pi = getByteRef(addr);
			*pi = mBuffer[4];
			// write response to serial : send a read var
			mBuffer[1] = 0;
			mBuffer[4] = 0x04;
			Serial.write(mBuffer, 5);
			
			// Mark config for saving
			gConfChanged = true;
			break;
		case 2:
			// read float
			pf = getFloatRef(addr);
			// write response to serial
			mBuffer[1] = 3;
			*reinterpret_cast<float*>(&mBuffer[4]) = *pf;
			mBuffer[8] = 0x04;
			Serial.write(mBuffer, 9);
			break;
		case 3:
			// write float
			pf = getFloatRef(addr);
			*pf = *(float*)&mBuffer[4];
			// write response to serial : send a read var
			mBuffer[1] = 2;
			mBuffer[4] = 0x04;
			Serial.write(mBuffer, 5);
			
			// Mark config for saving
			gConfChanged = true;
			break;
		}
	}
	
public:
	void manageSerial()
	{
		uint32_t now = millis();
		if (now - mLastRx > 1000)
		{
			// timeout !
			reset();
		}
		// manage serial line commands
		while (Serial.available() > 0)
		{
			if (mRxCounter == sizeof(mBuffer))
			{
				// buffer overflow !
				reset();
			}
			mBuffer[mRxCounter++] = Serial.read();
			mLastRx = now;
			
			switch (mState)
			{
			case SYNCING:
				if (mBuffer[0] == 0x01)
				{
					// Start of message
					mState = WAIT_CMD;
				}
				else
				{
					// not a valid start
					reset();
				}
				break;
			case WAIT_CMD:
				if (mBuffer[1] > sizeof(mMsgSize))
				{
					// invalid command! 
					reset();
				}
				else
				{
					mWaitRx = mMsgSize[mBuffer[1]];
					mState = WAIT_END;
				}
				break;
			case WAIT_END:
				if (mRxCounter == mWaitRx)
				{
					// check end marker
					if (mBuffer[mRxCounter-1] != 0x04)
					{
						// missing end marker !
						reset();
					}
					else
					{
						// message is complete, process it
						processCommand();
            reset();
					}
				}
				break;
			}
		}		
	}
};
CommManager gCommManager;

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
	gCommManager.manageSerial();
}
