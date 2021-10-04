# stm32f4_discovery_vocoder
A simple record & playback vocoder using the STM32F4 Discovery

### Features currently working:
* Audio output using the DAC and DMA
* Audio input using the ADC and DMA
* Recording and playback at different sampling rates
* Amplitude modulation with sine, square and triangle waves
* Adding echo and vibrato to signals
* LCD screen output
* Cycling between effect presets

### Pins used on the board:
* A4 - DAC Audio Output
* C2 - ADC Audio Input
* D0,D1,D2 - Pushbutton Inputs
* E9,E10,E11,E12,E13,E14 - LCD Sceen Output

The additional circuits which these pins will connect to are included in the documentation folder of this repository.
Code documentation generated with doxygen is also in this folder.

### LCD Screen
The LCD Screen used for this device is the QC1602A. This makes use of the HD44780 controller so any LCD using this controller with similar pins would work as well.
The wiring guide for the LCD Screen is as follows:
* Pin1(VSS) - Connect to ground
* Pin2(VDD) - Connect to 5V
* Pin3(VO) - Connect through a potentiometer to ground (for contrast adjustment)
* Pin4(RS) - Connect to Pin E10 on the microcontroller
* Pin5(RW) Connect to ground
* Pin6(E) - Connect to Pin E9 on the microcontroller
* Pin7(D0) - Not Connected
* Pin8(D1) - Not Connected
* Pin9(D2) - Not Connected
* Pin10(D3) - Not Connected
* Pin11(D4) - Connect to Pin E11 on the microcontroller
* Pin12(D5) - Connect to Pin E12 on the microcontroller
* Pin13(D6) - Connect to Pin E13 on the microcontroller
* Pin14(D7) - Connect to Pin E14 on the microcontroller
* Pin15(A) - Connect to 5V
* Pin16(K) - Connect to ground

  
To run this project, open the project files using Atollic True Studio.
