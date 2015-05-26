

/**
 This software is written using Daniel van den Ouden's RC Library
 This code is intended to replace the PCTx by endurance, which uses a PIC18 microcontroller

 This sketch can be loaded onto an Arduino, and interfaced with any standard RC trainer port
 This code was developed to interface with Matlab and allow for the devleopment of vehicle control 
 algorithms.
 
 This software is in the public domain, furnished "as is", without technical
 support, and with no warranty, express or implied, as to its usefulness for
 any purpose.

 ppmoutMatlab.ino
 Demonstrate Pulse Position Modulation Output functionality
 Author: Ryan Rodriguez
 Project: ArduinoRCLib
 Group: Hybrid Systems Lab
 Website: github.com/empireryan
 */

#include <PPMOut.h>
#include <Timer1.h>
#include <DigitalToggle.h>

#define CHANNELS 6

/**
 * Define States
 */

#define DEFAULT 0
#define START_FRAME 1
#define START_CNFRM 2
#define DATA_ROUTE 3
#define DATA_CAP 4
#define END_FRAME 5

uint16_t matlabInput[CHANNELS];
uint8_t  matlabWork[PPMOUT_WORK_SIZE(CHANNELS)];  // WORK_SIZE function returns a working buffer based on the number of channels
bool LED_State = FALSE;
// PPMOut requires two buffers:
//     Input buffer containing input samples in microseconds
//     Work buffer of ((channels + 1) * 2) elements for internal calculations and frame buffering
// This setup removes any limit on the number of channels you want, and makes sure the library doesn't use more
// memory than it really needs, since the client code supplies the buffers.

rc::PPMOut DX6_PPMOut(CHANNELS, matlabInput, matlabWork, CHANNELS);

void setup()
{
	/**
	 * Do FSM setup for packet routing
	 */
	uint8_t State = DEFAULT;
	uint8_t nextState = DEFAULT;

	uint8_t MSB = 0;
	uint8_t LSB = 0;
	uint8_t route = 0;

	Serial.begin(9600);		//Configure serial port at 9600 baud
	digitalWrite(ledPin,HIGH);	//Keep LED off until we've established a connection (High Impedance)
	establishContact();  // send bytes out into the ether until a receiver responds
	digitalWrite(ledPin,LOW);	//Turn on LED now that a connection has been made (Sink Current)
	
	// Initialize timer1, this is required for all features that use Timer1
	rc::Timer1::init();
	
	for (uint8_t i = 0;  i < CHANNELS; ++i)
	{		
		/**
		 * Initialize input values to 'dead-center'
		 * This is typically 1.5mS for servos
		 */
		matlabInput[i] = 1500;
	}
	
	// initialize PPMOut with some settings
	// 
	/**
	 * @todo: Need to compare these values with what I see coming out of the PCTx GUI
	 */
	DX6_PPMOut.setPulseLength(448);   // pulse length in microseconds
	DX6_PPMOut.setPauseLength(10448); // length of pause after last channel in microseconds
	// note: this is also called the end of frame, or start of frame, and is usually around 10ms
	
	// start PPMOut, pin 9/10 offer highest accuracy
	DX6_PPMOut.start(9);
}

/**
 * Process new Serial data, if available
 * Update PPM outputs if there is new data available
 */
void loop()
{
	if(Serial.available()) // if there is data to read
	{
		uint8_t packt = Serial.read();

		if(State == (DEFAULT || START_FRAME)){
			if(packt = B10101010){
				nextState = START_CNFRM;	//Have potentially found the start of a frame
			}
			else{
				nextState = START_FRAME;
			}
		}

		if(State == START_CNFRM){
			if(packt == B10101010){
				nextState = DATA_ROUTE;
			}
			else{
				nextState = START_FRAME;
			}
		}

		//Reading in two bytes
		// xxx000yy (first) yyyyyyyy (second)
		if(State == DATA_ROUTE){
			if((packt & B00011100) == B00000000){	//confirmed as routing bit
				route = ((packt & B11100000) >> 5);	//extract routing information
				MSB = (packt & B00000011);
				nextState = DATA_CAP;
			}
		}

		if(State == DATA_CAP){
			LSB = packt;	//extract LSB - note no checking of packt as the probability of 
							//incorrect framing at this point is very low
			matlabInput[route] = (MSB << 8 | LSB); //combine MSB and LSB into uint16_t
			/**
			 * if routing number is greater than or equal to number of channels, we've collected all data in packet, 
			 * restart state machine
			 * else, get next routing packet
			 */
			if(route >= CHANNELS){
				nextState = END_FRAME
			}
			else{
				nextState = DATA_ROUTE;
			}

		}

		if(State == END_FRAME){
		/**
		 * Process packet
		 * @todo: Consider adding a protection against out-of-bounds values
		 */
		//for(uint8_t i = 0; i < CHANNELSl i++){
			//matlabInput[i] = Serial.read();
			DX6_PPMOut.update();
			nextState = START_FRAME;
		}
	}	
	State = nextState;

	// tell PPMOut there are new values available in the input buffer
	DX6_PPMOut.update();
}
