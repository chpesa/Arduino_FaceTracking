#include <Wire.h>
#include <Servo.h>

#define laserPin 9        // Laser goes to DIO 9
#define PanID 10        // Pan Servo goes to DIO 10
#define TiltID 11      // Tilt Servo goes to DIO 11

Servo PanServo;  // create servo object to control a servo 
Servo TiltServo;  // create servo object to control a servo 

int laserPower = LOW;         // laser on/off state variable

// Pan & Tilt variables
byte pan = 90;      
byte tilt = 90;
int pan_temp;
int tilt_temp;
int myspeed=1;
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
String laser="";
void setup()
{
 Serial.begin(9600); // initialize serial:
 PanServo.attach(PanID);
 TiltServo.attach(TiltID);
 PanServo.write(pan);
 TiltServo.write(tilt);
 inputString.reserve(200);  // reserve 200 bytes for the inputString:
 pinMode(laserPin, OUTPUT);
 delay(1000);
 pan_temp=pan;
 tilt_temp=tilt;
  
}

void loop()
{
  if (stringComplete) {
    
    Decode();
    
    if(laser=="on")
    {
    laserPower = HIGH;
    digitalWrite(laserPin, laserPower); //send laser signal    
    }
    else if(laser=="off")
    {
    laserPower = LOW;
    digitalWrite(laserPin, laserPower); //send laser signal
    }
    
    if(pan_temp>=0 && pan_temp<=180 && tilt_temp>=0 && tilt_temp<=180)
    {
      PanServo.write(pan_temp);
      TiltServo.write(tilt_temp); //send tilt servo signal   
    }

    // *********** clear the string ***************:
     delay(15); 
    inputString = "";
    stringComplete = false;
  }
  
}
/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read(); 
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    } //end if
  }
}

void Decode()
{

  int firstSemiColon = inputString.indexOf(';');
  if (firstSemiColon>0)
  {
  String MyPan= inputString.substring(0,firstSemiColon);
   char test_as_pan[MyPan.length()+1];
   MyPan.toCharArray(test_as_pan, sizeof(test_as_pan));
  pan_temp= atoi(test_as_pan);
  }
    
  int secondSemiColon=inputString.indexOf(';',firstSemiColon+1);
  if(secondSemiColon>0)
  {
  String MyTilt=inputString.substring(firstSemiColon+1,secondSemiColon);
   char test_as_tilt[MyTilt.length()+1];
   MyTilt.toCharArray(test_as_tilt, sizeof(test_as_tilt));
  tilt_temp= atoi(test_as_tilt);
  }
  
  int EndIndex=inputString.indexOf(';',secondSemiColon+1);
  laser=inputString.substring(secondSemiColon+1,EndIndex);  
}
