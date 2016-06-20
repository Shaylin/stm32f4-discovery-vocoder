/* Includes */
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "audio_out.h"
#include "audio_in.h"
#include <stdlib.h>

uint8_t inputDone = 0;


void DMA2_Stream0_IRQHandler(void){
	if (DMA_GetITStatus(DMA2_Stream0, DMA_IT_TCIF0))
	{
		DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_TCIF0);
		inputDone = 1;
	}
}

int main(void)
{
	int i = 0;
	uint16_t* buffer = (uint16_t*)malloc(sizeof(uint16_t)*64000);
	initAudioIn(buffer, 64000*2);
	while(!inputDone);

	initAudioOut(buffer,64000);
	initAudioOut(buffer,64000);

	while (1)
	{
		i++;
	}
}



void EVAL_AUDIO_TransferComplete_CallBack(uint32_t pBuffer, uint32_t Size){return;}
uint16_t EVAL_AUDIO_GetSampleCallBack(void){return -1;}
