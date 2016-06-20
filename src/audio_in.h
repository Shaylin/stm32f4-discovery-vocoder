void initAudioIn(uint16_t* buffer, uint32_t size);

void initAudioInGPIO(void);
void initAudioInTIM(void);
void initAudioInADC(void);
void initAudioInNVIC(void);
void initAudioInDMA(uint16_t* buffer, uint32_t size);


void initAudioInGPIO(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);

	GPIO_InitTypeDef GPIOStruct;
	GPIOStruct.GPIO_Mode = GPIO_Mode_AN;
	GPIOStruct.GPIO_Pin = GPIO_Pin_2;
	GPIOStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIOStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOC, &GPIOStruct);
}

void initAudioInTIM(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);

	TIM_TimeBaseInitTypeDef TimerStruct;
	TimerStruct.TIM_Period = 2625*4-1;
	TimerStruct.TIM_Prescaler = 1-1;
	TimerStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TimerStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM2, &TimerStruct);
	TIM_SelectOutputTrigger(TIM2,TIM_TRGOSource_Update);
	TIM_Cmd(TIM2, ENABLE);
}

void initAudioInADC(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	ADC_CommonInitTypeDef ADCCommonStruct;
	ADCCommonStruct.ADC_Mode = ADC_Mode_Independent;
	ADCCommonStruct.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	ADCCommonStruct.ADC_Prescaler = ADC_Prescaler_Div2;
	ADCCommonStruct.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInit(&ADCCommonStruct);

	ADC_InitTypeDef ADCStruct;
	ADCStruct.ADC_ContinuousConvMode = DISABLE;
	ADCStruct.ADC_DataAlign = ADC_DataAlign_Right;
	ADCStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T2_TRGO;
	ADCStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Rising;
	ADCStruct.ADC_NbrOfConversion = 1;
	ADCStruct.ADC_Resolution = ADC_Resolution_12b;
	ADCStruct.ADC_ScanConvMode = DISABLE;
	ADC_Init(ADC1, &ADCStruct);

	ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 1, ADC_SampleTime_15Cycles);
	ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
	ADC_DMACmd(ADC1, ENABLE);
	ADC_Cmd(ADC1, ENABLE);
}

void initAudioInNVIC(void)
{
	NVIC_InitTypeDef NVICStruct;
	NVICStruct.NVIC_IRQChannel = DMA2_Stream0_IRQn;
	NVICStruct.NVIC_IRQChannelCmd = ENABLE;
	NVICStruct.NVIC_IRQChannelPreemptionPriority=0;
	NVICStruct.NVIC_IRQChannelSubPriority=0;
	NVIC_Init(&NVICStruct);
}

void initAudioInDMA(uint16_t* buffer, uint32_t size)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2 ,ENABLE);

	DMA_InitTypeDef DMAStruct;
	DMA_DeInit(DMA2_Stream0);
	DMAStruct.DMA_Channel = DMA_Channel_0;
	DMAStruct.DMA_PeripheralBaseAddr = (uint32_t)(&ADC1->DR);
	DMAStruct.DMA_Memory0BaseAddr = (uint32_t)(buffer);
	DMAStruct.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMAStruct.DMA_BufferSize = size;
	DMAStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMAStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMAStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMAStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMAStruct.DMA_Mode = DMA_Mode_Normal;
	DMAStruct.DMA_Priority = DMA_Priority_High;
	DMAStruct.DMA_FIFOMode = DMA_FIFOMode_Enable;
	DMAStruct.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMAStruct.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMAStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA2_Stream0, &DMAStruct);
	DMA_ITConfig(DMA2_Stream0, DMA_IT_TC, ENABLE);
	DMA_Cmd(DMA2_Stream0, ENABLE);
}

void initAudioIn(uint16_t* buffer, uint32_t size)
{
	TIM_DeInit(TIM2);
	ADC_DeInit();
	DMA_DeInit(DMA2_Stream0);
	initAudioInGPIO();
	initAudioInTIM();
	initAudioInADC();
	initAudioInNVIC();
	initAudioInDMA(buffer, size);
}

