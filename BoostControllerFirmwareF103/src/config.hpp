#pragma once

#include <stdint.h>

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
	// This allow to compensate for small adjustment of throttle (e.g. sensor reporting 95% @WOT)
	uint8_t	throttleScale = 100;
};

struct Config : public Config1
{
	/// Fuel pump prime in second after power up
	uint8_t fuelPumpPrime = 5;
};
