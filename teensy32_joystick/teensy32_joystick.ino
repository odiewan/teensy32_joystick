/* Basic USB Joystick Example
   Teensy becomes a USB joystick

   You must select Joystick from the "Tools > USB Type" menu

   Pushbuttons should be connected to digital pins 0 and 1.
   Wire each button between the digital pin and ground.
   Potentiometers should be connected to analog inputs 0 to 1.

   This example code is in the public domain.
*/
#include <Arduino.h>
#include <serialPrint.h>
bool serialOk = false;
int tmr = 0;
uint32_t iCount = 0;
bool btn2;
bool btn3;
bool btn4;
bool btn5;

bool jBtn1;
bool jBtn2;
bool jBtn3;
bool jBtn4;

//=============================================================================
void ledToggle() {
  digitalWrite(LED_BUILTIN, !digitalRead((LED_BUILTIN)));
  }


//=============================================================================
void setup() {
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(9600);
  tmr = 25;

  while (tmr > 0 && serialOk) {
    ledToggle();
    if (Serial)
      serialOk = true;

    delay(250);

    tmr--;
    }

  Serial.println("Serial Port OK");


  }

//=================================================================================================
void taskSerial() {
  String tmpStr = "";
  double _cmd;

  if (iCount % 125 == 0) {



    tmpStr = " ";

    tmpStr += " iC:";
    tmpStr += String(iCount);

    tmpStr += " btn2:";
    tmpStr += btn2;

    tmpStr += " btn3:";
    tmpStr += btn3;

    tmpStr += " btn4:";
    tmpStr += btn4;

    tmpStr += " btn5:";
    tmpStr += btn5;

    tmpStr += ">";
    serPrntNL(tmpStr);

    }
  }

//=============================================================================
void loop() {
  iCount++;
  // // read analog inputs and set X-Y position
  // Joystick.X(analogRead(0));
  // Joystick.Y(analogRead(1));

  // read the digital inputs and set the buttons
  btn2 = digitalRead(2);
  btn3 = digitalRead(3);
  btn4 = digitalRead(4);
  btn5 = digitalRead(5);

  if (btn2 == false || btn3 == false || btn4 == false || btn5 == false)
    ledToggle();

  Joystick.button(1, btn2);
  Joystick.button(2, btn3);
  Joystick.button(3, btn4);
  Joystick.button(4, btn5);

  taskSerial();

  // a brief delay, so this runs 20 times per second
  delay(50);
  }

