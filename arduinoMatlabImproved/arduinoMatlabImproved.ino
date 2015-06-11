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
#define sigPin 9  //set PPM signal output pin on the arduino
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
int ppm[chanel_number], routingInfo;
char charBuffer[12];
uint8_t currentState, nextState, MSB, LSB, route, count;
unsigned long interval, currentMillis, previousMillis; //Use this to keep track of time for serial ping
bool contact, ledState, newLineFlag;

enum states{
  deflt,
  wait,
  startFrame,
  receivePackets,
  endFrame
};

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
  Serial.begin(9600);   //Configure serial port at 9600 baud
  currentState = startFrame;
  nextState = deflt;
  contact = false;
  interval = 300;
  currentMillis = 0;
  previousMillis = 0;
  newLineFlag = false;

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
  Serial.setTimeout(50);
}
 
 void establishContact() {
     while (Serial.available() <= 0) {
       //Serial.println('A', 8);   // send a capital A
       Serial.println('A');   // send a capital A
       delay(100);
     }
 }
 
void loop(){

  runFSM();    //update this function to return error conditions
  /*
  if(Serial.available() > 0)
  {     
    static int curChan = 0;
    if((char)Serial.peek() == '\n'){curChan = 0; Serial.read();}    
    int someNum = Serial.parseInt();
    ppm[curChan] = someNum;
    curChan++;
    if(curChan > (chanel_number)) curChan = 0;
  }
  */

}

void runFSM(){
  if(Serial.available() > 0)
  {     
    static int curChan = 0;
    
    if(currentState == startFrame){
         if(Serial.parseInt() == 65535) nextState = receivePackets;  
    }
    if(currentState == receivePackets){
         if((char)Serial.peek() == '\n'){newLineFlag = true; goto NEWLINE;}
         ppm[curChan] = Serial.parseInt();
         curChan++;
     NEWLINE:    
         if(curChan >= (chanel_number)){ curChan = 0; nextState = endFrame;} 
         else if(newLineFlag == true){newLineFlag = false; curChan = 0; nextState = startFrame;}    //error condition
    }
    if(currentState == endFrame){
        char someChar = (char)Serial.read();
        if(someChar == '\n'){nextState = startFrame;}
        else{nextState = startFrame;}      //error condition
    }
    
    currentState = nextState;
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
