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
float current;
float inputVoltage;

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
    // reading data into char array 
    while(mySerial.available())          
    {
     char inChar = mySerial.read();
     storedData += inChar;
    }
    voltageValue = storedData.toInt();
    delay(500);
    // changing verification status to now that a value has been read
    verif = 1;
    // only if voltage value chosen is high enough, activate relays
    if (voltageValue > 14){
      Serial.print("helloboys\n");
      digitalWrite(7, HIGH);
      delay(150);
      digitalWrite(7, LOW);
      confirmation = "launched";
    }  
  }

  // to detect when the projectile has been launched
  if (digitalRead(IRCaptor) == LOW and departed == 0)
  {
    passageTime = millis();
    Serial.print("detected\n");
    departed = 1;
  }

  //read output current and input voltage
  current = analogRead(A1)*(5/1024);
  //Serial.println(current);
  inputVoltage = analogRead(A3)*(5/1024)/(0.08879); //voltage divider 0,08879
  if (current != 0){
    Serial.print("Current value is : ");
    Serial.println(current);
  }
  if (inputVoltage != 0){
    Serial.print("Voltage value is : ");
    Serial.println(inputVoltage);
  }
  
  
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);
  
  // check if the pushbutton is pressed. 
  // thus projectile has reached target
  // if it is, the buttonState is HIGH:
  if (buttonState == HIGH && departed == 1) {
    Serial.print("Impact\n");
    
    impactTime = millis();
    // Time projectile to target = time(buttonPushed) - time(IRcaptor)
    deltaT = (impactTime - passageTime );
    Serial.print(deltaT);
    Serial.print("\n");
    departed = 0;
  }    

  // sending of the speed of object and output
  if (deltaT != 0){ 
    int speed1 = deltaT;
    int voltageIn = 38; //TO BE REPLACED BY ANALOG READ
    int current = 5; //TO BE REPLACED BY ANALOG READ
    String voltInS = String(voltageIn);
    String currentS = String(current);
    String speed1S = String(speed1);
    
    Serial.print("S" + speed1S);
    // sends data with 'char' to enable recognition of the data
    mySerial.println("S" + speed1S + "C" + currentS + "V" + voltInS);
    Serial.print("\n");
    Serial.print("Test confirmed\n");  
    deltaT = 0;   
    test = 5; 
  }

  // sends status of the launching 
  if (verif != 0){
    String conf = "not launched";
    if (confirmation != ""){
      conf = confirmation;
    }
    mySerial.println("l" + conf);
    verif = 0;
  }
}
