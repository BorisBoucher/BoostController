//#include <avr/io.h> // Standard include for AVR
#include <atomic>
#include <cmath>
#include <functional>
#include <stdint.h>
#include <stddef.h>
#include <vector>
#include <stm32f1xx_ll_dma.h>

#include <FloatDefine.h>
#include <RunningStatistics.h>
//#include <parameters/include/ParameterDef.hpp>
#include <ParameterDef.hpp>
#include <FlashStore.hpp>


#include "std_helper.hpp"
#include "config.hpp"
#include "core/Inc/main.h"
#include "BoostController.hpp"
#include "InputCapture.hpp"
#include "AnalogInput.hpp"
#include "Interp.hpp"
#include <CANCommProvider.hpp>

//#include <EEPROM.h>

/*

  GTO/3000GT/Stealth Boost controller
  
  Boost controller read the following signals:
    - RPM (Hz)
    - Speed (Hz)
    - MAP (Analog), 0..5V, 1V/bar
    - Throttle (Analog), 0..5V (0..100%)
	- Fuel pressure (Analog), 
    
  Internal value:
    - Current gear is deduced from speed and RPM values. 
    - The target load (or boost) is computed based on:
       * throttle position,
       * current gear to limit torque on transmission in first gear!
       * RPM value (WOT load target can be set by step of 500 RPM) 
    
  From these inputs, the boost controller compute the boost set point (target boost in bar).
  Then a PWM signal for the waste gate solenoid is generated on output with a conversion table.

  Configuration and monitoring:
    - The controller communicated with a CAN link to:
      * Configure the boost controller parameter (target boost, correction table, servo parameters...)
      * Receive real time data collected by the boost controller to display and/or record for later analysis.

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
  Boost is controlled with a PID closed loop.
  The PID have three parameters that are store in a 1D table:
	P	Proportional term, give the basic behavior of the controller
	I	Integral term, correct long term residual error
	D	Derivative term, amortise the controller
	
  Conversion of PID boost to solenoid duty cycle
  ----------------------------------------------
  Once the PID controller computed the 'boost' value to inject in the system, we need a way
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
  waste gate solenoid.

  Automatic throttle position calibration
  ---------------------------------------
  In order to account for throttle position measurement not being @100%, an automatic calibration procedure
  is provided :
    
	* With RPM @0, if the throttle measurement is greater then 80% and steady during 3s, the throttle value
	will be considered as the maximum throttle position (i.e. 100% scale).
	* The BC confirm the new calibration by producing 3 pulses of 1/4s @50% DC on the BC sol.


  Configuration & logging protocol (CAN)
  -----------------------------------------
  Conf&Log protocol allow to read/publish acquisition data or internal states and read or write the parameter tables.
  
  The protocol is based on CAN bus. CAN id is used to identify parameter or acquisition data.
  Data are encoded as 1 or 2 bytes, with integer or fixed point encoding. 




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
  
	SPI for CAN MCP2515:
		#CS		PIN 10, Port PB2
		MOSI	PIN 11, Port PB3
		MISO	PIN 12, Port PB4
		SCK		PIN 13, Port PB5
		INT		PIN 3,  Port PD3 (INT1)

	Engine/car input:
		RPM				: PIN 7, port PD7
		SPEED			: PIN 6, port PD6
		MAF				: PIN 5, port PD5
		Throttle input	: PIN A7, ADC7, port AC7
		MAP input		: PIN A6, ADC6, port AC6
	
	Output:
		Solenoid output : PIN 4, port PD4
		Fuel pump prime : PIN 9, port PB1
	
	Debug output:
		Debug output    : PIN 19, port PC5
		Debug SPEED_OUT : PIN 16, port PC2
		Debug RPM_OUT 	: PIN 15, port PC1

	Other:
		Serial TX		:	PIN 1, Port PD1
		Serial RX		:	PIN 0, Port PD0
	
  Frequency measurement
  --------------------
	Frequency is measured by the pin change interrupt that measure the time between each
	rising edge for RPM and SPEED.
	
	The interrupt use the micros() arduino function to read the current time. This function has
	a resolution of 4 µs. For the worst case scenario of measuring a 400Hz RPM frequency, this lead to
	a minimum period of 250µs, i.e 62 timer ticks.
	
	For each parameter, a 4 values circular buffer is used to provide a cheap low pass filter.
	

	Main loop and CAN support
	-------------------------
	
	Main loop process acquires RPM, SPEED and throttle input 100 times per second.
	In the same times, interrupt are used to:
		* Detect changes on RPM, SPEED and MAF input to capture event date for later frequency computation
		in main loop.
		Quick reaction of this interrupt is critical for accurate frequency measurement.
		* Receive notification from CAN module that something happenned, either TX done or
		RX available. This interrupt just set a flag that will be processed by the main loop.
 
 */


#define VERSION_MAJOR	1
#define VERSION_MINOR	0

// I/O Pins 
#define RPM_IN	7
#define SPEED_IN 6
#define AIRFLOW_IN 5
//#define SOLENOID_OUT 4
//#define FUEL_PUMP_OUT 9
// Analog I/O
// #define MAP_IN A5    
// #define THROTTLE_IN A6  
// #define FUEL_PRESS_IN A7  

// Debug PIN
#define DEBUG_OUT 18
#define SPEED_OUT 16
#define RPM_OUT 15

// // Other constants
// #define LOOP_PERIOD 10	// loop period in ms


Config	gConfig;

/// Indicator set when the conf need to be saved
bool	gConfChanged = false;


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
//uint32_t	gLastRPMDate = 0;
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
//volatile uint32_t	gRPMPeriod = INT32_MAX;
// SPEED period in µs, 0 means no signal.
//volatile uint32_t	gSPEEDPeriod = INT32_MAX;

// Main loop variables
// uint32_t	gLastEvalCycle = 0;

bool debugState = false;
bool rpmOut = false;
bool speedOut = false;
#define TOGGLE_DEBUG     digitalWrite(DEBUG_OUT, debugState);    debugState = !debugState;


