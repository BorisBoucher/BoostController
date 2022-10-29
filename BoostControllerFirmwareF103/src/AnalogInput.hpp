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
	OIL_PRESS_IN,
	WATER_TEMP_IN,
	OIL_TEMP_IN,
	THROTTLE_IN,
	AFR_IN
};

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

//constexpr const float FULL_SCALE_ADC = 4096.0f;
// Compensate for voltage divisor ratio
constexpr const float FULL_SCALE_ADC = 3600.0f;

const auto aci = std::to_array<AnalogConvItem>( 
{
	// Channel			Factor					Offset  conversion function
	{MAP_IN, 		     3.0f / FULL_SCALE_ADC, 0.0f, unityConv},		// 3 Bar full scale
	{FUEL_PRESS_IN,     14.0f / FULL_SCALE_ADC, 14.0f / 5.0f * 0.5f, unityConv},		// 14 Bar full scale
	{OIL_PRESS_IN, 	    14.0f / FULL_SCALE_ADC, 0.0f, unityConv},		// 14 Bar full scale
	// Convert to 0..5V then to temperature curve
	{WATER_TEMP_IN,	     5.0f / FULL_SCALE_ADC, 0.0f, waterTempConv},	// 5V @ 150°C
	{OIL_TEMP_IN,      (180.0f-20.0f) / FULL_SCALE_ADC, 20.0f, unityConv},		// 0V@20°C, 5V @ 180°C
	{THROTTLE_IN,      100.0f / FULL_SCALE_ADC, 0.0f, unityConv},		// 100% full scale
	{AFR_IN,             1.0f / FULL_SCALE_ADC, 0.0f, unityConv},		// TBD
});

constexpr const size_t NB_ADC_CONV = std::tuple_size<decltype(aci)>::value;

class AnalogConverter
{
	ADC_HandleTypeDef* mHadc;

	// Analog converted and scaled ouput
	double mAdcOutputTable[NB_ADC_CONV] = {};

	// Lost DMA interrupt tracker
	uint32_t mLastConvStart = 0;

	// Current buffer
//	int mCurrentBuffer = 0;

	// Buffer ready indicator
	volatile bool mBufferReady = false;

	volatile bool mDEBUG_EVENT = false;

	volatile HAL_StatusTypeDef mDEBUG_last_start_DMA_ret = HAL_OK;
	volatile bool mDEBUG_DMA_started = false;
	volatile bool mConvertiongDone = false;
	volatile uint32_t mIterruptCounter = 0;

public:

	AnalogConverter(ADC_HandleTypeDef* hadc);

	void setup();

	void loop();

	// called from interrupt context
	void onConvertionComplete();

	float getAnalogInput(AnalogInput inputId);
};

extern AnalogConverter gAnalogConverter;

extern "C"
{
	void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc);
} // extern "C"
