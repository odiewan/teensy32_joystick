//---set usb type as flight sim controls + joystick
#include <Arduino.h>
#include <serialPrint.h>

#define AXIS_0_PIN  14
#define AXIS_1_PIN  15
#define AXIS_2_PIN  16
#define AXIS_3_PIN  17
#define BTN0_PIN    2
#define BTN1_PIN    3
#define BTN2_PIN    4
#define LED0_PIN    8
#define LED1_PIN    9
// #define BTN_PIN_09  10
// #define BTN_PIN_08  9
// #define BTN_PIN_07  8
// #define BTN_PIN_06  7
// #define BTN_PIN_05  6
// #define BTN_PIN_04  5
// #define BTN_PIN_03  4
// #define BTN_PIN_02  3
// #define BTN_PIN_01  2

#define AIN_MAX 1023
#define AIN_MID 512

#define LED_MODULO  25


int tmr;

bool serialOk;
uint32_t iCount;

int ain00;
int ain01;
int ain02;
int ain03;
int ain04;
int ain05;
// int ain06;

bool jAxisX_rev;
bool jAxisY_rev;
bool jAxisZ_rev;

bool btn0;
bool btn1;
bool btn2;
bool btn3;
bool btn4;
bool btn5;

bool led0;
bool led1;

bool jBtn1;
bool jBtn2;
bool jBtn3;
bool jBtn4;
bool jBtn5;
bool jBtn6;

int jAxisY;
int jAxisX;
int jAxisZ;

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
  jAxisY_rev = false;
  jAxisZ_rev = true;
  jAxisX = 0;
  jAxisY = 0;
  jAxisZ = 0;


  btn0 = 0;
  btn1 = 0;
  btn2 = 0;
  btn3 = 0;
  btn4 = 0;
  btn5 = 0;
  led0 = 0;
  led1 = 0;

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

  pinMode(BTN0_PIN, INPUT_PULLUP);
  pinMode(BTN1_PIN, INPUT_PULLUP);
  pinMode(BTN2_PIN, INPUT_PULLUP);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED0_PIN, OUTPUT);
  pinMode(LED1_PIN, OUTPUT);



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
    tmpStr = " teensy3.2 joystick";

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

    // tmpStr += " a4:";
    // tmpStr += ain04;

    // tmpStr += " a5:";
    // tmpStr += ain05;

    // tmpStr += " a6:";
    // tmpStr += ain06;


    // tmpStr += " a3:";
    // tmpStr += ain03;

    // tmpStr += "| jX:p";
    // tmpStr += AXIS_0_PIN;
    // tmpStr += " :";
    // tmpStr += jAxisX;
    // tmpStr += " r:";
    // tmpStr += jAxisX_rev;

    // tmpStr += " jY:p";
    // tmpStr += AXIS_1_PIN;
    // tmpStr += " :";
    // tmpStr += jAxisY;
    // tmpStr += " r:";
    // tmpStr += jAxisY_rev;

    // tmpStr += " jZ:p";
    // tmpStr += AXIS_2_PIN;
    // tmpStr += " :";
    // tmpStr += jAxisZ;
    // tmpStr += " r:";
    // tmpStr += jAxisY_rev;
    serPrnt(tmpStr);

    serPrntV("jBtn1", jBtn1);
    serPrntV("jBtn2", jBtn2);
    serPrntV("jBtn3", jBtn3);

    serPrntNL(">");

    // }



  }

//=============================================================================
void loop() {
  iCount++;

  btn0 = !digitalRead(BTN0_PIN);
  btn1 = !digitalRead(BTN1_PIN);
  btn2 = !digitalRead(BTN2_PIN);
  // jBtn3 = digitalRead(BTN_PIN_03);
  // jBtn4 = digitalRead(BTN_PIN_04);
  // jBtn5 = digitalRead(BTN_PIN_05);
  // jBtn6 = digitalRead(BTN_PIN_06);



  ain00 = analogRead(AXIS_0_PIN);
  ain01 = analogRead(AXIS_1_PIN);
  ain02 = analogRead(AXIS_2_PIN);
  ain03 = analogRead(AXIS_3_PIN);

  led0 = btn1;
  led1 = btn2;


  jAxisX = map(ain00, 0, 1023, 0, 1023);
  jAxisY = map(ain01, 0, 1023, 0, 1023);
  jAxisZ = map(ain02, 0, 1023, 0, 1023);


  if(jAxisX_rev) {
    jAxisX = AIN_MAX - jAxisX;
  }


  if(jAxisY_rev) {
    jAxisY = AIN_MAX - jAxisY;
  }

  if(jAxisZ_rev) {
    jAxisZ = AIN_MAX - jAxisZ;
  }

  jBtn1 = btn0;
  jBtn2 = btn1;
  jBtn3 = btn2;



  digitalWrite(LED0_PIN, led0);
  digitalWrite(LED1_PIN, led1);

  // read analog inputs and set X-Y position
  Joystick.X(jAxisX);
  Joystick.Y(jAxisY);
  Joystick.Z(jAxisZ);
  Joystick.Zrotate(AIN_MID);

  Joystick.slider(AIN_MID);
  Joystick.sliderLeft(AIN_MID);
  Joystick.sliderRight(AIN_MID);

  Joystick.button(1, jBtn1);
  Joystick.button(2, jBtn2);
  Joystick.button(3, jBtn3);
  // Joystick.button(3, jBtn3);
  // Joystick.button(4, jBtn4);
  // Joystick.button(5, jBtn5);
  // Joystick.button(6, jBtn6);

  taskSerial();

  // a brief delay, so this runs 20 times per second
  delay(20);
}
