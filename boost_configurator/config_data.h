#pragma once

#include <cstdint>

enum BC_ADDR
{
	MAP =				0x000,
	THROTTLE =			0x001,
	WGDC =				0x002,
	GEAR =				0x003,
	ENG_LOAD =			0x004,
	CPU_LOAD =			0x005,
	TARGET_BOOST =		0x006,
	TARGET_OUTPUT =		0x007,

	DC_TABLE_1 =		0x008 + 0 * 4,	// 4 values
	DC_TABLE_2 =		0x008 + 1 * 4,	// 4 values
	DC_TABLE_3 =		0x008 + 2 * 4,	// 4 values
	DC_TABLE_4 =		0x008 + 3 * 4,	// 4 values

	BOOST_GEAR_1 =		0x020 + 0 * 7,	// 7 values
	BOOST_GEAR_2 =		0x020 + 1 * 7,	// 7 values
	BOOST_GEAR_3 =		0x020 + 2 * 7,	// 7 values
	BOOST_GEAR_4 =		0x020 + 3 * 7,	// 7 values
	BOOST_GEAR_5 =		0x020 + 4 * 7,	// 7 values
	BOOST_GEAR_6 =		0x020 + 5 * 7,	// 7 values

	THROTTLE_ADJ =		0x04a,		// 6 values
	THROTTLE_DER_ADJ =	0x050,		// 7 values

	WGDC_TEST =			0x057,

	VERSION_MAJOR =		0x058,
	VERSION_MINOR =		0x059,

	TYRE_CIRC =			0x800,
	GEAR_RATIO_1 =		0x801,		// 6 values
	GEAR_RATIO_2 =		0x802,		// 6 values
	GEAR_RATIO_3 =		0x803,		// 6 values
	GEAR_RATIO_4 =		0x804,		// 6 values
	GEAR_RATIO_5 =		0x805,		// 6 values
	GEAR_RATIO_6 =		0x806,		// 6 values
	SPEED_RATIO =		0x807,
	REF_BOOST =			0x808,

	PID_P =				0x809,
	PID_I =				0x80a,
	PID_D =				0x80b,

	RPM =				0x80c,
	SPEED =				0x80d,
	THROTTLE_DERIV =	0x80e,
	AIR_FLOW =			0x80f
};

struct ConfigData
{
	uint8_t   mVersionMajor;
	uint8_t   mVersionMinor;
	float   mTyreSize;
    float   mGearRatio[6];
    float   mSpeedSensorRatio;
    float   mReferenceBoost;

    float   mPidP;
    float   mPidI;
    float   mPidD;

	uint8_t	mWGDCTable[6][4];
    uint8_t  mBoostTable[6][7];
	uint8_t  mThrottleTable[6];
	uint8_t  mThrottleDerivTable[7];
};

struct Measures
{
    float	mRPM = 0.0f;
    // Speed in km/h
    float	mSpeed = 0.0f;
    // MAP in Bar
    float	mMAP = 0.0f;
    // Throttle in % (0..100)
    float	mThrottle = 0.0f;
    // Throttle deriv in %/s (-1000..1000%/s)
    float	mThrottleDeriv = 0.0f;
	// Air flow (Hz)
	float	mAirFlow = 0.0f;
	// Solenoid duty cycle in % (0..100)
    float	mSolDC = 0.0f;
    // Computed gear (0..5 or 6)
    uint8_t	mGear = 0;
    // Target boost in Bar (relative)
    float	mTargetBoost = 0.0f;
    // Target boost in Bar, output of PID corrector
    float	mTargetOutput = 0.0f;
    // Engine load (%)
    float	mLoad= 0.0f;
    // Boost controler chip load (%)
    float	mCPULoad= 0.0f;
};

struct RuntimeData
{
    float   mRPM;
    float   mSpeed;
    float   mBoost;
    float   mThrottle;
    float   mSolDC;
	uint8_t mGear;
	float   mTargetBoost;
    float   mLoad;
    float   mCPULoad;
};

struct SimulationData
{
	// Force waster gate activation. 1 to 100%. 0 to deactivate override.
	uint8_t mForceWG = 0;
};
