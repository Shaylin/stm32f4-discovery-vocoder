#include <math.h>
#define TWOPI 6.28318530718

void echo(uint16_t* buffer, uint32_t size,  uint16_t ms);
void sineCarrier(uint16_t* buffer, uint32_t size, uint16_t freq);
void squareCarrier(uint16_t* buffer, uint32_t size, uint16_t freq);
void triCarrier(uint16_t* buffer, uint32_t size, uint16_t freq);
void vibrato(uint16_t* buffer, uint32_t size, uint16_t freq);
void vader(uint16_t* buffer, uint32_t size);
void probeDroid(uint16_t* buffer, uint32_t size);

void echo(uint16_t* buffer, uint32_t size, uint16_t ms)
{
	uint32_t samples = ms*16;
	if(samples>size)
	{
		STM_EVAL_LEDInit(LED5);
		STM_EVAL_LEDOn(LED5);
		STM_EVAL_LEDInit(LED6);
		STM_EVAL_LEDOn(LED6);
		return;
	}
	uint32_t i;
	for(i=samples; i<size; i++)
	{
		buffer[i] = buffer[i]/2 +buffer[i-samples]/2;
	}
	//Need to put this when the effects chain ends to play back
	//initAudioOut(buffer,size);
	//initAudioOut(buffer,size);
}

//Performs amplitude modulation with a sine wave with user defined frequency
void sineCarrier(uint16_t* buffer, uint32_t size, uint16_t freq)
{
	uint32_t i;
	for(i=0; i<size; i++)
	{
		//The 1800 in this case is approximately the 1.5V offset on the input signal
		float temp = (sinf(TWOPI*i/16000.0*freq)) * (buffer[i]-1800);
		buffer[i] = temp+1800;
	}
	//initAudioOut(buffer,size);
	//initAudioOut(buffer,size);
}

void squareCarrier(uint16_t* buffer, uint32_t size, uint16_t freq)
{
	uint32_t i,j;
	uint32_t period = (1.0/freq)*16000;
	float val;
	for(i=0; i<size; i++)
	{	if(j<period/2)
		{
			val=1.0;
		}
		else if(j<period)
		{
			val=0;
		}
		else
		{
			j=0;
		}
		j++;
		//The 1800 in this case is approximately the 1.5V offset on the input signal
		float temp = val * (buffer[i]-1800);
		buffer[i] = temp+1800;
	}
	initAudioOut(buffer,size);
	initAudioOut(buffer,size);
}

void triCarrier(uint16_t* buffer, uint32_t size, uint16_t freq)
{
	uint32_t i,j;
	float val;
	uint32_t period = (1.0/freq)*16000;
	for(i=0; i<size; i++)
	{
		val = (1.0*j)/period;
		if(j>period)
		{
			j=0;
		}
		j++;
		//The 1800 in this case is approximately the 1.5V offset on the input signal
		float temp = val * (buffer[i]-1800);
		buffer[i] = temp+1800;
	}
	initAudioOut(buffer,size);
	initAudioOut(buffer,size);
}

void vader(uint16_t* buffer, uint32_t size)
{
	sineCarrier(buffer,size,42);
	echo(buffer, size, 50);
	initAudioOut2(buffer,size,6800);
	initAudioOut2(buffer,size,6800);
}

void probeDroid(uint16_t* buffer, uint32_t size)
{
	sineCarrier(buffer,size,1500);
	echo(buffer, size, 20);
	initAudioOut2(buffer,size,5250);
	initAudioOut2(buffer,size,5250);
}

//Similar to echo - but the delay is varied according to an LFO
//The delay amount varies from 0 to 50ms
void vibrato(uint16_t* buffer, uint32_t size, uint16_t freq)
{
	uint32_t i;
	uint32_t j = 0;
	int32_t delay;
	uint32_t period = (1.0/freq)*16000;
	for(i=period; i<size; i++)
	{
		if(j>period)
		{
			j = 0;
		}
		delay = period - j;
		j++;
		buffer[i] = buffer[i]/2 +buffer[i-delay]/2;
	}
	//initAudioOut(buffer,size);
	//initAudioOut(buffer,size);
}
