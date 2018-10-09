#ifndef CONFIG_DATA_H
#define CONFIG_DATA_H

enum BC_ADDR
{
    TYPE_CIRC =     0x0000,
    GEAR_RATIO_1 =  0x0001,
    GEAR_RATIO_2 =  0x0002,
    GEAR_RATIO_3 =  0x0003,
    GEAR_RATIO_4 =  0x0004,
    GEAR_RATIO_5 =  0x0005,
    GEAR_RATIO_6 =  0x0006,
    SPEED_SENSOR_CAL=  0x0007,
    REF_BOOST =     0x0008,
    PID_P =         0x0010,
    PID_I =         0x0011,
    PID_D =         0x0012,

    BOOST_TABLE_1 = 0x0100,
    BOOST_TABLE_2 = 0x0110,
    BOOST_TABLE_3 = 0x0120,
    BOOST_TABLE_4 = 0x0130,
    BOOST_TABLE_5 = 0x0140,
    BOOST_TABLE_6 = 0x0150,

    RPM =       0x0020,
    SPEED =     0x0021,
    MAP =       0x0022,
    THROTTLE =  0x0023,
    SOL_DC =    0x0024,
    GEAR =      0x0025,
    LOAD =      0x0026,
    CPU_LOAD =  0x0027,
    TARGET_BOOST =  0x0028,
    TARGET_OUTPUT = 0x0029,
    THROTTLE_DERIV = 0x002a,
};

struct ConfigData
{
    float   mTyreSize;
    float   mGearRatio[6];
    float   mSpeedSensorRatio;
    float   mReferenceBoost;

    float   mPidP;
    float   mPidI;
    float   mPidD;

    uint8_t  mBoostTable[6][7];
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
    uint8_t  mGear;
    float    mTargetBoost;
    float   mLoad;
    float   mCPULoad;
};


#endif // CONFIG_DATA_H
