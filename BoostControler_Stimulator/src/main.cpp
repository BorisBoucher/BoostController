// This sketch is a stimulator for the BoostControler arduino code
//
// Target : Arduino Nano
//
// 

#include <Arduino.h>
#include <FastPID.h>

#define OUT_RPM 13
#define OUT_SPEED 12
#define OUT_MAF 11
#define IN_PLUS 7
#define IN_MINUS 8
#define OUT_VBOOST 3  // PWM out, control voltage booster
#define IN_VBOOST_REF A0    // Analog in, target voltage booster : 0..5v => 5..15V
#define IN_VBOOST A1    // Analog in, monitor voltage booster


struct State
{
  float mSpeed = 50.0f;
  float mRpm = 0.0f;
  float mMaf = 0.0f;
  uint32_t mLastSpeedPulse = 0;
  uint32_t mSpeedHalfPeriod = 0;
  uint32_t mLastRpmPulse = 0;
  uint32_t mRpmHalfPeriod = 0;
  uint32_t mLastMafPulse = 0;
  uint32_t mMafHalfPeriod = 0;
  uint32_t mLastInputCheck = 0;

  // Voltage booster control
  uint16_t mInputV = 0;   // 1000th of volt
  uint16_t mOutputV = 0;  // 1000th of volt
  uint16_t mTargetV = 13 * 1000;  // 1000th of volt
  uint16_t mLoopCount = 0;
} gState;

const double kI = 0.1 ;
const double kP = 4.0;
const double kD = 0.000;

FastPID vBoostPid(kI, kP, kD, 1000);

void setup() {
  pinMode(OUT_RPM, OUTPUT);
  pinMode(OUT_SPEED, OUTPUT);
  pinMode(OUT_MAF, OUTPUT);
  pinMode(IN_PLUS, INPUT_PULLUP);
  pinMode(IN_MINUS, INPUT_PULLUP);
  pinMode(OUT_VBOOST, OUTPUT);

  // Init serial for MUT simulation
  Serial.begin(1953);

//  Serial.begin(9600);

  // Init voltage booster PID
//  vBoostPID.setMode(1);  // auto mode
  vBoostPid.setOutputRange(0, 10 * 1000); // Clamp output to 10V => 5+10 ~15V

  // Configure PWM output for 100KHz, with a maximum of 50% duty cycle
  // Use Timer 2 and output 3
  TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(CS20); // no div
//  TCCR2B = _BV(CS22); // /64
  OCR2A = 0;
  OCR2B = 0;
}

void updateVoltageBooster(uint32_t nowUs)
{
  static uint32_t lastUpdateUs = 0U;

  if (nowUs - lastUpdateUs > 1000)
  {
    // Read input, value in 1000th of V. Read 1024 for 5V throw a voltage divider with 3.3k and 1k resistor. 
//    gState.mTargetV = 5000 + round(analogRead(IN_VBOOST_REF) / 1.024 * 10.0);
    gState.mInputV = round(analogRead(IN_VBOOST) * (1330.0f / 330.0f) / 1.024 * 5.0);
  
    gState.mOutputV = vBoostPid.step(gState.mTargetV, gState.mInputV);

    // Compute PWM value
//    OCR2B = (uint32_t(gState.mInputV) * 220) / 10000;
    OCR2B = (uint32_t(gState.mOutputV) * 200) / 10000;

    ++gState.mLoopCount;
    lastUpdateUs = nowUs;
  }
}

void loop() 
{
  uint32_t now = micros();

  updateVoltageBooster(now);

  if (now - gState.mLastInputCheck > 100000)
  {
    gState.mLastInputCheck = now;
    // @1000Hz
    if (not digitalRead(IN_PLUS) and gState.mSpeed < 350.0f)
    {
      // Increase speed
      gState.mSpeed += 1.0f;
    }
    if (not digitalRead(IN_MINUS) and gState.mSpeed > 0.0f)
    {
      // Increase speed
      gState.mSpeed -= 1.0f;
    }

    // Compute RPM based on speed, gear shift @6000RPM
    static const float gearRatios[] = {		
        12.200f,
         6.908f,
         4.383f,
         3.271f,
         2.620f,
      };

    static const float tyreCirc = 2.02f;

    // Gearbox outspeed (RPM)
    float speedms = gState.mSpeed * 1000.0 / 3600.0;
    float outHz = speedms / tyreCirc;

    int gear;
    float engineHz = 0.0;
    float engineRpm = 0.0f;
    for (gear = 0; gear < 5; ++gear)
    {
      // Compute engine RPM @ gear
      engineHz = outHz * gearRatios[gear];
      engineRpm = engineHz * 60;
      if (engineRpm < 6000)
      {
        // We found a valid gear
        break;
      }
    }

    // store engine rpm
    gState.mRpm = engineRpm;

    // Compute estimate of MAF Hz 
    // 2800Hz @ 6000rpm
    gState.mMaf = engineRpm * 0.46f;

    // Compute speed, engine and maf pulse period
    float speedHz = gState.mSpeed * 0.73f;
    float speedHalfPeriod = 1.0f / speedHz / 2.0f;
    float engineHalfPeriod = 1.0f / (engineHz * 3.0f) / 2.0f;
    float mafHalfPeriod = 1.0f / gState.mMaf / 2.0f;

    gState.mSpeedHalfPeriod = uint32_t(speedHalfPeriod * 1000000.0f);
    gState.mRpmHalfPeriod = uint32_t(engineHalfPeriod * 1000000.0f);
    gState.mMafHalfPeriod = uint32_t(mafHalfPeriod * 1000000.0f);
  }

  // Manage speed and rpm pulses
  if (gState.mSpeedHalfPeriod > 0 and now - gState.mLastSpeedPulse > gState.mSpeedHalfPeriod)
  {
    digitalWrite(OUT_SPEED, not digitalRead(OUT_SPEED));
    gState.mLastSpeedPulse = now;
  }
  if (gState.mRpmHalfPeriod > 0 and now - gState.mLastRpmPulse > gState.mRpmHalfPeriod)
  {
    digitalWrite(OUT_RPM, not digitalRead(OUT_RPM));
    gState.mLastRpmPulse = now;
  }
  if (gState.mMafHalfPeriod > 0 and now - gState.mLastMafPulse > gState.mMafHalfPeriod)
  {
    digitalWrite(OUT_MAF, not digitalRead(OUT_MAF));
    gState.mLastMafPulse = now;
  }

  // // Serial display
  // static uint32_t lastDisplay = 0;
  // if (now - lastDisplay > 1000000)
  // {
  //   Serial.print("VTGT = ");
  //   Serial.println(gState.mTargetV / 1000.0);
  //   Serial.print("VIN  = ");
  //   Serial.println(gState.mInputV / 1000.0);
  //   Serial.write("VOUT = ");
  //   Serial.println(gState.mOutputV / 1000.0);
  //   Serial.write("PWM = ");
  //   Serial.println(OCR2B);
  //   Serial.print("Loop/s = ");
  //   Serial.println(gState.mLoopCount);
  //   gState.mLoopCount = 0;
  //   lastDisplay = now;
  // }

  // MUT responder
  if (Serial.available() > 0)
  {
    int command = Serial.read();

    Serial.write(command);
    Serial.write(random(0,256));
  }
}
