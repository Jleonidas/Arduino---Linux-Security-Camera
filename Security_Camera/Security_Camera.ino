/*
Security Camera System

This program its ment to be used with arduino UNO, Nano or other ATMEGA 328 based unit.

The video recording unit uses a linux based computer to record video from a camera in case of an intrussion event.

The pheripherials used are:

1. A 4x4 keypad whose pins are conected as follow (mentiojçned from right to left)

Keypad Pins  Arduino Pins
 1              2
 2              3
 3              4
 4              5
 5              6
 6              7
 7              8
 8              9

2. A magnetic switch whose common pin its connected to ground and normaly open connected to arduino pin 10
that its defined as an pulled up input, thas why no pullup resistor its needed.

3. A Green LED with a 220 Ohm resistor whose anode its connected to arduino pin 13 and cathode connected to ground.
This LED will indicate if the password was correctly, in which case will glow for 100 miliseconds and if its not will 
glow three times with 100 miliseconds intervals.

4. A Yellow LED with a 220 Ohm resistor whose anode its connected to arduino pin 12 and cathode to ground.
This LED its intended to indicate the user that the door was closed and the system its rearmed and ready again.

This program was developed based on keypad and password sketches. Adapted by José Díaz Chow & Medrano on July 23, 2015.

for more information visit https://github.com/Jleonidas/ArduinoSecurityCamera.git

This program is under GNU GENERAL PUBLIC LICENSE

*/


#include <Password.h>
#include <Keypad.h>

// Door states
#define DOOR_CLOSED 0
#define DOOR_OPEN 1

// Password states
#define PASS_OK 1
#define PASS_NONE 0

//Security states
#define CLEAR 1
#define BROKEN 0

//Camara states
#define STOPPED 0
#define RECORDING 1

#define DOOR_PIN 10

Password password = Password( "1234" );

byte passwordState ; //  Password state
byte doorState ;     //  Door state
byte cameraState ;   //  Camera state
byte securityState ; //  Security state
byte doorSensor ;    //  Door sensor state

const byte ROWS = 4; // Four rows
const byte COLS = 4; //  columns
// Define the Keymap
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {9, 8, 7, 6}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {5, 4, 3, 2}; //connect to the column pinouts of the keypad
// Create the Keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
int led = 13;
int led1 = 12;

//-------------------------------------------------------------------------------------------------

void setup(){

  pinMode(led,OUTPUT); //Set Pin13 as Output
  pinMode(led1,OUTPUT); //Set Pin12 as Output
  pinMode(DOOR_PIN,INPUT_PULLUP); //Set Pin 10 as Pulled Up Input for door sensor


  Serial.begin(9600);
  keypad.addEventListener(keypadEvent); //add an event listener for this keypad
  passwordState = PASS_NONE;   // Initial Password state
  doorState = DOOR_CLOSED;     // Initial Door state
  cameraState = STOPPED;       // Initial Camera state
  securityState = CLEAR;       // Initial Security state
  doorSensor = DOOR_CLOSED;    // Initial Door sensor state
}



void loop(){

 keypad.getKey();

 doorSensor = digitalRead(DOOR_PIN) == HIGH? DOOR_OPEN: DOOR_CLOSED;
 if (doorState == DOOR_CLOSED && doorSensor == DOOR_OPEN)
 {
   doorState = DOOR_OPEN;

   if (passwordState == PASS_NONE ) // Intrussion detected
   {
       securityState = BROKEN;
       Serial.print("Intrussion detected ");
       Serial.print("CAMREC ");
       //Starts recording
   }
   else {
       securityState = CLEAR;
   }
 }
 if (doorState == DOOR_OPEN && doorSensor == DOOR_CLOSED)
 {
   doorState = DOOR_CLOSED;

   if (passwordState == PASS_OK)
   {
     passwordState = PASS_NONE;
     Serial.print("Autorized Entrance");
     digitalWrite(led1, HIGH);   // turn the LED on (HIGH is the voltage level
    delay(100);               // wait for a 100 mili second
    digitalWrite(led1, LOW);    // turn the LED off by making the voltage LOW
    delay(100);    // wait for a 100 mili second
    digitalWrite(led1, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(100);               // wait for a 100 mili second
    digitalWrite(led1, LOW);    // turn the LED off by making the voltage LOW
    delay(100);    // wait for a 100 mili second

     password.reset();
     Serial.print("Password Cleared");

   }

 }

}

//take care of some special events
void keypadEvent(KeypadEvent eKey){
  switch (keypad.getState()){
    case PRESSED:
	Serial.print("Pressed: ");
	Serial.println(eKey);
	switch (eKey){
	  case '*': checkPassword(); break;
	  case '#': password.reset(); break;
	  default: password.append(eKey);
     }
  }
}

void checkPassword(){

  if (password.evaluate()){

    Serial.println("Success");

    passwordState = PASS_OK;
    digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(100);                // wait for a 100 mili second
    digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
    delay(100);                // wait for a 100 mili second
    password.reset();          //Reset the password
   }
      else{

    Serial.println("Wrong");

  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level
  delay(100);               // wait for a 100 mili second
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  delay(100);    // wait for a 100 mili second
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(100);               // wait for a 100 mili second
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  delay(100);    // wait for a 100 mili second
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(100);               // wait for a 100 mili second
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  delay(100);    // wait for a 100 mili second

  }



}
