/**
 * Credit to: https://code.google.com/p/generate-ppm-signal/
 *
 * This code will be used to replace the PCTx module which served
 * as an interface between controllers in Matlab and RC vehicles. 
 *
 * This module communicates with Matlab via a serial comm link. The values
 * obtained over serial are then converted to PPM values and piped into the
 * trainer port of the DX6i RC transmitter.
 */

/******************************CONFIGURATION******************************/
#define chanel_number 6  //set the number of chanels
#define default_servo_value 1500  //set the default servo value
#define PPM_FrLen 22500  //set the PPM frame length in microseconds (1ms = 1000µs)
#define PPM_PulseLen 300  //set the pulse length
#define onState 1  //set polarity of the pulses: 1 is positive, 0 is negative
#define sigPin 10  //set PPM signal output pin on the arduino
#define ledPin 7
/**************************************************************************/

/**
 * Define States
 */
#define DEFAULT 1
#define START_FRAME 2
#define START_CNFRM 3
#define DATA_ROUTE 4
#define DATA_CAP 5
#define END_FRAME 6

/*this array holds the servo values for the ppm signal
 change theese values in your code (usually servo values move between 1000 and 2000)*/ 
int ppm[chanel_number], someNum, routingInfo;
char charBuffer[12];
uint8_t State, nextState, MSB, LSB, route, count;
unsigned long interval, currentMillis, previousMillis; //Use this to keep track of time for serial ping
bool contact, ledState;

void pingEther();   //Send out packets so that Matlab can ACK and establish a connection

void runFSM();    //After connection has been established, process packets as they come in

void setup(){  
   /**
   * Pin Setup
   */
  pinMode(sigPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  /**
   * initiallize default ppm values
   */
  Serial.begin(115200);   //Configure serial port at 9600 baud

  State = START_FRAME;
  nextState = DEFAULT;
  MSB = 0;
  LSB = 0;
  route = 0;
  contact = false;
  interval = 300;
  currentMillis = 0;
  previousMillis = 0;

  for(int i=0; i < chanel_number; i++){
    ppm[i]= default_servo_value;
  }


  PORTB = (((B00000000) | !onState) << (sigPin-8)); 
  
  cli();  //No Interrupts
  TCCR1A = 0; // set entire TCCR1 register to 0
  TCCR1B = 0;
  
  OCR1A = 100;  // compare match register, change this
  TCCR1B |= (1 << WGM12);  // turn on CTC mode
  TCCR1B |= (1 << CS11);  // 8 prescaler: 0,5 microseconds at 16mhz
  TIMSK1 |= (1 << OCIE1A); // enable timer compare interrupt
  sei(); //Interrupts

  uint8_t State = DEFAULT;
  uint8_t nextState = DEFAULT;

  uint8_t MSB = 0;
  uint8_t LSB = 0;
  uint8_t route = 0;
  
  digitalWrite(ledPin, HIGH);
  establishContact();
  Serial.setTimeout(100);
  
}
 
 void establishContact() {
     while (Serial.available() <= 0) {
       //Serial.println('A', 8);   // send a capital A
       Serial.println('A');   // send a capital A
       delay(500);
     }
 }
 
void loop(){


  //if(contact == false){
    //pingEther();
  //}
  //else{     //We've made contact with Matlab, run the FSM to process packets as they come in
    runFSM();
    //static int val = 1;    
    //ppm[0] = ppm[0] + val;
    //if(ppm[0] >= 2000){ val = -1; }
    //if(ppm[0] <= 1000){ val = 1; }

  //}

}

void runFSM(){
  if(Serial.available() > 0)
  {     
        static int curChan = 0;

    int someNum = Serial.parseInt();
    //Serial.print("I recieved: ");
    //Serial.println(someNum);
    
    ppm[curChan] = Serial.parseInt();
    curChan++;
    if(curChan >= (chanel_number)) curChan = 0;
    //nextState = START_FRAME;
    
//    if(State == (START_FRAME)){
//      routingInfo = Serial.parseInt();
//      nextState = DATA_CAP;
//    }
    
    //if(State == DATA_CAP){

    //} 

    //uint8_t packt = Serial.read();

//    if(State == (START_FRAME)){
//      if(packt = 's'){  
//        nextState = START_CNFRM;  //Have potentially found the start of a frame
//      }
//      else{
//        nextState = START_FRAME;
//      }
//    }
//    
//    if(State == (START_CNFRM)){
//      if(packt = 't'){  
//        nextState = DATA_ROUTE;  //Have potentially found the start of a frame
//      }
//      else{
//        nextState = START_FRAME;
//      }
//    }
//    //Reading in two bytes
//    // xxx00yyy (first) yyyyyyyy (second)
//    if(State == DATA_ROUTE){
//      if((packt & B00011000) == 0){ //confirmed as routing bit
//        route = ((packt & B11100000) >> 5); //extract routing information
//        MSB = (packt & B00000111);
//        nextState = DATA_CAP;    
//        digitalWrite(ledPin, LOW);
//      }
//
//    }
//
//    if(State == DATA_CAP)
//    {
//      //digitalWrite(ledPin, LOW);
//
//      LSB = packt;  //extract LSB - note no checking of packt as the probability of 
//      ppm[route-1] = ((MSB << 8) | LSB);
//
//      /**
//       * if routing number is greater than or equal to number of channels, we've collected all data in packet, 
//       * restart state machine
//       * else, get next routing packet
//       */
//      if(route >= chanel_number){
//        nextState = START_FRAME;
//      }
//      else{
//        nextState = DATA_ROUTE;
//      }
//
//    }

    State = nextState;

  }
}

ISR(TIMER1_COMPA_vect){  //leave this alone
  static boolean state = true;
  
  TCNT1 = 0;
  
  if(state) {  //start pulse
    PORTB = (((B00000000) | onState) << (sigPin-8)); 
    OCR1A = PPM_PulseLen * 2;
    state = false;
  }
  else{  //end pulse and calculate when to start the next pulse
    static byte cur_chan_numb;
    static unsigned int calc_rest;
  
    PORTB = (((B00000000) | !onState) << (sigPin-8)); 
    state = true;

    if(cur_chan_numb >= chanel_number){   
      cur_chan_numb = 0;
      calc_rest = calc_rest + PPM_PulseLen;// 
      OCR1A = (PPM_FrLen - calc_rest) * 2;
      calc_rest = 0;
    }
    else{
      OCR1A = (ppm[cur_chan_numb] - PPM_PulseLen) * 2;
      calc_rest = calc_rest + ppm[cur_chan_numb];
      cur_chan_numb++;
    }     
  }
}

void pingEther(){
  currentMillis = millis();
  if(Serial.available() <= 0){
    if(currentMillis - previousMillis >= interval) {
      count++;
      // save the last time you blinked the LED 
      previousMillis = currentMillis;   
      //Serial.println('A', 8);
      Serial.println('A', 8);
    }
  }
  else{
    digitalWrite(ledPin,LOW); //Turn on LED now that a connection has been made (Sink Current)
    contact = true;
  }

}