uint32_t micros()
{
	// Systick run @72Mhz, with a reload @72000 for 1ms interrupt
	uint32_t tickBefore;
	uint32_t tickAfter;
	uint32_t ms;

	// Make sure we read ms and tick counter outside of a reload interrupt
	do
	{
		tickBefore = SysTick->VAL;
		ms = HAL_GetTick();
		tickAfter = SysTick->VAL;
	} while (tickBefore < tickAfter);

	// Compute micro by combining ms integration and elapsed systick counter.
	uint32_t micro = ms * 1000 + (72000 - tickAfter) / (72000 / 1000);

	return micro;
}

// enum AnalogInput
// {
// 	MAP_IN,
// 	FUEL_PRESS_IN,
// 	OIL_TEMP_IN,
// 	WATER_TEMP_IN,
// 	OIL_PRES_IN,
// 	THROTTLE_IN,
// 	AFR_IN
// };

float analogRead(AnalogInput input)
{
	// TODO implement me
	return 0.0f;
}

enum DigitalOut
{
	SOLENOID_OUT,
	FUEL_PUMP_OUT
};

void digitalWrite(DigitalOut output, bool state)
{
	switch (output)
	{
	case SOLENOID_OUT:
		HAL_GPIO_WritePin(WG_PWM_GPIO_Port, WG_PWM_Pin, state ? GPIO_PIN_SET : GPIO_PIN_RESET);
		break;
	case FUEL_PUMP_OUT:
		HAL_GPIO_WritePin(FUEL_P_GPIO_Port, FUEL_P_Pin, state ? GPIO_PIN_SET : GPIO_PIN_RESET);
		break;
	}
}

struct VersionInfo
{
	char mVersionString[10] = "SNX_BC_vx";

	bool operator==(const VersionInfo &other) const
	{
		bool ok = true;
		for (size_t i = 0; i < sizeof(mVersionString) and ok; ++i)
		{
			ok = ok and (mVersionString[i] == other.mVersionString[i]);
		}
		return ok;
	}

	bool operator!=(const VersionInfo &other) const
	{
		return not (*this == other);
	}
};

extern ParamIndex gParamIndex;

FlashStore<> gFlashStore;

template <typename T>
void processParamStorage(uint16_t paramId, T& confItem, bool store)
{
	if (store)
	{
		auto bv = gParamIndex.getParam<T>(static_cast<ParamID>(paramId));
		if (bv.first)
		{
			confItem = bv.second;
		}
	}
	else
	{
		gParamIndex.setParam(paramId, confItem, true);
	}
}

void processConfigStorage(bool store)
{
	// single items
	{
		processParamStorage(ParamID::TIRE_CIRC, gConfig.tireCircum, store);
		processParamStorage(ParamID::SPEED_RATIO,  gConfig.speedFactor, store);
		processParamStorage(ParamID::BC_MAX_TTL_CAL,  gConfig.throttleScale, store);
	}

	// gear ratio
	{
		const auto desc = gParamIndex.getParamDef(ParamID::GEAR_RATIO);
		for (auto gear = 0; gear < desc->mArrayDesc->mArrayRow; ++gear)
		{
			processParamStorage(ParamID::GEAR_RATIO + gear, gConfig.gearRatios[gear], store);
		}
	}
	
	// PID params
	{
		processParamStorage(ParamID::BC_SERVO_P, gConfig.pidParam[0], store);
		processParamStorage(ParamID::BC_SERVO_I, gConfig.pidParam[1], store);
		processParamStorage(ParamID::BC_SERVO_D, gConfig.pidParam[2], store);
	}

	// Boost table
	{
		const auto desc = gParamIndex.getParamDef(ParamID::BC_BOOST_TABLE);
		for (auto gear = 0; gear < desc->mArrayDesc->mArrayRow; ++gear)
		{
			for (auto rpmCol = 0; rpmCol < desc->mArrayDesc->mArrayCol; ++rpmCol)
			{
				processParamStorage(
					ParamID::BC_BOOST_TABLE + rpmCol + gear * desc->mArrayDesc->mArrayCol, 
					gConfig.boostTable[gear][rpmCol],
					store);
			}
		}
	}

	// Throttle boost correction
	{
		const auto desc = gParamIndex.getParamDef(ParamID::BC_TTL_COR);
		for (auto tps = 0; tps < desc->mArrayDesc->mArrayRow; ++tps)
		{
			processParamStorage(ParamID::BC_TTL_COR + tps, gConfig.throttleBoostTable[tps], store);
		}
	}

	// Throttle move boost correction
	{
		const auto desc = gParamIndex.getParamDef(ParamID::BC_TTL_DER_COR);
		for (auto tpsDer = 0; tpsDer < desc->mArrayDesc->mArrayRow; ++tpsDer)
		{
			processParamStorage(
				ParamID::BC_TTL_DER_COR + tpsDer, 
				gConfig.throttleDerivBoostTable[tpsDer],
				store);
		}
	}

	// reference boost
	{
		processParamStorage(ParamID::BC_REF_BOOST, gConfig.boostReference, store);
	}

	// Duty cycle table
	{
		const auto desc = gParamIndex.getParamDef(ParamID::BC_DC_TABLE);
		for (auto presRow = 0; presRow < desc->mArrayDesc->mArrayRow; ++presRow)
		{
			for (auto loadCol = 0; loadCol < desc->mArrayDesc->mArrayCol; ++loadCol)
			{
				processParamStorage(
					ParamID::BC_DC_TABLE + loadCol + presRow * desc->mArrayDesc->mArrayCol, 
					gConfig.dutyCycleTable[presRow][loadCol],
					store);
			}
		}
	}

	// Fuel pump prime
	{
		processParamStorage(ParamID::BC_FUEL_PRIME, gConfig.fuelPumpPrime, store);
	}
}

void saveConfig()
{
	// copy param value into conf
	processConfigStorage(true);

	// And save it to flash
	gFlashStore.store(makeChunkLabel("BCTL"), &gConfig, sizeof(gConfig));
}

