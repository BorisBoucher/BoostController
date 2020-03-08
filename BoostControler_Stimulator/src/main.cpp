// This sketch is a stimulator for the BoostControler arduino code
//
// Target : Arduino Nano
//
// 

#include <Arduino.h>

#define OUT_RPM 13
#define OUT_SPEED 12
#define OUT_MAF 11
#define IN_PLUS 7
#define IN_MINUS 8

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
} gState;

void setup() {
  pinMode(OUT_RPM, OUTPUT);
  pinMode(OUT_SPEED, OUTPUT);
  pinMode(OUT_MAF, OUTPUT);
  pinMode(IN_PLUS, INPUT_PULLUP);
  pinMode(IN_MINUS, INPUT_PULLUP);

  // Init serial for MUT simulation
  Serial.begin(1953);
}

void loop() {
  uint32_t now = micros();

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

  // MUT responder
  if (Serial.available() > 0)
  {
    int command = Serial.read();

    Serial.write(command);
    Serial.write(random(0,256));
  }
}
