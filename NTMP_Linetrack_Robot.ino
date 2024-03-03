#include <Arduino.h>

#define MOTOR_LEFT_PIN  1
#define MOTOR_RIGHT_PIN 2

double Kp = 0.18;
// #define Ki 0.00001
double Kd = 1.219;
int MaxSpeed = 100;
double setpoint = 500.0;

#include "NKP_TCSensor.h"
#include "PID.h"

#define Press_OK()  {Serial.println(F("Press OK")); sw_OK_press();}

//#define Ki 0
unsigned long RoundTime = 0;

void AdjustPID(double Ap, double Ad, double Aspeed, double Asetpoint);
void StartMission();

void setup(){
  Serial.begin(115200);
  Serial.println();
  Serial.println(F("Line Tracking"));
  sound(1500,200);

  // Init Sensor PIN
  setSensorPins((const int[]) {
   A0, A1, A2, A3, A4, A5, A6, A7
  }, NUM_SENSOR);

  BTCommand_Init(); // Init bluetooth PID config

  Serial.println(F("End setup"));

  // ไว้ใส่ code calibrate ค่าสี
  sw_OK_press();
  // setSensorMin((const int[]) { 100,  100,  100,  100,  100,  100,  100,  100});
  // setSensorMax((const int[]) {1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000});
  sound(1500,200);

  // รอกดเริ่มออกตัว
  sw_OK_press();
  sound(1500,200);
  delay(800);
  RoundTime = millis();
  StartMission();
  RoundTime = millis()-RoundTime;
  AO();
  // วิ่งจบ
}

void StartMission(){
  // Serial.println(F("StartMission"));

  // Smooth start
  for(int x=50; x>=0 ;x--) {motor(12,x);  delay(1);} AO();

  // วิ่งไป เลี้ยวซ้าย ขวา ใส่ตรงนี้
  while(true){
    PID();
  }
}

void AdjustPID(double Ap, double Ad, double Aspeed, double Asetpoint){
  Kp = Ap;
  Kd = Ad;
  MaxSpeed = Aspeed;
  setpoint = Asetpoint;
}

void loop() {}