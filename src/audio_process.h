#include <math.h>
#define TWOPI 6.28318530718

void echo(uint16_t* buffer, uint32_t size,  uint16_t ms);
void sineCarrier(uint16_t* buffer, uint32_t size, uint16_t freq);
void doubleSineCarrier(uint16_t* buffer, uint32_t size, uint16_t freq);
void squareCarrier(uint16_t* buffer, uint32_t size, uint16_t freq);
void triCarrier(uint16_t* buffer, uint32_t size, uint16_t freq);
void distort(uint16_t* buffer, uint32_t size, float level);
void overdrive(uint16_t* buffer, uint32_t size);
void vader(uint16_t* buffer, uint32_t size);
void vibrato(uint16_t* , uint32_t size);

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

void distort(uint16_t* buffer, uint32_t size, float level)
{
	uint32_t i;
	for(i=0; i<size; i++)
	{
		float e = 1.0-exp((-1)*buffer[i]-1800);
		buffer[i] = (uint16_t)(e*buffer[i]);
	}
	initAudioOut(buffer,size);
	initAudioOut(buffer,size);
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
	initAudioOut(buffer,size);
	initAudioOut(buffer,size);
}

void doubleSineCarrier(uint16_t* buffer, uint32_t size, uint16_t freq)
{
	uint32_t i;
	for(i=0; i<size; i++)
	{
		//The 1800 in this case is approximately the 1.5V offset on the input signal
		float temp = (sinf(TWOPI*i/16000.0*freq)) * (buffer[i]-1800);
		buffer[i] = temp+1800;
	}
	for(i=0; i<size; i++)
	{
		//The 1800 in this case is approximately the 1.5V offset on the input signal
		float temp = (sinf(TWOPI*i/16000.0*freq/2.0)) * (buffer[i]-1800);
		buffer[i] = temp+1800;
	}
	initAudioOut(buffer,size);
	initAudioOut(buffer,size);
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

void overdrive(uint16_t* buffer, uint32_t size)
{
	uint32_t i;
	for(i=0; i<size; i++)
	{	if(buffer[i]<1024)
		{
			buffer[i]=buffer[i]*2;
		}
		else if(buffer[i]<2500)
		{
			buffer[i]=(uint16_t)((3.0-(2.0-3.0*buffer[i]/4096.0)*(2.0-3.0*buffer[i]/4096.0))/3.0);
		}
		else
		{
			buffer[i] = 4069;
		}
	}
	initAudioOut(buffer,size);
	initAudioOut(buffer,size);
}

void vader(uint16_t* buffer, uint32_t size)
{
	echo(buffer, size, 60);
	initAudioOut2(buffer,size,7200);
	initAudioOut2(buffer,size,7200);
}

//Similar to echo - but the delay is varied according to an LFO
void vibrato(uint16_t* , uint32_t size)
{
	uint32_t i;
		for(i=0; i<size; i++)
		{
			int16_t temp = buffer[i] - 1800;
			float val = temp/4096.0;
			buffer[i] = (uint16_t)((val*val*val)*temp+1800);
		}
}
