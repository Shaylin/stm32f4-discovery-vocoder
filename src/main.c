
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "audio_out.h"
#include "audio_in.h"
#include "audio_process.h"
#include "buttons.h"
#include "lcd.h"
#include <stdlib.h>

/**
 * @brief Variable that keeps track of the current effect being used to process the
 * input signal.
 * 0 = Darth Vader,
 * 1 = Imperial Probe Droid
 */
int8_t currentEffect = 0;
/**
 * @brief Flag which is set high when the audio input has been completed.
 */
uint8_t inputDone = 1;
/**
 * @brief Flag which is set high when the audio output has been completed.
 */
uint8_t outputDone = 1;
/**
 * @brief Flag which is set high to indicate the the audio input requires processing.
 */
uint8_t process = 0;
/**
 * @brief Pointer to the unsigned 16-bit integer signal buffer.
 */
uint16_t* buffer;
/**
 * @brief Size of the signal buffer.
 */
uint32_t size = 48000;

/**
 * @brief Interrupt handler which is triggered when DMA2_Stream0, or the ADC signal
 * input has filled the buffer.
 */
void DMA2_Stream0_IRQHandler(void)
{
	if (DMA_GetITStatus(DMA2_Stream0, DMA_IT_TCIF0))
	{
		DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_TCIF0);
		inputDone = 1;
		process = 1;
	}
}

/**
 * @brief Interrupt handler which is triggered when DMA1_Stream5, or the DAC signal
 * output has gone through the buffer.
 */
void DMA1_Stream5_IRQHandler(void)
{
	if (DMA_GetITStatus(DMA1_Stream5, DMA_IT_TCIF5))
	{
		DMA_ClearITPendingBit(DMA1_Stream5, DMA_IT_TCIF5);
		outputDone = 1;
	}
}

/**
 * @brief Main loop which runs through the following procedures:
 * Wait for button input to begin audio input, Wait for completion
 * of input and for process flag to be set to begin audio processing,
 * Begin audio output when processing is done, reset all flags when
 * output is complete and wait once more for button input.
 */
int main(void)
{
	buffer = (uint16_t*)malloc(sizeof(uint16_t)*size);
	initButtons();
	initLCD();
	uint8_t prev0=0;
	uint8_t prev1=0;
	uint8_t prev2=0;

	while (1)
	{
		//Manages LCD screen display
		if(currentEffect==2)
		{
			LCD_LINE(1);
			LCD_STR("Swoosh?        ");
			LCD_LINE(2);
			LCD_STR("                ");
		}
		else if(currentEffect==1)
		{
			LCD_LINE(1);
			LCD_STR("Imperial Probe");
			LCD_LINE(2);
			LCD_STR("Droid");
		}
		else if(currentEffect==0)
		{
			LCD_LINE(1);
			LCD_STR("Darth Vader     ");
			LCD_LINE(2);
			LCD_STR("                ");
		}

		//Buttons used to cycle between effect presets
		if(checkButton(2) && checkButton(2)!=prev2)
		{
			currentEffect++;
			if(currentEffect>2)
			{
				currentEffect=0;
			}
		}
		else if(checkButton(1) && checkButton(1)!=prev1)
		{
			currentEffect--;
			if(currentEffect<0)
			{
				currentEffect=2;
			}
		}

		//Manages the LED indicators
		if(inputDone && !process && outputDone)
		{
			STM_EVAL_LEDInit(LED4);
			STM_EVAL_LEDOn(LED4);
		}
		else
		{
			STM_EVAL_LEDOff(LED4);
		}
		if(!inputDone)
		{
			STM_EVAL_LEDInit(LED5);
			STM_EVAL_LEDOn(LED5);
		}
		else
		{
			STM_EVAL_LEDOff(LED5);
		}
		if(!outputDone)
		{
			STM_EVAL_LEDInit(LED6);
			STM_EVAL_LEDOn(LED6);
		}
		else
		{
			STM_EVAL_LEDOff(LED6);
		}

		if(inputDone && !process && outputDone && checkButton(0)==1 && checkButton(0)!=prev0)
		{
			inputDone = 0;
			outputDone = 0;
			initAudioIn(buffer,size);
		}
		if(process)
		{
			if(currentEffect==2)
			{
				swoosh(buffer,size);
			}
			else if(currentEffect==1)
			{
				probeDroid(buffer,size);
			}
			else
			{
				vader(buffer,size);
			}
			process = 0;
		}

		prev0 = checkButton(0);
		prev1 = checkButton(1);
		prev2 = checkButton(2);
	}
}

void EVAL_AUDIO_TransferComplete_CallBack(uint32_t pBuffer, uint32_t Size){return;}
uint16_t EVAL_AUDIO_GetSampleCallBack(void){return -1;}
