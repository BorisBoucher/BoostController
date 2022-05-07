#include "AnalogInput.hpp"

float correctSensorValue(float adcRaw, const SensorCorrectionLUT* lut, size_t lutSize)
{
	// find lower value LUT
	size_t lowerIdx = 0;
	while (lowerIdx < lutSize and lut[lowerIdx].mRawValue < adcRaw)
		++lowerIdx;
	
	if (lowerIdx > 0)
	{
		--lowerIdx;
	}

	// Compute alpha 
	float deltaRaw = lut[lowerIdx+1].mRawValue - lut[lowerIdx].mRawValue;
	float deltaOut = lut[lowerIdx+1].mOutputValue - lut[lowerIdx].mOutputValue;
	float alpha = (lut[lowerIdx+1].mRawValue - adcRaw) / deltaRaw;

	// Compute output value
	float output = lut[lowerIdx].mOutputValue + deltaOut * alpha;

	return output;
}

// 0..5V to Water temp (°C)
const SensorCorrectionLUT gWaterTempLUT[] = 
{
	{  2.5f,  20.0f},
	{  2.6f,  40.0f},
	{  2.8f,  60.0f},
	{  3.2f,  80.0f},
	{  4.0f, 100.0f},
	{  5.0f, 150.0f},
};

#define sizeof_array(arr) (sizeof(arr) / sizeof(arr[0]))


/// Unity conversion function, do nothing.
float unityConv(float input)
{
	return input;
};

/// Water temperature curve correction function.
float waterTempConv(float input)
{
	return correctSensorValue(input, gWaterTempLUT, sizeof_array(gWaterTempLUT));
};

// const auto aci = std::to_array<AnalogConvItem>( 
// {
// 	{MAP_IN, 		     3.0f / FULL_SCALE_ADC, 0.0f, unityConv},		// 3 Bar full scale
// 	{FUEL_PRESS_IN,     14.0f / FULL_SCALE_ADC, 0.0f, unityConv},		// 14 Bar full scale
// 	{OIL_TEMP_IN, 	   150.0f / FULL_SCALE_ADC, 0.0f, unityConv},		// 14 Bar full scale
// 	// Convert to 0..5V then to temperature curve
// 	{WATER_TEMP_IN,	     5.0f / FULL_SCALE_ADC, 0.0f, waterTempConv},		// 14 Bar full scale
// });

// constexpr const size_t NB_ADC_CONV = std::tuple_size<decltype(aci)>::value;

AnalogConverter::AnalogConverter(ADC_HandleTypeDef* hadc)
:	mHadc(hadc)
{
}

volatile uint16_t gAdcOutputTable[NB_ADC_CONV] = {0};

void AnalogConverter::setup()
{
	// Init ADC
	HAL_ADCEx_Calibration_Start(mHadc);

	// Start 1st conversion
	HAL_ADC_Start_DMA(
		mHadc, (uint32_t*)(gAdcOutputTable), NB_ADC_CONV);
}

void AnalogConverter::loop()
{
	// Wait for conversion done
	if (not mBufferReady)
	{
		return;
	}

	// Apply conversion factors
	for (unsigned int i=0; i<NB_ADC_CONV; ++i)
	{
		mAdcOutputTable[i] = gAdcOutputTable[i];
	}

	// Start next conversion and wait
	mBufferReady = false;
	HAL_ADC_Start_DMA(
		mHadc, (uint32_t*)(gAdcOutputTable), NB_ADC_CONV);
}

// called from interrupt context
void AnalogConverter::onConvertionComplete()
{
	if (not mBufferReady)
	{
		mBufferReady = true;
	}
}

uint16_t AnalogConverter::getAnalogInput(AnalogInput inputId)
{
	return mAdcOutputTable[inputId];
}


AnalogConverter gAnalogConverter(&hadc1);

extern "C"
{
	void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
	{
		gAnalogConverter.onConvertionComplete();
	}
} // extern "C"
