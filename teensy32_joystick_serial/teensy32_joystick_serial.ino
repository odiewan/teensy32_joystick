//---set usb type as flight sim controls + joystick
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <varObj.h>
#include <ods_util.h>
#include <serialPrint.h>
#include <PWMServo.h>
#define SER_DLY           250
#define LOOP_DLY          20
#define DEF_BAUD          115200

#define AXIS_0_PIN  14
#define AXIS_1_PIN  15
#define AXIS_2_PIN  16
#define AXIS_3_PIN  17

#define BTN0_PIN    2
#define BTN1_PIN    3
#define BTN2_PIN    4
#define BTN3_PIN    5
#define BTN4_PIN    6
#define BTN5_PIN    7
#define BTN6_PIN    8


#define LED_MODULO  25
#define SER_OUT_MODULO  50

#define AIN_MAX 1023
#define AIN_MID 512


bool serialOk;
int tmr;
uint32_t iCount;
bool prntHex;
int ain01;

bool btn0;
bool btn1;
bool btn2;
bool btn3;
bool btn4;
bool btn5;
bool btn6;

bool btn4led;

bool btn0Shadow;
bool btn1Shadow;
bool btn2Shadow;
bool btn3Shadow;
bool btn4Shadow;
bool btn5Shadow;
bool btn6Shadow;

uint8_t sld_led0;
uint8_t sld_led1;

uint8_t btn0_cnt;
uint8_t btn1_cnt;
uint8_t btn2_cnt;
uint8_t btn3_cnt;
uint8_t btn4_cnt;
uint8_t btn5_cnt;
uint8_t btn6_cnt;

varObj vo00;
varObj vo01;
varObj vo02;
varObj vo03;

int btnCount;

bool useVJoy;
bool useServo;
bool useThrottleServo;
bool printBtns;

PWMServo svo;
int svoPos;

//=============================================================================
void setup() {
  svo.attach(2, 1000, 2000);
  ain01 = 0;
  svoPos = 90;

  serialOk = false;
  iCount = 0;
  tmr = 25;
  prntHex = true;
  useVJoy = true;
  printBtns = false;
  btnCount = 0;


  btn0 = false;
  btn1 = false;
  btn2 = false;
  btn3 = false;
  btn4 = false;
  btn5 = false;
  btn6 = false;

  btn4led = false;

  btn0Shadow = false;
  btn1Shadow = false;
  btn2Shadow = false;
  btn3Shadow = false;
  btn4Shadow = false;
  btn5Shadow = false;
  btn6Shadow = false;

  btn0_cnt = 0;
  btn1_cnt = 0;
  btn2_cnt = 0;
  btn3_cnt = 0;
  btn4_cnt = 0;
  btn5_cnt = 0;
  btn6_cnt = 0;

  pinMode(AXIS_0_PIN, INPUT);
  pinMode(AXIS_1_PIN, INPUT);
  pinMode(AXIS_2_PIN, INPUT);
  pinMode(AXIS_3_PIN, INPUT);

  pinMode(BTN0_PIN, INPUT_PULLUP);
  pinMode(BTN1_PIN, INPUT_PULLUP);
  pinMode(BTN2_PIN, INPUT_PULLUP);
  pinMode(BTN3_PIN, INPUT_PULLUP);
  pinMode(BTN4_PIN, INPUT_PULLUP);
  pinMode(BTN5_PIN, INPUT_PULLUP);
  pinMode(BTN6_PIN, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);


  Serial.begin(DEF_BAUD);

  while (tmr > 0 && serialOk) {
    ledToggle();
    if (Serial)
      serialOk = true;

    delay(SER_DLY);

    tmr--;
  }

  Serial.println("Serial Port OK");

  vo00 = varObj(V_TYP_UINT10, true, -7);
  vo01 = varObj(V_TYP_UINT10, false, 9);
  vo02 = varObj(V_TYP_UINT10, true, 1);
  vo03 = varObj(V_TYP_UINT10, false, 7);

  Serial.println("vo00:" + vo00.getBC());
  Serial.println("vo01:" + vo01.getBC());
  Serial.println("vo02:" + vo02.getBC());
  Serial.println("vo03:" + vo03.getBC());



  ledToggle();
  delay(1000);
  Serial.println("Setup done");
}


//=================================================================================================
void handleSerIn() {
  String inStr = recvWithEndMarker();
  if (inStr > "") {

    if (inStr == "vj") {
      useVJoy = !useVJoy;
      serPrnt("ic:" + String(iCount) + ":");
      serPrntNL("toggle between serial and joystick mode");
      }

    else if (inStr == "h") {
      prntHex = !prntHex;
      serPrnt("ic:" + String(iCount) + ":");
      serPrntNL("toggle print in hex");
      }
    else if (inStr == "s") {
      useServo = !useServo;
      if (useServo)
        useThrottleServo = false;
      serPrnt("ic:" + String(iCount) + ":");
      serPrntNL("toggle servo out en");
      }
    else if (inStr == "th") {
      useThrottleServo = !useThrottleServo;

      if (useThrottleServo)
        useServo = false;
      serPrnt("ic:" + String(iCount) + ":");
      serPrntNL("toggle throttle servo out");
      }
    else {
      serPrnt("ic:" + String(iCount) + ":");
      serPrnt(":" + inStr);
      serPrntNL(":unhandled cmd");
      }

    }
  }