void loadConfig()
{
	// try to load the config object from flash
	Config conf;
	if (gFlashStore.load(makeChunkLabel("BCTL"), &conf, sizeof(conf)))
	{
		gConfig = conf;
	}
	else
	{
		// try with older configuration version...
	}

	// Fill value from config into params
	processConfigStorage(false);

	// // single items
	// {
	// 	gParamIndex.setParam(
	// 		ParamID::TIRE_CIRC, 
	// 		gConfig.tireCircum,
	// 		true);

	// 	gParamIndex.setParam(
	// 		ParamID::SPEED_RATIO, 
	// 		gConfig.speedFactor,
	// 		true);
	// }

	// // gear ratio
	// {
	// 	const auto desc = gParamIndex.getParamDef(ParamID::GEAR_RATIO);
	// 	for (auto gear = 0; gear < desc->mArrayDesc->mArrayRow; ++gear)
	// 	{
	// 		gParamIndex.setParam(
	// 			ParamID::GEAR_RATIO + gear, 
	// 			gConfig.gearRatios[gear],
	// 			true);
	// 	}
	// }
	
	// // PID params
	// {
	// 	gParamIndex.setParam(
	// 		ParamID::BC_SERVO_P, 
	// 		gConfig.pidParam[0],
	// 		true);
	// 	gParamIndex.setParam(
	// 		ParamID::BC_SERVO_I, 
	// 		gConfig.pidParam[1],
	// 		true);
	// 	gParamIndex.setParam(
	// 		ParamID::BC_SERVO_D, 
	// 		gConfig.pidParam[2],
	// 		true);
	// }

	// // Boost table
	// {
	// 	const auto desc = gParamIndex.getParamDef(ParamID::BC_BOOST_TABLE);
	// 	for (auto gear = 0; gear < desc->mArrayDesc->mArrayRow; ++gear)
	// 	{
	// 		for (auto rpmCol = 0; rpmCol < desc->mArrayDesc->mArrayCol; ++rpmCol)
	// 		{
	// 			gParamIndex.setParam(
	// 				ParamID::BC_BOOST_TABLE + rpmCol + gear * desc->mArrayDesc->mArrayCol, 
	// 				gConfig.boostTable[gear][rpmCol],
	// 				true);
	// 		}
	// 	}
	// }

	// // Throttle boost correction
	// {
	// 	const auto desc = gParamIndex.getParamDef(ParamID::BC_TTL_COR);
	// 	for (auto tps = 0; tps < desc->mArrayDesc->mArrayRow; ++tps)
	// 	{
	// 		gParamIndex.setParam(
	// 			ParamID::BC_TTL_COR + tps, 
	// 			gConfig.throttleBoostTable[tps],
	// 			true);
	// 	}
	// }

	// // Throttle move boost correction
	// {
	// 	const auto desc = gParamIndex.getParamDef(ParamID::BC_TTL_DER_COR);
	// 	for (auto tpsDer = 0; tpsDer < desc->mArrayDesc->mArrayRow; ++tpsDer)
	// 	{
	// 		gParamIndex.setParam(
	// 			ParamID::BC_TTL_DER_COR + tpsDer, 
	// 			gConfig.throttleDerivBoostTable[tpsDer],
	// 			true);
	// 	}
	// }

	// // reference boost
	// {
	// 	gParamIndex.setParam(
	// 		ParamID::BC_REF_BOOST, 
	// 		gConfig.boostReference,
	// 		true);
	// }

	// // Duty cycle table
	// {
	// 	const auto desc = gParamIndex.getParamDef(ParamID::BC_DC_TABLE);
	// 	for (auto presRow = 0; presRow < desc->mArrayDesc->mArrayRow; ++presRow)
	// 	{
	// 		for (auto loadCol = 0; loadCol < desc->mArrayDesc->mArrayCol; ++loadCol)
	// 		{
	// 			gParamIndex.setParam(
	// 				ParamID::BC_DC_TABLE + loadCol + presRow * desc->mArrayDesc->mArrayCol, 
	// 				gConfig.dutyCycleTable[presRow][loadCol],
	// 				true);
	// 		}
	// 	}
	// }

	// // Fuel pump prime
	// {
	// 	gParamIndex.setParam(ParamID::BC_FUEL_PRIME, gConfig.fuelPumpPrime, true);
	// }
}

void BoostController::setup()
{
	gAnalogConverter.setup();

	// Init serial port @115200 baud, 8N1
	// Serial.begin( 57600);

	// Serial.write("init\n");
	// Serial.write("load config\n");

	// load conf
	loadConfig();

	// Serial.write("set pinMode\n");

	// Configure IO pins
	// pinMode(RPM_IN, INPUT_PULLUP);
	// pinMode(SPEED_IN, INPUT_PULLUP);
	// pinMode(AIRFLOW_IN, INPUT_PULLUP);
	// pinMode(SOLENOID_OUT, OUTPUT);
	// pinMode(FUEL_PUMP_OUT, OUTPUT);
	// pinMode(DEBUG_OUT, OUTPUT);

	// Debug pin
	// pinMode(SPEED_OUT, OUTPUT);
	// pinMode(RPM_OUT, OUTPUT);

	// Configure analog input
	//TODO

	// Serial.write("pci setup\n");

	// activate pin change interrupt
	// pciSetup(RPM_IN);
	// pciSetup(SPEED_IN);
	// pciSetup(AIRFLOW_IN);
}

void BoostController::computeGear()
{
	// compute gear 
	// Eval is done by computing the current speed to RPM ratio and founding the nearest 
	// corresponding gear ratio
	float engineHz = mMeasures.RPM / 60.0f;
	float speedMs = (mMeasures.SPEED / 3600.0f) * 1000.0f;
	float revPerMeter = engineHz / speedMs;
	float gearRatio = revPerMeter
						* gConfig.tireCircum;
	
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
	
	mMeasures.GEAR = gear;
	gParamIndex.setParam(ParamID::GEAR, mMeasures.GEAR, false);
}

