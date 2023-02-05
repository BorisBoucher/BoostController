
#pragma once

#include <FilterDerivative.h>
#include <FilterOnePole.h>
#include <Filters.h>
#include <FilterTwoPole.h>


uint32_t micros();

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
	// Boost controller chip load (%)
	float	CHIP_LOAD = 0.0f;
	// Oil pressure
	float	OIL_PRESS = 0.0f;
	// Oil temperature
	float	OIL_TEMP = 0.0f;
	// Coolant temperature
	float	CLT_TEMP = 0.0f;
	// Lambda value
	float	LAMBDA = 0.0f;
};


class BoostController : public ParamIndex::ChangeCallbackInterface
{
	// Other constants
	// loop period in ms
	static const constexpr uint32_t LOOP_PERIOD = 25;

	FilterOnePole mMapLowPassFilter =          {micros, LOWPASS,  5.0f};
	FilterOnePole mBoostHighpassFilter =       {micros, HIGHPASS, 0.1f}; 
	FilterOnePole mTargetLowpassFilter =       {micros, LOWPASS,  5.0f};
	FilterOnePole mThrottleLowpassFilter =     {micros, LOWPASS,  5.0f};
	FilterOnePole mThrottleDerHighpassFilter = {micros, HIGHPASS, 1.0f};
	// Low pass filter for Air flow data. Should correct the measure aliasing
	// of counting MAG HZ in short period of 10ms (which may lead to very quantified result
	// at low HZ).
//	FilterOnePole mAirflowLPF =                {micros, LOWPASS,  5.0f}; 
	FilterOnePole mAirflowLPF =                {micros, LOWPASS,  1.0f}; 

	Measurement	mMeasures;

	// PID state variables
	float mErrorInteg = 0.0f;
	float mErrorDeriv = 0.0f;
	float mLastMAP = 0.0f;
	float mLastError = 0.0f;


	uint32_t mLastEvalCycle = 0;
	uint32_t mLastSaveCheck = 0;

	// Fuel pump priming on startup.
	bool mPrimingFuel = true;

	void computeGear();
	void manageFuelPrime();

	void evalCycle();

	// A parameter has changed (new value received from comm provider).
	void onParamChange(uint16_t paramId) override final;

public:
	void setup();

	void loop();
};