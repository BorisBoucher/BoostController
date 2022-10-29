#include "InputCapture.hpp"



InputCaptureMng gInputCapture;

extern "C"
{

// Input capture callback called when input capture counter reach max value and wrap to zero.
// We must increase the loop counter on each capturing item and reset item that reach
// too many loop (inactivity detection).
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	const auto processCapture = [](InputCaptureItem& ici, uint32_t& captureValue)
	{
		++ici.mOverflowCount;
		if (ici.mOverflowCount >= ici.mMaxOverflow)
		{
			// no signal for too long
			captureValue = 0;
			ici.mLastCapture = 0;
			ici.mWaitFirstCapture = true;
		}
	};

	// Increment common loop counter
	++gInputCapture.mLoopCounter;

	// Process individual capture item
	processCapture(gInputCapture.mRPMItem,   gInputCapture.mValues.mRPM);
	processCapture(gInputCapture.mSpeedItem, gInputCapture.mValues.mSpeed);
	processCapture(gInputCapture.mMAFItem,   gInputCapture.mValues.mMAF);
}

// Input capture callback called when a captured input state is signaled.
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	auto processInput = [htim](InputCaptureItem& ici, uint32_t& captureValue, uint32_t capture)
	{
		uint32_t newCounter = (gInputCapture.mLoopCounter << 16) + capture;
		// manage OF if capture value is in first quarter
		if (capture < 0x4000 and (htim->Instance->SR & TIM_FLAG_UPDATE) == TIM_FLAG_UPDATE)
		{
			// loop update interrupt pending, we need to add 1 loop to the counter
			newCounter += 0x10000;
		}
		if (ici.mWaitFirstCapture)
		{
			ici.mWaitFirstCapture = false;
		}
		else
		{
			captureValue = newCounter - ici.mLastCapture;
		}

		ici.mLastCapture = newCounter;
		// reset individual loop counter to assert active signal
		ici.mOverflowCount = 0;
	};

	// Select the input to process
	switch (htim->Channel)
	{
	case HAL_TIM_ACTIVE_CHANNEL_1:
		// RPM Hz
		processInput(
			gInputCapture.mRPMItem,
			gInputCapture.mValues.mRPM,
			htim->Instance->CCR1);
		break;
	case HAL_TIM_ACTIVE_CHANNEL_2:
		// Speed Hz
		processInput(
			gInputCapture.mSpeedItem, 
			gInputCapture.mValues.mSpeed,
			htim->Instance->CCR2);
		break;
	case HAL_TIM_ACTIVE_CHANNEL_3:
		// MAF Hz
		processInput(
			gInputCapture.mMAFItem, 
			gInputCapture.mValues.mMAF,
			htim->Instance->CCR3);
		break;
	case HAL_TIM_ACTIVE_CHANNEL_4:
	case HAL_TIM_ACTIVE_CHANNEL_CLEARED:
		// nothing;
		break;
	}
}

} // extern "C"