void BoostController::manageFuelPrime()
{
	if (not mPrimingFuel)
		return;

	auto now = HAL_GetTick();

	// NB: output is a low side switch with a boost transistor that invert the logic of the 
	// output.
	if (now / 1000 < gConfig.fuelPumpPrime)
	{
		// fuel priming period, force fuel pump
		digitalWrite(FUEL_PUMP_OUT, false);
	}
	else
	{
		// prime finished, let ECM control the fuel pump 
		digitalWrite(FUEL_PUMP_OUT, true);
		mPrimingFuel = false;
	}
}

template<typename T, typename U, typename V>
void clamp(T& value, const U min, const V max)
{
	if (value < min)
	{
		value = min;
	}
	else if (value > max)
	{
		value = max;
	}
}


// 0..5V to Water temp (°C)
SensorCorrectionLUT waterTempLUT[] = 
{
	{  2.5f,  20.0f},
	{  2.6f,  40.0f},
	{  2.8f,  60.0f},
	{  3.2f,  80.0f},
	{  4.0f, 100.0f},
	{  5.0f, 150.0f},
};

float RPMHz = 0.0;
float speedHz = 0.0;
float MAFHz = 0.0;

void BoostController::evalCycle()
{
//	static uint32_t previousLoopDate = 0;

	gAnalogConverter.loop();

	// MAP_IN,
	// FUEL_PRESS_IN,
	// OIL_TEMP_IN,
	// WATER_TEMP_IN,
	// OIL_PRES_IN,
	// THROTTLE_IN,
	// AFR_IN

	// Hz reading on timer 2, APB1, 72MHz base freq
	// 65536 @ 72MHz => 1098 overflow/s, way too more!
	// * 400Hz for RPM
	// * 220Hz for speed
	// * 2800Hz for MAF
	//		357.14µs
	//	 2795Hz
	//		357.78µs
	//		Require 100ns rez => 10Mz => 152 overflow/s
	//		429s overflow max on 32 bits before 32 bits overflow
	//
	// Real value:
	//	72MHz clock, div 7 => 10.285MHz timer clock => 97ns rez, 6.37ms per period
	//
	// Low frequency limits
	// * 1Hz for RPM
	//		10.2 tick per pulse
	//		156 periods
	//		
	// * 5Hz for speed
	//		32 periods
	// * 10Hz for MAF
	//		16 periods

	const constexpr float IC_TICK_DUR = 1.0 / (72'000'000 / 7.0);

	// float RPMHz = 0.0;
	// float speedHz = 0.0;
	// float MAFHz = 0.0;

	// capture read
	__disable_irq();
	InputCapture inputCapture = gInputCapture.mValues;
	__enable_irq();

	if (inputCapture.mRPM != 0)
	{
		RPMHz = 1 / (inputCapture.mRPM * IC_TICK_DUR);
		if (RPMHz < 0)
		{
			RPMHz = 0;
		}
	}
	else
	{
		RPMHz = 0;
	}
	if (inputCapture.mSpeed != 0)
	{
		speedHz = 1 / (inputCapture.mSpeed * IC_TICK_DUR);
	}
	else
	{
		speedHz = 0;
	}
	if (inputCapture.mMAF != 0)
	{
		MAFHz = 1 / (inputCapture.mMAF * IC_TICK_DUR);
	}
	else
	{
		MAFHz = 0;
	}

	// read input and process inputs
	//------------------------------
	// uint8_t oldSREG = SREG;
	// cli();
//	uint32_t RPMPeriod = 1'000'000.0f / RPMHz;
	uint32_t SPEEDPeriod = 1.0f;
//	uint32_t lastRPMDate = gLastRPMDate;
	uint32_t lastSPEEDDate = gLastSPEEDDate;
	int8_t lastAirflowAccum = gAirFlowAccum;
	// reset air flow accum for next loop
	gAirFlowAccum = 0;
	// SREG = oldSREG;

	uint32_t now = micros();

	// Retrieve analog reads
	// MAP. 1V per bar, 0V @ 0bar, 1V@1 bar (atmospheric pressure)...
	mMeasures.MAP = gAnalogConverter.getAnalogInput(MAP_IN);
	// Filter MAP
	mMeasures.MAP = mMapLowPassFilter.input(mMeasures.MAP);
	gParamIndex.setParam(ParamID::MAP, mMeasures.MAP, false);

	// Fuel pressure, relative. 2.58Bar per V, 0.5V..4.5V => 0..150PSI (10.38Bar), 0.5V @ 0bar
	mMeasures.FUEL_PRESS = gAnalogConverter.getAnalogInput(FUEL_PRESS_IN);
	gParamIndex.setParam(ParamID::FUEL_PRES, mMeasures.FUEL_PRESS, false);

	// Oil pressure, relative. 2.58Bar per V, 0.0V..4.5V => 0..150PSI (10.38Bar), 0.5V @ 0bar
	mMeasures.OIL_PRESS = gAnalogConverter.getAnalogInput(OIL_PRESS_IN);
	gParamIndex.setParam(ParamID::OIL_PRES, mMeasures.OIL_PRESS, false);

	// Oil temperature.
	mMeasures.OIL_TEMP = gAnalogConverter.getAnalogInput(OIL_TEMP_IN);
	gParamIndex.setParam(ParamID::OIL_TEMP, mMeasures.OIL_TEMP, false);

	// Coolant temperature.
	mMeasures.CLT_TEMP = gAnalogConverter.getAnalogInput(WATER_TEMP_IN);
	gParamIndex.setParam(ParamID::WATER_TEMP, mMeasures.CLT_TEMP, false);

	// Lambda.
	mMeasures.LAMBDA = gAnalogConverter.getAnalogInput(AFR_IN);
	gParamIndex.setParam(ParamID::LAMBDA, mMeasures.LAMBDA, false);

	// Throttle analog read. 0..100% => 0..5V
	mMeasures.THROTTLE = gAnalogConverter.getAnalogInput(THROTTLE_IN);
	mMeasures.THROTTLE = mThrottleLowpassFilter.input(mMeasures.THROTTLE);
	// Scale throttle value with max scale correction
	if (uint8_t(mMeasures.THROTTLE) > gConfig.throttleScale)
	{
		// We found a greater value, update max scale
		gConfig.throttleScale = uint8_t(round(mMeasures.THROTTLE));
		gParamIndex.setParam(BC_MAX_TTL_CAL, gConfig.throttleScale, true);
		// Make sure to save it
		gConfChanged = true;
	}
	mMeasures.THROTTLE = mMeasures.THROTTLE / float(gConfig.throttleScale) * 100.0f;
	gParamIndex.setParam(ParamID::THROTTLE, mMeasures.THROTTLE, false);
	// Get high pass filter for throttle derivative
	mMeasures.THROTTLE_DERIV = mThrottleDerHighpassFilter.input(mMeasures.THROTTLE)/* * 1000.0f*/;
	gParamIndex.setParam(ParamID::TTL_POS_DER, mMeasures.THROTTLE_DERIV, false);

//! 4ms
//return;

	// Detect 0 RPM : last pulse > 100RPM period : 100RPM=>1.6666Hz, *3=>5Hz, 1/5Hz=0.2s=>200'000µs
	// if (RPMPeriod > 0 and (now - lastRPMDate) > 200000)
	// {
	// 	// 0 stand for 0 RPM
	// 	RPMPeriod = 0;
	// }
	// Detect 0 Speed : last pulse > 1km/h period : 1Kmh/0.73 = 1.36s=1'360'000µs
	// if (SPEEDPeriod > 0 and (now - lastSPEEDDate) > 1'360'000)
	// {
	// 	// 0 stand for 0 speed
	// 	SPEEDPeriod = 0;
	// }

	// Convert µs period into Hz
	// RPM need an additional ratio of 3 to account for the number of cylinder fired each revolution.
	if (RPMHz > 0)
	{
		mMeasures.RPM = RPMHz * (60.0f / 3.0f);
	}
	else
	{
		mMeasures.RPM = 0.0f;
	}
	gParamIndex.setParam(ParamID::RPM, mMeasures.RPM, false);
	if (speedHz > 0)
	{
		mMeasures.SPEED = speedHz * 2 * gConfig.speedFactor;
	}
	else
	{
		mMeasures.SPEED = 0.0;
	}
	gParamIndex.setParam(ParamID::SPEED, mMeasures.SPEED, false);

	// Compute airflow HZ
	if (MAFHz > 0.0f)
	{
		// Filter HZ value
		mMeasures.AIR_FLOW = mAirflowLPF.input(MAFHz);
	}
	else
	{
		mMeasures.AIR_FLOW = 0.0;
	}
	gParamIndex.setParam(ParamID::AIR_FLOW, mMeasures.AIR_FLOW, false);

//! 5.9ms
	computeGear();
	manageFuelPrime();
// !6ms


	// Compute engine load
	// We don't have MAF or VE info, so the load is a simple approximation based on MAP :
	// 	0 bar MAP => 0% load,
	//	2 bar MAP => 100% load (2 bar MAP means 1 bar turbo pressure)
	mMeasures.LOAD = constrain((mMeasures.MAP * 0.5f) * 100.0f, 0.0f, 100.0f);
	gParamIndex.setParam(ParamID::LOAD, mMeasures.LOAD, false);

//! 7ms

	// compute solenoid DC 
	//--------------------
	float targetBoost = gConfig.boostReference;
	// 1st, choose the target boost in the gear table
	//------------------------------------------------
	targetBoost *= interp2D(gParamIndex, ParamID::BC_BOOST_TABLE, mMeasures.GEAR, mMeasures.RPM) * 0.01f;

//! 7%


	// Apply throttle correction
	// -------------------------
	// throttle position correction
	targetBoost = targetBoost * interp1D(gParamIndex, ParamID::BC_TTL_COR, mMeasures.THROTTLE) * 0.01f;

	// Throttle pos variation correction
	targetBoost = targetBoost * interp1D(gParamIndex, ParamID::BC_TTL_DER_COR, mMeasures.THROTTLE_DERIV) * 0.01f;

	// Convert boost value into MAP value, just add the atmo pressure ;)
	targetBoost += 1.0f;

	// And finally get target boost filtered
	mMeasures.TARGET_BOOST = mTargetLowpassFilter.input(targetBoost);
	gParamIndex.setParam(ParamID::BC_TGT_BOOST, mMeasures.TARGET_BOOST, false);

	// 2nd, apply PID filter to compute boost error
	float error = mMeasures.TARGET_BOOST - (std::max(0.0f, mMeasures.MAP));
	float P = error * gConfig.pidParam[0];
	// Integral windup check
	if (mMeasures.RPM > 2000 and fabs(error) < 0.4f)
	{
		mErrorInteg += error * gConfig.pidParam[1] * (LOOP_PERIOD * 0.001f);
		mErrorInteg = constrain(mErrorInteg, -0.3f, 0.3f);
	}
	else
	{
		// too much error, windup integral
		mErrorInteg = 0.0f;
	}
	mErrorDeriv = ((error - mLastError) * gConfig.pidParam[2]) / (LOOP_PERIOD * 0.001f);
	mErrorDeriv = mBoostHighpassFilter.input(mErrorDeriv);
	mErrorDeriv = constrain(mErrorDeriv, -0.5f, 0.5f);
	mLastMAP = mMeasures.MAP;
	mLastError = error;
	
	mMeasures.TARGET_OUTPUT = P + mErrorInteg + mErrorDeriv;
	mMeasures.TARGET_OUTPUT = constrain(mMeasures.TARGET_OUTPUT, 0.0f, 2.0f);
	gParamIndex.setParam(ParamID::BC_TGT_OUTPUT, mMeasures.TARGET_OUTPUT, false);
	
//! 10ms

	// Convert target output to solenoid DC
	// We need a conversion constant to map a boost value to a solenoid DC
	// target output is in absolute pressure, convert it to relative pressure.
	// DC is updated only once per sol cycle
	float relativeTarget = mMeasures.TARGET_OUTPUT; // - 1.0f;
	float dc = interp2D(gParamIndex, ParamID::BC_DC_TABLE, relativeTarget, mMeasures.LOAD);
	mMeasures.SOL_DC = dc;

	// At last, prevent the solenoid to activate if the speed if low
	if (mMeasures.SPEED < 8.0f)
	{
		mMeasures.SOL_DC = 0.0f;
	}

	gParamIndex.setParam(ParamID::BC_SOL_DC, mMeasures.SOL_DC, false);

	// Take simulation into account
	float effectiveSolDc = mMeasures.SOL_DC;
	if (gSimulation.FORCE_WG != 0)
	{
		effectiveSolDc = gSimulation.FORCE_WG;
	}
  
	// produce output value
	// --------------------
	TIM1->CCR1 = std::min(uint32_t(65534), 65535 - uint32_t(65534 * effectiveSolDc / 100 ));

//	previousLoopDate = now;
}

