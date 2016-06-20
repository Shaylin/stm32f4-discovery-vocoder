/* Includes */
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "audio_out.h"
#include "audio_in.h"
#include <stdlib.h>

uint8_t inputDone = 0;
uint8_t outputDone = 0;
uint16_t buffer[64000];

void DMA2_Stream0_IRQHandler(void){
	if (DMA_GetITStatus(DMA2_Stream0, DMA_IT_TCIF0))
	{
		DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_TCIF0);
		inputDone = 1;
		initAudioOut(buffer,64000); //FOR SOME PERVERSE REASON IT WORKS WHEN IT'S HERE
	}
}

void DMA1_Stream5_IRQHandler(void){
	if (DMA_GetITStatus(DMA1_Stream5, DMA_IT_TCIF5))
	{
		DMA_ClearITPendingBit(DMA1_Stream5, DMA_IT_TCIF5);
		outputDone = 1;
	}
}

int main(void)
{
	int i = 0;

	initAudioIn(buffer, 64000);

	while (1)
	{
		i++;
		if(inputDone)
		{
			STM_EVAL_LEDInit(LED5);
			STM_EVAL_LEDOn(LED5);
		}
		else
		{
			STM_EVAL_LEDOff(LED5);
		}
		if(outputDone)
		{
			STM_EVAL_LEDInit(LED4);
			STM_EVAL_LEDOn(LED4);
		}
		else
		{
			STM_EVAL_LEDOff(LED4);
		}
	}
}



void EVAL_AUDIO_TransferComplete_CallBack(uint32_t pBuffer, uint32_t Size){return;}
uint16_t EVAL_AUDIO_GetSampleCallBack(void){return -1;}
