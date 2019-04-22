#include <SoftwareSerial.h>
#include <Wire.h>

SoftwareSerial mySerial = SoftwareSerial(10,11);

// initilialisation of 
char capacitorVoltageP = 0;
char inputCurrentVoltageP = 1;

// set pin numbers
const char IRCaptor = 2;
const int buttonPin = 6;     
const char transistorLoad = 5;
const char transistorUnload = 7;
const char testLed = 8;

// initialisation variables
long fireTime;
long passageTime;
long impactTime;
float deltaT = 0;
int buttonState = 0;
int departed = 0;
int voltageValue = 0;
int test = 0;
String confirmation = "";
int verif = 0;

void setup()
{
  Serial.begin(9600);
  
  // uart configuration with BBB
  pinMode(10,INPUT);
  pinMode(11,OUTPUT);
  mySerial.begin(9600);

  // configuration of captors and transistors
  pinMode(transistorLoad, OUTPUT);
  pinMode(transistorUnload, OUTPUT);
  pinMode(IRCaptor, INPUT);
  pinMode(buttonPin, INPUT);
  pinMode(testLed, OUTPUT);
  
}
 
void loop()
{
  // BBB to arduino 
  
  
  if(mySerial.available() > 0)
  {
    String storedData = "";
    while(mySerial.available())          // reading data into char array 
    {
     char inChar = mySerial.read();
     storedData += inChar;
    }
    voltageValue = storedData.toInt();
    delay(500);
    Serial.print(voltageValue);
    Serial.write("\n");
    // test
    verif = 1;
    if (voltageValue > 14){
      Serial.print("helloboys\n");
      if (15 <= voltageValue && voltageValue <= 30){
        Serial.print("test2\n");
        digitalWrite(testLed, HIGH);
        delay(500);
        digitalWrite(testLed, LOW);
        delay(500);
        digitalWrite(testLed, HIGH);
        delay(500);
        digitalWrite(testLed, LOW);
        voltageValue = 0;
        confirmation = "launched";
      }
      else if (voltageValue > 30){
        Serial.print("test3\n");
        digitalWrite(testLed, HIGH);
        delay(500);
        digitalWrite(testLed, LOW);
        delay(500);
        digitalWrite(testLed, HIGH);
        delay(500);
        digitalWrite(testLed, LOW);
        delay(500);
        digitalWrite(testLed, HIGH);
        delay(500);
        digitalWrite(testLed, LOW);
        voltageValue = 0;
        confirmation = "launched";
      }
    }  
  }

  

  if (digitalRead(IRCaptor) == LOW and departed == 0)
  {
    passageTime = millis();
    Serial.print("detected\n");
    departed = 1;
  }
  
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);
  
  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  if (buttonState == HIGH && departed == 1) {
    Serial.print("Impact\n");
    
    impactTime = millis();
    deltaT = (impactTime - passageTime );
    Serial.print(deltaT);
    Serial.print("\n");
    departed = 0;
  }    

  // sending of the speed of object
  if (deltaT != 0){ 
    int speed1 = deltaT;
    int output = 70;
    String outputS = String(output);
    String speed1S = String(speed1);
    
    Serial.print("S" + speed1S);
    mySerial.println("S" + speed1S + "O" + outputS);
    Serial.print("\n");
    Serial.print("Test confirmed\n");  
    deltaT = 0;   
    test = 5; 
  }

  if (verif != 0){
    String conf = "not launched";
    if (confirmation != ""){
      conf = confirmation;
    }
    mySerial.println("C" + conf);
    verif = 0;
  }
}