BoostController gBoostController;

class SerialMgr : public CommInterface
{
	CommProviderCallback* mCommCallback = nullptr;

	UART_HandleTypeDef*	mHUart;

	// 256 bytes buffer (2^8)
	static const constexpr size_t BUFFER_POWER = 8;
	static const constexpr size_t BUFFER_SIZE = 1 << BUFFER_POWER;
	static const constexpr size_t BUFFER_SIZE_MASK = BUFFER_SIZE-1;
	// 256 bytes input ring buffer
	uint8_t mInputBuffer[BUFFER_SIZE];
	// Input ring buffer head (AKA read cursor)
	uint8_t mInputHead = 0;
	// Input ring buffer tail (AKA write cursor)
	uint8_t mInputTail = 0;
	// Number of bytes awaited for the current receiving message.
	uint8_t mExpectInput = MINIMUM_MSG_SIZE;

	// header + checksum
	static const constexpr size_t MINIMUM_MSG_SIZE = 4;

	enum InputState : uint8_t
	{
		WAIT_HEADER,
		WAIT_DATA
	};

	enum Command : uint8_t
	{
		DATA_RQ = 0b00,
		DATA_TX = 0b01,

		INVALID_HEADER = 0xff
	};

	enum AsciiCodes : uint8_t
	{
		SOT = 0x02
	};

