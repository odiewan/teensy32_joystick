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

#define AXIS_0_PIN  19
#define AXIS_1_PIN  20
#define AXIS_2_PIN  21
#define AXIS_3_PIN  22

#define LED_MODULO  25

#define BTN_PIN_10  11
#define BTN_PIN_09  10
#define BTN_PIN_08  9
#define BTN_PIN_07  8
#define BTN_PIN_06  7
#define BTN_PIN_05  6
#define BTN_PIN_04  5
#define BTN_PIN_03  4
#define BTN_PIN_02  3
#define BTN_PIN_01  2

#define AIN_MAX 1023
#define AIN_MID 512

bool serialOk;
int tmr;
uint32_t iCount;

int ain00;
int ain01;
int ain02;
int ain03;

bool jAxisX_rev;
bool jAxisY_rev;

bool btn0;
bool btn1;
bool btn2;
bool btn3;
bool btn4;
bool btn5;

bool jBtn1;
bool jBtn2;
bool jBtn3;
bool jBtn4;
bool jBtn5;
bool jBtn6;

int jAxisY;
int jAxisX;

//=============================================================================
void ledToggle() {
  digitalWrite(LED_BUILTIN, !digitalRead((LED_BUILTIN)));
}


//=============================================================================
void setup() {
  serialOk = false;
  iCount = 0;
  tmr = 25;
  jAxisX_rev = true;
  jAxisY_rev = true;
  jAxisX = 0;
  jAxisX = 0;


  btn0 = 0;
  btn1 = 0;
  btn2 = 0;
  btn3 = 0;
  btn4 = 0;
  btn5 = 0;
  jBtn1 = 0;
  jBtn2 = 0;
  jBtn3 = 0;
  jBtn4 = 0;
  jBtn5 = 0;
  jBtn6 = 0;


  pinMode(AXIS_0_PIN, INPUT);
  pinMode(AXIS_1_PIN, INPUT);
  pinMode(AXIS_2_PIN, INPUT);
  pinMode(AXIS_3_PIN, INPUT);

  pinMode(BTN_PIN_10, INPUT);
  pinMode(BTN_PIN_09, INPUT);

  pinMode(BTN_PIN_08, INPUT_PULLUP);
  pinMode(BTN_PIN_07, INPUT_PULLUP);
  pinMode(BTN_PIN_06, INPUT_PULLUP);
  pinMode(BTN_PIN_05, INPUT_PULLUP);
  pinMode(BTN_PIN_04, INPUT_PULLUP);
  pinMode(BTN_PIN_03, INPUT_PULLUP);
  pinMode(BTN_PIN_02, INPUT_PULLUP);
  pinMode(BTN_PIN_01, INPUT_PULLUP);

  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(9600);

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

  if (iCount % LED_MODULO == 0)
    ledToggle();

  // if (iCount % 25 == 0) {
    tmpStr = " ";

    tmpStr += " iC:";
    tmpStr += String(iCount);

    tmpStr += " a0:";
    tmpStr += ain00;

    tmpStr += " a1:";
    tmpStr += ain01;

    tmpStr += " a2:";
    tmpStr += ain02;

    tmpStr += " a3:";
    tmpStr += ain03;

    tmpStr += "| jAxisX:p";
    tmpStr += AXIS_0_PIN;
    tmpStr += " :";
    tmpStr += jAxisX;
    tmpStr += " r:";
    tmpStr += jAxisX_rev;

    tmpStr += " jAxisY:p";
    tmpStr += AXIS_1_PIN;
    tmpStr += " :";
    tmpStr += jAxisY;
    tmpStr += " r:";
    tmpStr += jAxisY_rev;
    serPrnt(tmpStr);

    serPrntV("jBtn1", jBtn1);
    serPrntV("jBtn2", jBtn2);

    serPrntNL(">");

    // }



  }

//=============================================================================
void loop() {
  iCount++;

  jBtn1 = digitalRead(BTN_PIN_01);
  jBtn2 = digitalRead(BTN_PIN_02);
  jBtn3 = digitalRead(BTN_PIN_03);
  jBtn4 = digitalRead(BTN_PIN_04);
  jBtn5 = digitalRead(BTN_PIN_05);
  jBtn6 = digitalRead(BTN_PIN_06);



  ain00 = analogRead(AXIS_1_PIN);
  ain01 = analogRead(AXIS_0_PIN);
  ain02 = analogRead(AXIS_2_PIN);
  ain03 = analogRead(AXIS_3_PIN);

  jAxisX = map(ain00, 0, 1023, 0, 1023);
  jAxisY = map(ain01, 0, 1023, 0, 1023);



  if(jAxisX_rev) {
    jAxisX = AIN_MAX - jAxisX;
  }


  if(jAxisY_rev) {
    jAxisY = AIN_MAX - jAxisY;
  }



  // read analog inputs and set X-Y position
  Joystick.X(jAxisX);
  Joystick.Y(jAxisY);
  Joystick.Z(AIN_MID);
  Joystick.Zrotate(AIN_MID);

  Joystick.slider(AIN_MID);
  Joystick.sliderLeft(AIN_MID);
  Joystick.sliderRight(AIN_MID);

  Joystick.button(1, !jBtn1);
  Joystick.button(2, !jBtn2);
  Joystick.button(3, !jBtn3);
  Joystick.button(4, !jBtn4);
  Joystick.button(5, !jBtn5);
  Joystick.button(6, !jBtn6);

  taskSerial();

  // a brief delay, so this runs 20 times per second
  delay(10);
}
