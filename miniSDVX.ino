#include <MouseTo.h>
#include <Mouse.h>
#include <Encoder.h>
#include <Keyboard.h>

//Button pins
#define BTN_A 2
#define BTN_B 3
#define BTN_C 4
#define BTN_D 5
#define BTN_START 6
#define FX_L 7
#define FX_R 8
#define BTN_FX_L 20
#define BTN_FX_R 19

//Button binds
#define BIND_A 'd'
#define BIND_B 'f'
#define BIND_C 'j'
#define BIND_D 'k'
#define BIND_START KEY_RETURN
#define BIND_FX_L 'c'
#define BIND_FX_R 'm'
#define BIND_BTN_FX_L KEY_F4
#define BIND_BTN_FX_R 234


int BTN_S[] = {BTN_A,BTN_B,BTN_C,BTN_D,BTN_START,BTN_FX_L,BTN_FX_R,FX_L,FX_R};
byte BTN_BIND[] = {BIND_A,BIND_B,BIND_C,BIND_D,BIND_START,BIND_BTN_FX_L,BIND_BTN_FX_R,BIND_FX_L,BIND_FX_R};

int ledBtn = 18;

int Lclk = 14;
int Ldt = 15;

int Rclk = 10;
int Rdt = 16;

int LEDLeft = 21;
int LEDRight = 9;

int btnCount = sizeof(BTN_S) / sizeof(BTN_S[0]);

int lastCLKR;
int currentCLKR;

int lastCLKL;
int currentCLKL;

bool LEDState;
bool isPressLED;

Encoder encL(Ldt,Lclk);
Encoder encR(Rdt,Rclk);

int previousMillisL = 0;
int previousMillisR = 0;

int interval = 40;



void setup() {
  for(int i=0; i < btnCount; i++){
    pinMode(BTN_S[i],INPUT_PULLUP);
  }
  pinMode(BTN_FX_L, INPUT_PULLUP);
  pinMode(ledBtn, INPUT_PULLUP);

  pinMode(Lclk, INPUT);
  pinMode(Ldt, INPUT);
  pinMode(Rclk, INPUT);
  pinMode(Rdt, INPUT);

  pinMode(LEDLeft, OUTPUT);
  pinMode(LEDRight, OUTPUT);
  
  Serial.begin(9600);

  Keyboard.begin();
  Mouse.begin();
  lastCLKR = encR.read();
  lastCLKL = encL.read();
}

void loop() {
  if(digitalRead(ledBtn) == LOW){
    if(isPressLED == false){
      if(LEDState){
        LEDState = false;
      }else{
        LEDState = true;
      }
      isPressLED = true;
    }
  }else{
    isPressLED = false;
  }
  
  if(encR.read() != lastCLKR){
      if(encR.read() > lastCLKR){
        Mouse.move(0,15);
      }else{
        Mouse.move(0,-15);
      }
    lastCLKR = encR.read();
  }
  
  if(encL.read() != lastCLKL){
    if(encL.read() > lastCLKL){
      Mouse.move(10,0);
    }else{
      Mouse.move(-10,0);
    }
    lastCLKL = encL.read();
  }

  if(LEDState){
    if(digitalRead(BTN_A) == LOW || digitalRead(BTN_B) == LOW){
      digitalWrite(LEDLeft, HIGH);
    }else{
      digitalWrite(LEDLeft, LOW);
    }
    if(digitalRead(BTN_C) == LOW || digitalRead(BTN_D) == LOW || digitalRead(FX_R) == LOW){
      digitalWrite(LEDRight, HIGH);
    }else{
      digitalWrite(LEDRight, LOW);
    }
    if(digitalRead(FX_R) == LOW){
      int currentMillisR = millis();

      if(currentMillisR < 0){
        currentMillisR = (currentMillisR * -1);
      }
      int trig = currentMillisR - previousMillisR;
      if(trig < 0){
        trig = trig * -1;
      }
      if (trig >= interval) {
          previousMillisR = currentMillisR;
          if (digitalRead(LEDRight) == LOW) {
            digitalWrite(LEDRight, HIGH);
          } else {
            digitalWrite(LEDRight, LOW);
          }
        }
        
    }
    if(digitalRead(FX_L) == LOW){
      int currentMillisL = millis();
      if(currentMillisL < 0){
        currentMillisL = (currentMillisL * -1);
      }
      int trig = currentMillisL - previousMillisL;
      if(trig < 0){
        trig = trig * -1;
      }
      if (trig >= interval) {
          previousMillisL = currentMillisL;
          if (digitalRead(LEDLeft) == LOW) {
            digitalWrite(LEDLeft, HIGH);
          } else {
            digitalWrite(LEDLeft, LOW);
          }
      }
        
    }
    
    
    
  }
    
  
  for(int i = 0; i < btnCount; i++){
    if(digitalRead(BTN_S[i]) == LOW){
      Keyboard.press(BTN_BIND[i]);
    }else{
      Keyboard.release(BTN_BIND[i]);
    }
  }
}