	struct Header
	{
		Command  mCommand;
		uint16_t mId;
		uint8_t  mSize;
	};

	InputState mInputState = WAIT_HEADER;

	Header readHeader()
	{
		Header header;
		// parse the header
		if (mInputBuffer[mInputHead] != SOT)
		{
			header.mCommand = INVALID_HEADER;
		}
		else
		{
			uint8_t ctrl = mInputBuffer[(mInputHead + 1) & BUFFER_SIZE_MASK];
			uint8_t idLow = mInputBuffer[(mInputHead + 2) & BUFFER_SIZE_MASK];

			header.mCommand = static_cast<Command>((ctrl >> 3) & 0b11);
			header.mSize = 1 + ((ctrl >> 5) & 0b111);
			header.mId = ((uint16_t(ctrl) & 0b111) << 8) + idLow;
		}

		return header;
	}

	void purgeInputBuffer()
	{
		// Remove at least one byte if possible
		if (mInputHead != mInputTail)
		{
			mInputHead = (mInputHead + 1) & BUFFER_SIZE_MASK;
		}
		// Then purge the rest, but stop on SOT
		while (mInputHead != mInputTail 
			and mInputBuffer[mInputHead] != SOT)
		{
			mInputHead = (mInputHead + 1) & BUFFER_SIZE_MASK;
		}

		// Once purged, we restart with a header
		mExpectInput = MINIMUM_MSG_SIZE;

		// And reset state
		mInputState = WAIT_HEADER;
	}

	void consumeInput(size_t nbByte)
	{
		while (nbByte > 0 and mInputTail != mInputHead)
		{
			mInputHead = (mInputHead + 1) & BUFFER_SIZE_MASK;
		}
	}

	bool verifyChecksum(const uint8_t startIdx, const uint8_t endIdx) const
	{
		uint8_t idx = startIdx;
		uint8_t sum = 0;

		while (idx != endIdx)
		{
			sum += mInputBuffer[idx];
			idx = (idx + 1) & BUFFER_SIZE_MASK;
		};

		// sum, including checksum byte shall be zero
		return sum == 0;
	}

	// Request the communication provider to send a data transmission.
	void commSendCb(uint16_t paramId, const uint8_t* data, uint8_t size) override final
	{
		uint8_t sendBuffer[MINIMUM_MSG_SIZE + 8];

		// init checksum
		uint8_t& sum = sendBuffer[3 + size];
		sum = 0;

		// Encode header:
		// Start Of Text
		sendBuffer[0] = SOT;
		sum += sendBuffer[0];
		// Command
		sendBuffer[1] = (DATA_TX & 0b11) << 3;
		// Data size
		sendBuffer[1] |= ((size-1) & 0b111) << 5;
		// Param ID higher bits
		sendBuffer[1] |= (paramId >> 8) & 0b111;
		sum += sendBuffer[1];
		// Param ID lower bits
		sendBuffer[2] = paramId & 0xff;
		sum += sendBuffer[2];
		// data bytes
		for (size_t i=0; i<size; ++i)
		{
			sendBuffer[3+i] = data[i];
			sum += sendBuffer[3+i];
		}
		sum = 256 - sum;

		HAL_UART_Transmit(mHUart, sendBuffer, size + 4, 10);
	}

