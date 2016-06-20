void initAudioOut(uint16_t* buffer, uint32_t size);

void initAudioOutDAC(void);
void initAudioOutGPIO(void);
void initAudioOutTIM(void);
void initAudioOutDMA(uint16_t* buffer, uint32_t size);

void initAudioOutDAC(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
	DAC_InitTypeDef DACStruct;
	DACStruct.DAC_Trigger = DAC_Trigger_T6_TRGO;
	DACStruct.DAC_WaveGeneration = DAC_WaveGeneration_None;
	DACStruct.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
	DAC_Init(DAC_Channel_1, &DACStruct);
	DAC_DMACmd(DAC_Channel_1, ENABLE);
	DAC_Cmd(DAC_Channel_1, ENABLE);
}

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

//Timer 6 Runs at 84MHz
void initAudioOutTIM(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	TIM_TimeBaseInitTypeDef TimerStruct;
	TimerStruct.TIM_Period = 2625-1;
	TimerStruct.TIM_Prescaler = 1-1;
	TimerStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TimerStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TimerStruct.TIM_RepetitionCounter = 0x0000;
	TIM_TimeBaseInit(TIM6, &TimerStruct);
	TIM_SelectOutputTrigger(TIM6, TIM_TRGOSource_Update);
	TIM_Cmd(TIM6, ENABLE);
}

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
	DMAStruct.DMA_FIFOMode = DMA_FIFOMode_Enable;
	DMAStruct.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMAStruct.DMA_Priority = DMA_Priority_High;
	DMAStruct.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMAStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA1_Stream5, &DMAStruct);
	DMA_Cmd(DMA1_Stream5, ENABLE);
}

void initAudioOut(uint16_t* buffer, uint32_t size)
{
	initAudioOutDAC();
	initAudioOutGPIO();
	initAudioOutTIM();
	initAudioOutDMA(buffer,size);
}
