#pragma once

#include <stdint.h>
#include "core/Inc/main.h"

/// Input capture support
/// We need to manage IC (Input Capture) interrupt callback as well as counter overflow interrupt 
/// callback.
/// STM" HAL will call the IC first, and lastly the overflow (OF).
/// We use OF CB to count the number of counter cycle but there is a single
/// interrupt for IC and OF, so there is a possibility of having, in a single interrupt:
///  * IT triggered by IC1, soon before OF
///  * OF interrupt raised during IT
///  * IC2 raised soon after OF, still in interrupt processing.
/// The HAL always test and callback for OF at the end of interrupt handler.
/// Therefore, once processing an IC callback, if the value is in the lower 
/// quarter value, we test the OF flag. If the flag is raised, we add 
/// one to the counter loop. 
struct InputCapture
{
	uint32_t	mRPM = 0;
	uint32_t	mSpeed = 0;
	uint32_t	mMAF = 0;
};

struct InputCaptureItem
{
	uint32_t mMaxOverflow;
	bool     mWaitFirstCapture = true;
	uint32_t mLastCapture = 0;
	uint32_t mOverflowCount = 0;
};

struct InputCaptureMng
{
	uint32_t 	mLoopCounter = 0;
	InputCaptureItem mRPMItem   {156};
	InputCaptureItem mSpeedItem { 64};
	InputCaptureItem mMAFItem   { 16};
	InputCapture mValues;
};

extern InputCaptureMng gInputCapture;

extern "C"
{

// Input capture callback called when input capture counter reach max value and wrap to zero.
// We must increase the loop counter on each capturing item and reset item that reach
// too many loop (inactivity detection).
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

// Input capture callback called when a captured input state is signaled.
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);

} // extern "C"