	// Request the communication provider to send a data request.
	void commSendRqCb(uint16_t paramId, uint8_t size) override final
	{
		uint8_t sendBuffer[MINIMUM_MSG_SIZE + 8];

		// init checksum
		uint8_t& sum = sendBuffer[3];
		sum = 0;

		// Encode header:
		// Start Of Text
		sendBuffer[0] = SOT;
		sum += sendBuffer[0];
		// Command
		sendBuffer[1] = (DATA_RQ & 0b11) << 3;
		// Data size
		sendBuffer[1] |= ((size-1) & 0b111) << 5;
		// Param ID higher bits
		sendBuffer[1] |= (paramId >> 8) & 0b111;
		sum += sendBuffer[1];
		// Param ID lower bits
		sendBuffer[2] = paramId & 0xff;
		sum += sendBuffer[2];

		// finalize checksum
		sum = 256 - sum;

		// And transmit data over serial line
		HAL_UART_Transmit(mHUart, sendBuffer, 4, 10);
	}

public:
	SerialMgr(UART_HandleTypeDef* hUart)
	:	mHUart(hUart)
	{
	}

	void setup()
	{
		// Start DMA RX
		HAL_StatusTypeDef ret = HAL_UART_Receive_DMA(mHUart, mInputBuffer, BUFFER_SIZE);
	}

	void setCommCallback(CommProviderCallback* commCallback)
	{
		mCommCallback = commCallback;
	}

	void loop()
	{
		// polling DMA state
		
		mInputTail = BUFFER_SIZE - LL_DMA_GetDataLength(mHUart->hdmarx->DmaBaseAddress, LL_DMA_CHANNEL_5);
//		mInputTail = BUFFER_SIZE - LL_DMA_GetDataLength(mHUart->hdmarx->DmaBaseAddress, mHUart->hdmarx->ChannelIndex);


		// size_t maxReadPerLoop = 100;
		// while (		maxReadPerLoop > 0 
		// 		and HAL_UART_Receive(mHUart, mInputBuffer + mInputTail, 1, 0) == HAL_OK)
		// {
		// 	--maxReadPerLoop;
		// 	// advance write cursor
		// 	mInputTail = (mInputTail + 1) & BUFFER_SIZE_MASK;

		bool again;
		do
		{
			again = false;
			size_t available = (mInputTail - mInputHead) & BUFFER_SIZE_MASK;

			// Do we have the awaited data ?
			if (available >= MINIMUM_MSG_SIZE)
			{
				Header header = readHeader();
				// process input
				if (mInputState == WAIT_HEADER)
				{
					if (header.mCommand == DATA_RQ)
					{
						if (verifyChecksum(mInputHead, (mInputHead + MINIMUM_MSG_SIZE) & BUFFER_SIZE_MASK))
						{
							// we can process the command
							mCommCallback->requestFromComm(this, header.mId, header.mSize);
							consumeInput(MINIMUM_MSG_SIZE);
						}
						else
						{
							purgeInputBuffer();
						}
					}
					else if (header.mCommand == DATA_TX)
					{
						// We need to wait for more bytes
						mExpectInput = MINIMUM_MSG_SIZE + header.mSize;
						mInputState = WAIT_DATA;
					}
					else
					{
						purgeInputBuffer();
					}

					again = true;
				}
				else if (mInputState == WAIT_DATA and available >= mExpectInput)
				{
					if (header.mCommand == DATA_TX)
					{
						if (verifyChecksum(mInputHead, (mInputHead + MINIMUM_MSG_SIZE + header.mSize) & BUFFER_SIZE_MASK))
						{
							// copy the data from the buffer!
							uint8_t data[8];
							for (size_t i=0; i<header.mSize; ++i)
							{
								data[i] = mInputBuffer[(mInputHead + 3 + i) % BUFFER_SIZE_MASK];
							}
							// we can process the command
							mCommCallback->setFromComm(this, header.mId, data, header.mSize);
							consumeInput(MINIMUM_MSG_SIZE + header.mSize);
							mInputState = WAIT_HEADER;
						}
						else
						{
							purgeInputBuffer();
						}
					}
					else
					{
						purgeInputBuffer();
					}

					again = true;
				}
				// else
				// {
				// 	purgeInputBuffer();
				// }
			}
		} while (again);
	}
};

SerialMgr gSerialMgr(&huart1);

/// Dual comm serial.CAN handler.
/// Bridge the CAN bus, the serial line and local node in all direction:
/// - CAN => serial and local
/// - Serial => CAN and local
/// - Local => CAN and serial
class MultiCommProvider : public CommInterface, public CommProviderCallback
{
	CommProviderCallback* mCommCallback = nullptr;

	std::vector<CommInterface*> mCommInterfaces;

	// Request the communication provider to send a data transmission.
	void commSendCb(uint16_t paramId, const uint8_t* data, uint8_t size) override final
	{
		for (const auto& comm : mCommInterfaces)
		{
			comm->commSendCb(paramId, data, size);
		}
	}

	// Request the communication provider to send a data request.
	void commSendRqCb(uint16_t paramId, uint8_t size) override final
	{
		for (const auto& comm : mCommInterfaces)
		{
			comm->commSendRqCb(paramId, size);
		}
	}

	void setFromComm(CommInterface* sender, uint16_t paramId, const uint8_t* data, uint8_t size) override final
	{
		// send to local node
		mCommCallback->setFromComm(sender, paramId, data, size);

		// send to other comm provider. But how do we know which ?
		for (auto& commProv : mCommInterfaces)
		{
			if (sender != commProv)
			{
				commProv->commSendCb(paramId, data, size);
			}
		}
	}

	void requestFromComm(CommInterface* sender, uint16_t paramId, uint8_t size) override final
	{
		// send to local node
		mCommCallback->requestFromComm(sender, paramId, size);

		// send to other comm provider. But how do we know which ?
		for (auto& commProv : mCommInterfaces)
		{
			if (sender != commProv)
			{
				commProv->commSendRqCb(paramId, size);
			}
		}
	}

public:

	MultiCommProvider()
	:	mCommCallback()
	{
		mCommInterfaces.reserve(2);
	}

	void setCommCallback(CommProviderCallback* commCallback)
	{
		mCommCallback = commCallback;
	}

	void addCommInterface(CommInterface* comm)
	{
		mCommInterfaces.push_back(comm);
	}
};

