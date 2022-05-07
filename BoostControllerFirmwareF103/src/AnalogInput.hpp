#pragma once

//#include <avr/io.h> // Standard include for AVR
// #include <atomic>
#include <stdint.h>
#include <stddef.h>
// #include <cmath>
#include <functional>
// #include <FloatDefine.h>
// #include <RunningStatistics.h>
#include "std_helper.hpp"
// #include "config.hpp"
#include "core/Inc/main.h"
// #include "BoostController.hpp"
// #include "InputCapture.hpp"


enum AnalogInput
{
	MAP_IN,
	FUEL_PRESS_IN,
	OIL_TEMP_IN,
	WATER_TEMP_IN,
	OIL_PRES_IN,
	THROTTLE_IN,
	AFR_IN
};

// template <typename T, typename U, typename V>
// T constrain(const T value, const U min, const V max)
// {
// 	T ret;
// 	if (value < min)
// 	{
// 		ret = min;
// 	}
// 	else if (value > max)
// 	{
// 		ret = max;
// 	}
// 	else
// 	{
// 		ret = value;
// 	}

// 	return ret;
// }

struct SensorCorrectionLUT
{
	float mRawValue;
	float mOutputValue;
};

float correctSensorValue(float adcRaw, const SensorCorrectionLUT* lut, size_t lutSize);


#define sizeof_array(arr) (sizeof(arr) / sizeof(arr[0]))

// Sample analog input
// Analog input:
// 0 : MAP
// 1 : FUEL PRES
// 2 : OIL PRES
// 3 : WATER TEMP
// 4 : OIL TEMP
// 5 : TPS
// 6 : AFR

/// Parameter for an analog convertion
struct AnalogConvItem
{
	/// ID of the analog parameter 
	AnalogInput		mInput;
	/// Conversion factor. Raw ADC value is multiplied by it
	float			mConvFactor;
	/// Conversion offset. This value is added to multiplied ADCvalue 
	float			mConvOffset;
	/// Conversion function. Scaled/offset value is passed to this function for further correction
	/// like non linear temperature curve correction.
	std::function<float(float)> mConvFunction;
};

/// Unity conversion function, do nothing.
float unityConv(float input);

/// Water temperature curve correction function.
float waterTempConv(float input);

// template <typename NextConv = std::function<float(float)>(&unityConv)>
// struct linearConv
// {
// 	float mScale;
// 	float mOffset;

// 	float operator()(float input)
// 	{
// 		double out = input * mScale + mOffset;

// 		return NextConv(out);
// 	}
// };

constexpr const float FULL_SCALE_ADC = 1023.0f;

const auto aci = std::to_array<AnalogConvItem>( 
{
	{MAP_IN, 		     3.0f / FULL_SCALE_ADC, 0.0f, unityConv},		// 3 Bar full scale
	{FUEL_PRESS_IN,     14.0f / FULL_SCALE_ADC, 0.0f, unityConv},		// 14 Bar full scale
	{OIL_PRES_IN, 	   150.0f / FULL_SCALE_ADC, 0.0f, unityConv},		// 14 Bar full scale
	// Convert to 0..5V then to temperature curve
	{WATER_TEMP_IN,	     5.0f / FULL_SCALE_ADC, 0.0f, waterTempConv},		// 14 Bar full scale
	{OIL_TEMP_IN,      150.0f / FULL_SCALE_ADC, 0.0f, unityConv},		// 14 Bar full scale
	{THROTTLE_IN,      100.0f / FULL_SCALE_ADC, 0.0f, unityConv},		// 100% full scale
	{AFR_IN,             1.0f / FULL_SCALE_ADC, 0.0f, unityConv},		// TBD
});

constexpr const size_t NB_ADC_CONV = std::tuple_size<decltype(aci)>::value;

class AnalogConverter
{
	ADC_HandleTypeDef* mHadc;

	// Double buffer table for DMA conversion
	uint16_t mAdcOutputTable[NB_ADC_CONV] = {};

	// Current buffer
//	int mCurrentBuffer = 0;

	// Buffer ready indicator
	volatile bool mBufferReady = false;

public:

	AnalogConverter(ADC_HandleTypeDef* hadc);

	void setup();

	void loop();

	// called from interrupt context
	void onConvertionComplete();

	uint16_t getAnalogInput(AnalogInput inputId);
};

extern AnalogConverter gAnalogConverter;

extern "C"
{
	void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc);
} // extern "C"
