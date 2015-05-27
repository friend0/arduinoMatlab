 /*
  Communications Matlab <--> Arduino
  Arduino file 1 for use with Matlab file 1 
  L. Toms
  establishContact() routine by:
  by Tom Igoe and Scott Fitzgerald
  http://www.arduino.cc/en/Tutorial/SerialCallResponse
  other ideas from
  robot grrrl
  http://robotgrrl.com/blog/2010/01/15/arduino-to-matlab-read-in-sensor-data/
  */
   int ledPin=13;
   int i = 0;

void setup() {
     
   // start serial port at 9600 bps:
   Serial.begin(9600);
   digitalWrite(ledPin,HIGH);
   establishContact();  // send a byte to establish contact until receiver responds
   digitalWrite(ledPin,LOW);
 }

 void loop() {
 Serial.println(i);
 i=i+1;  
 }

 void establishContact() {
     while (Serial.available() <= 0) {
       Serial.println('A', 8);   // send a capital A
       delay(300);
     }
 }

