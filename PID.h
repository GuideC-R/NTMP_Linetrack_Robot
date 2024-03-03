#define MaxMotorSpeed 100



// ตัวคำนวณ PID // ห้ามแก้ //

double present_position;
double errors = 0;
double output = 0;
double integral;
double derivative;
double previous_error;

void pid(double Kp , double Kd , int speed_max){
  present_position = readline() / (NUM_SENSOR-1);
  Serial.print(present_position); Serial.println();
  
  errors = setpoint - present_position;
  integral = integral + errors;
  derivative = (errors - previous_error);
  output = Kp * errors + Ki * integral + Kd * derivative;

  previous_error = errors;

  int16_t LeftMotorSpeed = speed_max + output;
  int16_t RightMotorSpeed = speed_max - output;
  
  if (LeftMotorSpeed > MaxMotorSpeed )LeftMotorSpeed = MaxMotorSpeed;
  else if (LeftMotorSpeed < -MaxMotorSpeed)LeftMotorSpeed = -MaxMotorSpeed;

  if (RightMotorSpeed > MaxMotorSpeed )RightMotorSpeed = MaxMotorSpeed;
  else if (RightMotorSpeed < -MaxMotorSpeed)RightMotorSpeed = -MaxMotorSpeed;
  Serial.print(LeftMotorSpeed); 
  Serial.print(' ');
  Serial.print(RightMotorSpeed); 
  Serial.print(' ');


  motor(MOTOR_LEFT_PIN, LeftMotorSpeed);
  motor(MOTOR_RIGHT_PIN, RightMotorSpeed);

  delay(1);
}