//=================================================================================================
void taskSerialOut() {
  String tmpStr = "";

  if (iCount % SER_OUT_MODULO == 0)

    Serial.print(F("tjs iC:"));
    Serial.print(String(iCount));

    Serial.print(" b:" + (String)btn0 + (String)btn1 + (String)btn2 +(String)btn3);
    Serial.print((String)btn4 + (String)btn5 + (String)btn6);


    if(prntHex){
      Serial.print(F(" a0:"));
      Serial.print(vo00.getVal(), HEX);

      Serial.print(F(" a1:"));
      Serial.print(vo01.getVal(), HEX);

      Serial.print(F(" a2:"));
      Serial.print(vo02.getVal(), HEX);

      Serial.print(F(" a3:"));
      Serial.print(vo03.getVal(), HEX);
    }
    else {
      Serial.print(F(" a0:"));
      Serial.print(vo00.getVal());

      Serial.print(F(" a1:"));
      Serial.print(vo01.getVal());

      Serial.print(F(" a2:"));
      Serial.print(vo02.getVal());

      Serial.print(F(" a3:"));
      Serial.print(vo03.getVal());
    }

    serPrntNL();


  }

//=============================================================================
void taskTelemOut() {
  String tmpStr = "";
  if (iCount % 1 == 0) {
    tmpStr += "A55A";
    tmpStr += "_a0:" + String(vo00.getVal());
    tmpStr += "_a1:" + String(vo01.getVal());
    tmpStr += "_a2:" + String(vo02.getVal());
    tmpStr += "_a3:" + String(vo03.getVal());
    tmpStr += "_b0:" + String(btn0);
    tmpStr += "_b1:" + String(btn1);
    tmpStr += "_b2:" + String(btn2);
    tmpStr += "_b3:" + String(btn3);
    tmpStr += "_b4:" + String(btn4);
    tmpStr += "_b5:" + String(btn5);
    tmpStr += "_b6:" + String(btn6);
    tmpStr += "B66B";
    Serial.println(tmpStr);
    }
  }

//=============================================================================
void taskDigRead() {

  btn0 = digitalRead(BTN0_PIN) ? false : true;
  btn1 = digitalRead(BTN1_PIN) ? false : true;
  btn2 = digitalRead(BTN2_PIN) ? false : true;
  btn3 = digitalRead(BTN3_PIN) ? false : true;
  btn4 = digitalRead(BTN4_PIN) ? false : true;
  btn5 = digitalRead(BTN5_PIN) ? false : true;
  btn6 = digitalRead(BTN5_PIN) ? false : true;



  btn0Shadow = btn0;
  btn1Shadow = btn1;
  btn2Shadow = btn2;
  btn3Shadow = btn3;
  btn4Shadow = btn4;
  btn5Shadow = btn5;
  btn6Shadow = btn6;
  btn4Shadow = btn4;

  }

//=============================================================================
void taskAnalogRead() {

  vo00.pushVal(analogRead(AXIS_0_PIN));
  vo01.pushVal(analogRead(AXIS_1_PIN));

  vo02.pushVal(analogRead(AXIS_2_PIN));
  vo03.pushVal(analogRead(AXIS_3_PIN));
}

//=============================================================================
void taskDigWrite() {
  // digitalWrite(BTN4_LED_PIN, btn4led);
}

//=============================================================================
void taskAnalogWrite() {

}
//=============================================================================
void taskHandle_js_out() {
  Joystick.X(vo01.getVal());
  Joystick.Y(vo00.getVal());
  Joystick.Z(vo02.getVal());
  Joystick.Zrotate(vo03.getVal());
  Joystick.slider(AIN_MID);
  Joystick.sliderLeft(AIN_MID);
  Joystick.sliderRight(AIN_MID);
  Joystick.sliderLeft(AIN_MID);
  Joystick.sliderRight(AIN_MID);



  Joystick.button(1, btn0);
  Joystick.button(2, btn1);

  Joystick.button(3, btn2);
  Joystick.button(4, btn3);

  Joystick.button(5, btn4);
  Joystick.button(6, btn5);
  Joystick.button(7, btn6);
}

//=============================================================================
void doMixing() {
  // if (vo02.Val > 650 && vo02.Val < 750) {
  //   sld_led0 = 255;
  //   sld_led1 = 255;
  //   }
  // else {
  //   sld_led0 = 0;
  //   sld_led1 = 0;
  //   }
  // if (useServo)
  //   svoPos = map((float)ain01, 215, 880, 10, 170);
  // else if (useThrottleServo)
  //   svoPos = map((float)vo00.getVal(), 215, 880, 10, 170);

  }

//=============================================================================
void loop() {
  iCount++;
  if (iCount % LED_MODULO == 0)
    ledToggle();

  taskAnalogRead();
  taskDigRead();
  handleSerIn();


  taskAnalogWrite();
  taskDigWrite();

  if(useVJoy) {
    taskTelemOut();
    }
  else {
    taskHandle_js_out();
    taskSerialOut();
  }

  svo.write(svoPos);
  // a brief delay, so this runs 20 times per second
  delay(LOOP_DLY);
}
