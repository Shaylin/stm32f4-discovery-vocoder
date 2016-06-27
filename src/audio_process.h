#include <math.h>
/**
 * @brief Constant 2*pi for ease of calculations.
 */
#define TWOPI 6.28318530718

//Effect building block functions
void echo(uint16_t* buffer, uint32_t size,  uint16_t ms);
void sineCarrier(uint16_t* buffer, uint32_t size, uint16_t freq);
void squareCarrier(uint16_t* buffer, uint32_t size, uint16_t freq);
void triCarrier(uint16_t* buffer, uint32_t size, uint16_t freq);
void vibrato(uint16_t* buffer, uint32_t size, uint16_t freq);

//Effect presets
void vader(uint16_t* buffer, uint32_t size);
void probeDroid(uint16_t* buffer, uint32_t size);
void swoosh(uint16_t* buffer, uint32_t size);

/**
 * @brief Function that mixes in a delayed copy of the signal.
 * A delay of 10ms correponds to a slight reverb. 50ms is a noticeable
 * echo.
 * @param buffer Pointer to the unsigned 16-bit integer signal buffer.
 * @param size Size of the signal buffer.
 * @param ms Amount of time in milliseconds to delay the second
 * mixed in signal. 
 */
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
}

/**
 * @brief Performs amplitude modulation with a sine wave with user defined frequency
 * @param buffer Pointer to the unsigned 16-bit integer signal buffer.
 * @param size Size of the signal buffer.
 * @param freq Frequency of the sine wave to modulate with
 */
void sineCarrier(uint16_t* buffer, uint32_t size, uint16_t freq)
{
	uint32_t i;
	for(i=0; i<size; i++)
	{
		//The 1800 in this case is approximately the 1.5V offset on the input signal
		float temp = (sinf(TWOPI*i/16000.0*freq)) * (buffer[i]-1800);
		buffer[i] = temp+1800;
	}
}

/**
 * @brief Performs amplitude modulation with a square wave with user defined frequency
 * @param buffer Pointer to the unsigned 16-bit integer signal buffer.
 * @param size Size of the signal buffer.
 * @param freq Frequency of the square wave to modulate with.
 */
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
}

/**
 * @brief Performs amplitude modulation with a triangle wave with user defined frequency
 * @param buffer Pointer to the unsigned 16-bit integer signal buffer.
 * @param size Size of the signal buffer.
 * @param freq Frequency of the triangle wave to modulate with.
 */
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
}

/**
 * @brief Preset effect using echo, sine modulation and time stretching
 * to produce a darth vader like voice.
 * @param buffer Pointer to the unsigned 16-bit integer signal buffer.
 * @param size Size of the signal buffer.
 */
void vader(uint16_t* buffer, uint32_t size)
{
	sineCarrier(buffer,size,42);
	echo(buffer, size, 50);
	//The method has to be called twice because of a quirk with the 
	//way audio input and output with DMA works.
	//Calling it twice resets all interrupt flags and set bits which 
	//would otherwise prevent it from starting.
	initAudioOut2(buffer,size,6700);
	initAudioOut2(buffer,size,6700);
}

/**
 * @brief Preset effect using echo and sine modulation
 * to produce an imperial probe droid like voice.
 * @param buffer Pointer to the unsigned 16-bit integer signal buffer.
 * @param size Size of the signal buffer.
 */
void probeDroid(uint16_t* buffer, uint32_t size)
{
	sineCarrier(buffer,size,1500);
	echo(buffer, size, 20);
	//The method has to be called twice because of a quirk with the 
	//way audio input and output with DMA works.
	//Calling it twice resets all interrupt flags and set bits which 
	//would otherwise prevent it from starting.
	initAudioOut(buffer,size);
	initAudioOut(buffer,size);
}


/**
 * @brief Function that effectively performs a variable echo. The delay
 * used in the signal is a triangular wave function in this case.
 * @param buffer Pointer to the unsigned 16-bit integer signal buffer.
 * @param size Size of the signal buffer.
 * @param freq Frequency of the triangle wave used to calculate the
 * varying delay amounts.
 */
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
}

/**
 * @brief Preset effect using amplitude modulation where the frequency
 * of the carrier wave is a function of a lower frequency wave. It should
 * produce a voice which oscilates in tone.
 * @param buffer Pointer to the unsigned 16-bit integer signal buffer.
 * @param size Size of the signal buffer.
 */
void swoosh(uint16_t* buffer, uint32_t size)
{
	uint32_t i;
	float freq;

	for(i=0; i<size; i++)
	{
		//Frequency oscillates according to a much lower frequency wave
		freq = 110.0 * (sinf(TWOPI*i/16000.0*11)+1.0) + 1.0;

		//The 1800 in this case is approximately the 1.5V offset on the input signal
		float temp = (sinf(TWOPI*i/16000.0*freq)) * (buffer[i]-1800);
		buffer[i] = temp+1800;
	}

	initAudioOut2(buffer,size,6300);
	initAudioOut2(buffer,size,6300);
}
