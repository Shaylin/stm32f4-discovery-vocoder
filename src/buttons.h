void initButtons(void);
int8_t checkButton(uint8_t butno);

void initButtons(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	GPIO_InitTypeDef GPIOStruct;
	GPIOStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIOStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
	GPIOStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIOStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOD, &GPIOStruct);
}

int8_t checkButton(uint8_t butno)
{
	if(butno==0)
	{
		return GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_0);
	}
	else if(butno==1)
	{
		return GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_1);
	}
	else if(butno==2)
	{
		return GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_2);
	}
	else
	{
		return -1;
	}
}