MultiCommProvider gMultiComm;

ParamIndex gParamIndex(DeviceID::BOOST_CTRL, &gMultiComm, &gBoostController);

CANCommProvider gCanCommProvider(&hcan, &gParamIndex);

// Automatic parameter broadcast
struct ParamBroadcast
{
	// The parameter to broadcast
	ParamID	mParamId;
	// Broadcast period (ms)
	uint32_t mPeriod;
};

const constexpr ParamBroadcast gParamBroadcastTable[] =
{
	// ID					period(ms)
	{ParamID::MAP,			   10},
	{ParamID::THROTTLE,		   50},
	{ParamID::BC_SOL_DC,	   50},
	{ParamID::GEAR,			  100},
	{ParamID::LOAD,			   50},
	{ParamID::BC_CPU_LOAD,	   50},
	{ParamID::BC_TGT_BOOST,	   50},
	{ParamID::BC_TGT_OUTPUT,   50},
	{ParamID::OIL_PRES,		   50},
	{ParamID::FUEL_PRES,	   50},
	{ParamID::LAMBDA,		  100},
	{ParamID::RPM,			   20},
	{ParamID::SPEED,		   20},
	{ParamID::TTL_POS_DER,	  100},
	{ParamID::AIR_FLOW,		   20},
	{ParamID::WATER_TEMP,	 1000},
	{ParamID::OIL_TEMP,		 1000},
};

class Broadcaster
{
	ParamIndex&	mParamIndex;

	uint32_t	mLastTx[sizeof_array(gParamBroadcastTable)] = {0};
public:
	Broadcaster(ParamIndex& paramIndex)
	:	mParamIndex(paramIndex)
	{}

	void loop()
	{
		uint32_t now = HAL_GetTick();

		for (size_t i = 0; i < sizeof_array(gParamBroadcastTable); ++i)
		{
			const ParamBroadcast& paramBroadcast = gParamBroadcastTable[i];
			if (now - mLastTx[i] > paramBroadcast.mPeriod)
			{
				// It's time to transmit this param!

				mParamIndex.triggerTransmission(paramBroadcast.mParamId);

				// keep track of last TX
				mLastTx[i] = now;
			}
		}
	}
};

Broadcaster gBroadcaster(gParamIndex);

void BoostController::onParamChange(uint16_t paramId)
{
	auto desc = gParamIndex.getParamDef(paramId);
	if (	desc != nullptr
		and desc->mIsConf
		and desc->mDeviceID == DeviceID::BOOST_CTRL)
	{
		// Need to save the conf
		gConfChanged = true;
	}
}

void BoostController::loop()
{
	// 100 eval cycles per second
	uint32_t now = micros();
	
	// manage serial port
	gSerialMgr.loop();

	// manage CAN interface
	gCanCommProvider.loop();

	// Manage parameter broadcast
	gBroadcaster.loop();
	
	if (now - mLastSaveCheck > 1000000)
	{
		// every second, look for saving the conf
		if (gConfChanged)
		{
			saveConfig();
			gConfChanged = false;
		}
		mLastSaveCheck = now;
	}

	if (now - mLastEvalCycle > LOOP_PERIOD * 1000)
	{
		evalCycle();
		mLastEvalCycle = now;
		
		uint32_t after = micros();
		
		// compute CPU load
		// Maximum load is kept for 500ms
		static uint32_t lastLoadReset = 0;
		static float maxLoad = 0.0f;
		static float prevMaxLoad = 0.0f;
		if (now - lastLoadReset > 500000)
		{
			prevMaxLoad = maxLoad;
			maxLoad = 0.0f;
			lastLoadReset = now;
		}

		mMeasures.CHIP_LOAD = (after - now) * 100 / (LOOP_PERIOD * 1000.0f);
		if (mMeasures.CHIP_LOAD > maxLoad)
		{
			maxLoad = mMeasures.CHIP_LOAD;
			gParamIndex.setParam(
				ParamID::BC_CPU_LOAD,
				std::max(prevMaxLoad, maxLoad),
				false);
		}
	}
}

extern "C"
{

void setup()
{
	gBoostController.setup();

	gSerialMgr.setCommCallback(&gMultiComm);
	gMultiComm.setCommCallback(&gParamIndex);
	gMultiComm.addCommInterface(&gCanCommProvider);
	gMultiComm.addCommInterface(&gSerialMgr);

	HAL_CAN_Start(&hcan);
	HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING|CAN_IT_RX_FIFO1_MSG_PENDING);

	CAN_FilterTypeDef filter;
	filter.FilterActivation = ENABLE;
	filter.FilterBank = 0;
	filter.FilterMode = CAN_FILTERMODE_IDMASK;
	filter.FilterFIFOAssignment = CAN_FILTER_FIFO0;
	filter.FilterScale = CAN_FILTERSCALE_16BIT;
	filter.FilterIdHigh = 0;
	filter.FilterIdLow = 0;
	filter.FilterMaskIdHigh = 0;
	filter.FilterMaskIdLow = 0;
	HAL_CAN_ConfigFilter(&hcan, &filter);

	gSerialMgr.setup();
}

void loop()
{
	HAL_CAN_ActivateNotification(&hcan,0);
	gBoostController.loop();
	HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING|CAN_IT_RX_FIFO1_MSG_PENDING);
}

void my_HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan);
void my_HAL_CAN_RxFifo0FullCallback(CAN_HandleTypeDef *hcan);
void my_HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan);

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	my_HAL_CAN_RxFifo0MsgPendingCallback(hcan);
	// const auto& map = CANCommProvider::getMap();
	// auto it = map.find(hcan);
	// if (it != map.end())
	// {
	// 	it->second->onCanReceive();
	// }
}

void HAL_CAN_RxFifo0FullCallback(CAN_HandleTypeDef *hcan)
{
	my_HAL_CAN_RxFifo0FullCallback(hcan);
}

void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	my_HAL_CAN_RxFifo1MsgPendingCallback(hcan);
}

} // Extern "C"
