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


AnalogConverter::AnalogConverter(ADC_HandleTypeDef* hadc)
:	mHadc(hadc)
{
}

volatile uint16_t gAdcOutputTable[NB_ADC_CONV] = {0};

void AnalogConverter::setup()
{
	// Init ADC
//	HAL_ADCEx_Calibration_Start(mHadc);

	mLastConvStart = HAL_GetTick();
	// Start 1st conversion
	mDEBUG_last_start_DMA_ret = HAL_ADC_Start_DMA(mHadc, (uint32_t*)gAdcOutputTable, NB_ADC_CONV);
}

void AnalogConverter::loop()
{
	if (not mBufferReady)
	{
		// check for 'missed' interrupt...
		auto now = HAL_GetTick();
		if (now - mLastConvStart > 100)
		{
			// more than 100ms... Restart conv
			mLastConvStart = HAL_GetTick();
		 	mDEBUG_last_start_DMA_ret = HAL_ADC_Start_DMA(mHadc, (uint32_t*)gAdcOutputTable, NB_ADC_CONV);
		}
	}
	else
	{
		// Apply conversion factors
		for (unsigned int i=0; i<NB_ADC_CONV; ++i)
		{
			mAdcOutputTable[i] = gAdcOutputTable[i] * aci[i].mConvFactor + aci[i].mConvOffset;
	//		mAdcOutputTable[i] = gAdcOutputTable[i];
		}

		// Start next conversion and wait
		mBufferReady = false;
		mDEBUG_DMA_started = true;
		mConvertiongDone = false;
		mLastConvStart = HAL_GetTick();
		mDEBUG_last_start_DMA_ret = HAL_ADC_Start_DMA(mHadc, (uint32_t*)gAdcOutputTable, NB_ADC_CONV);
	}
}

// called from interrupt context
void AnalogConverter::onConvertionComplete()
{
	++mIterruptCounter;
	mDEBUG_DMA_started = false;
	mConvertiongDone = true;

	if (not mBufferReady)
	{
		mBufferReady = true;
	}
	else
	{
		mDEBUG_EVENT = true;
	}
}

float AnalogConverter::getAnalogInput(AnalogInput inputId)
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

	void HAL_ADC_ErrorCallback(ADC_HandleTypeDef* hadc)
	{
		gAnalogConverter.onConvertionComplete();
	}

} // extern "C"
