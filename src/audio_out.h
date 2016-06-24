void initAudioOut(uint16_t* buffer, uint32_t size);
void initAudioOut2(uint16_t* buffer, uint32_t size, uint32_t arr);
void initAudioOutDAC(void);
void initAudioOutGPIO(void);
void initAudioOutTIM(void);
void initAudioOutTIM2(uint32_t arr);
void initAudioOutNVIC(void);
void initAudioOutDMA(uint16_t* buffer, uint32_t size);

/**
 * @brief Initialise the DAC in 12bit mode to output signal 
 * sampes. Also allow DMA access to the DAC.
 */
void initAudioOutDAC(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
	DAC_InitTypeDef DACStruct;
	DACStruct.DAC_Trigger = DAC_Trigger_T4_TRGO;
	DACStruct.DAC_WaveGeneration = DAC_WaveGeneration_None;
	DACStruct.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
	DAC_Init(DAC_Channel_1, &DACStruct);
	DAC_DMACmd(DAC_Channel_1, ENABLE);
	DAC_Cmd(DAC_Channel_1, ENABLE);
}

/**
 * @brief Initialise GPIO Pin A4 in analog mode to output
 * a signal from the DAC.
 */
void initAudioOutGPIO(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef GPIOStruct;
	GPIOStruct.GPIO_Pin = GPIO_Pin_4;//channel 1
	GPIOStruct.GPIO_Speed = GPIO_Mode_AN;
	GPIOStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIOStruct.GPIO_OType = GPIO_OType_PP;
	GPIOStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIOStruct);
}

/**
 * @brief Initialise Timer 4 with a tick rate of 16000Hz
 * to trigger the DAC to output at this rate.
 */
void initAudioOutTIM(void)
{
	//Timer 4 Runs at 84MHz
	//Do not use Timer 6 for this - It is not able to trigger the DAC
	//Even though the documentation would have you believe otherwise.
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	TIM_TimeBaseInitTypeDef TimerStruct;
	TimerStruct.TIM_Period = 5250-1;
	TimerStruct.TIM_Prescaler = 1-1;
	TimerStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TimerStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TimerStruct);
	TIM_SelectOutputTrigger(TIM4, TIM_TRGOSource_Update);
	TIM_Cmd(TIM4, ENABLE);
}

/**
 * @brief An alternate timer initialisation where the user may define 
 * the tick rate through the arr parameter. This initialisation is used 
 * when a differrent audio playback rate is required for time stretching
 * or pitch alterations.
 * @param arr The value the timer has to count up to before a timer
 * tick is generated. Timer frequency = (84MHz/(Prescaler+1))/Period.
 */
void initAudioOutTIM2(uint32_t arr)
{ 
	//Timer 4 Runs at 84MHz
	//Do not use Timer 6 for this - It is not able to trigger the DAC
	//Even though the documentation would have you believe otherwise.
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	TIM_TimeBaseInitTypeDef TimerStruct;
	TimerStruct.TIM_Period = arr-1;
	TimerStruct.TIM_Prescaler = 1-1;
	TimerStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TimerStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TimerStruct);
	TIM_SelectOutputTrigger(TIM4, TIM_TRGOSource_Update);
	TIM_Cmd(TIM4, ENABLE);
}


/**
 * @brief Initialise the NVIC to allow interupts for DMA1_Stream5
 * which is the stream being used to transfer signal data to the DAC.
 * In this case, the interupt will be triggered when the buffer has 
 * been emptied.
 */
void initAudioOutNVIC(void)
{
	NVIC_InitTypeDef NVICStruct;
	NVICStruct.NVIC_IRQChannel = DMA1_Stream5_IRQn;
	NVICStruct.NVIC_IRQChannelCmd = ENABLE;
	NVICStruct.NVIC_IRQChannelPreemptionPriority=0;
	NVICStruct.NVIC_IRQChannelSubPriority=0;
	NVIC_Init(&NVICStruct);
}

/**
 * @brief Initialises DMA to transfer the sampled data from the signal buffer
 * to the DAC. Also enables the triggering
 * of an interrupt when the buffer has been emptied.
 * @param buffer Pointer to the unsigned 16-bit integer signal buffer.
 * @param size Size of the signal buffer.

 */
void initAudioOutDMA(uint16_t* buffer, uint32_t size)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
	DMA_DeInit(DMA1_Stream5);
	DMA_InitTypeDef DMAStruct;
	DMA_StructInit(&DMAStruct);
	DMAStruct.DMA_Channel = DMA_Channel_7;
	DMAStruct.DMA_PeripheralBaseAddr =(uint32_t)(DAC_BASE + 0x08);
	DMAStruct.DMA_Memory0BaseAddr = (uint32_t)(buffer);
	DMAStruct.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	DMAStruct.DMA_BufferSize = size;
	DMAStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMAStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMAStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMAStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMAStruct.DMA_Mode = DMA_Mode_Normal;
	DMAStruct.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMAStruct.DMA_Priority = DMA_Priority_High;
	DMAStruct.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMAStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_ITConfig(DMA1_Stream5, DMA_IT_TC, ENABLE);
	DMA_Init(DMA1_Stream5, &DMAStruct);
	DMA_Cmd(DMA1_Stream5, ENABLE);
}

/**
 * @brief De-initialises all peripherals needed for audio output and 
 * Initialises them using the above methods.
 * @param buffer Pointer to the unsigned 16-bit integer signal buffer.
 * @param size Size of the signal buffer.
 */
void initAudioOut(uint16_t* buffer, uint32_t size)
{
	DAC_DeInit();
	TIM_DeInit(TIM4);
	DMA_DeInit(DMA1_Stream5);
	initAudioOutDAC();
	initAudioOutGPIO();
	initAudioOutTIM();
	initAudioOutNVIC();
	initAudioOutDMA(buffer,size);
}

/**
 * @brief De-initialises all peripherals needed for audio output and 
 * Initialises them using the above methods. Is used to initialise
 * playback at a different rate by changing the ARR of the timer.
 * @param buffer Pointer to the unsigned 16-bit integer signal buffer.
 * @param size Size of the signal buffer.
 * @param arr The value the timer has to count up to before a timer
 * tick is generated. Timer frequency = (84MHz/(Prescaler+1))/Period.
 */
void initAudioOut2(uint16_t* buffer, uint32_t size, uint32_t arr)
{
	DAC_DeInit();
	TIM_DeInit(TIM4);
	DMA_DeInit(DMA1_Stream5);
	initAudioOutDAC();
	initAudioOutGPIO();
	initAudioOutTIM2(arr);
	initAudioOutNVIC();
	initAudioOutDMA(buffer,size);
}

