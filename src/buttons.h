void initButtons(void);
int8_t checkButton(uint8_t butno);

/**
 * @brief Initialise Pins D0,D1 and D2 in input mode with pull up
 * resistors. The buttons connected to these pins are expected to be
 * hardware debounced and active high.
 */
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

/**
 * @brief 
 * @param butno The number of the button where Pin DO correponds to button 0
 * , Pin D1 corresponds to button 1 and pin D2 coresponds to button 2.
 * @returns 1 if the button is being pushed (voltage on the pin is high)
 * 0 otherwise.
 */
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
