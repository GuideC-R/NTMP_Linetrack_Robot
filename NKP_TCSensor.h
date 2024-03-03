
// ระบบอ่านเซนเซอร์จับเส้น

#define NUM_SENSOR 8

#define readSensor(index) map(analogRead(_sensorPins[index]), _min_sensor_values[index], _max_sensor_values[index], 0, 1000)

int * _sensorPins = nullptr;
int  _NumofSensor = 0;
int * _min_sensor_values = nullptr;
int * _max_sensor_values = nullptr;
long _lastPosition = 0;

int _Sensitive  = 450;
#define outlineRef 600


// ห้ามแก้ //
void setSensorPins(const int * _pins, int _NumofSensor_){
  _NumofSensor = _NumofSensor_;
  _sensorPins = (int *)realloc(_sensorPins, sizeof(int) * _NumofSensor_);
  _min_sensor_values = (int *)realloc(_min_sensor_values, sizeof(int) * _NumofSensor_);
  _max_sensor_values = (int *)realloc(_max_sensor_values, sizeof(int) * _NumofSensor_);
  for (uint8_t i=0; i < _NumofSensor_ ;i++){
    _sensorPins[i] = _pins[i];
    _min_sensor_values[i] = 4095;
    _max_sensor_values[i] = 0;
  }

}

void setSensorMin(const uint16_t * _MinSensor){
  for (uint8_t i=0; i < _NumofSensor ;i++){
    _min_sensor_values[i] = _MinSensor[i];
  }
}

void setSensorMax(const uint16_t * _MaxSensor){
  for (uint8_t i = 0; i < _NumofSensor; i++)
  {
    _max_sensor_values[i] = _MaxSensor[i];
  }
}

void setSensitive(const uint16_t  _SensorSensitive){
    _Sensitive = _SensorSensitive;
}

void setCalibrate(void){
  for (uint8_t i=0; i < _NumofSensor ;i++){
    if(analogRead(_sensorPins[i]) > _max_sensor_values[i] || _max_sensor_values[i] > 1023 ){
      _max_sensor_values[i]  = analogRead(_sensorPins[i]);
      if(_max_sensor_values[i] > 4095 )_max_sensor_values[i] = 4095;
    }
  }
  for (uint8_t i=0; i < _NumofSensor ;i++){
    if(analogRead(_sensorPins[i]) < _min_sensor_values[i] || _min_sensor_values[i] == 0){
      _min_sensor_values[i] = analogRead(_sensorPins[i]);
      if(_min_sensor_values[i] < 0) _min_sensor_values[i] = 0;
    }
  }
}

int ReadSensorMinValue(uint8_t _Pin){
    return _min_sensor_values[_Pin];
}

int ReadSensorMaxValue(uint8_t _Pin){
    return _max_sensor_values[_Pin];
}

long last_avg = 0;
long last_sum = 0;

long readline(void)
{                
  long avg = 0;
  long sum = 1;
  // Read Sensor
  for (uint8_t i = 0; i < _NumofSensor ;i++){
    long value = map(analogRead(_sensorPins[i]), _min_sensor_values[i], _max_sensor_values[i], 0, 1000);
    // less than 500 = black
    if (value < 500){
      avg += (long)value * (i * 1000);
      sum += value;
      // onLine = true;
    }
  }

  if (avg == 0){
    // Not detect Black
    // Not online [ L or R ]
    // Check from last loop position
    if (_lastPosition/(NUM_SENSOR-1) < 500){
      // turn L
      return out_Right();
    }
    else{
      // turn R
      return out_Left();
    }
    return 500;
  }

  // Normal return
  _lastPosition = avg / sum;
  last_avg = avg;
  last_sum = sum;
  return _lastPosition;
}

// แก้ได้ //

/**
 * Out left of curve -> turn "Right"
 * @param void 
 * @return int : Mid line position
 */
void out_Left(){
  Serial.print(F("----- R ----- ")); Serial.print(_lastPosition/(NUM_SENSOR-1));
  while(true){
    if(analogRead(_sensorPins[2]) < outlineRef || analogRead(_sensorPins[1]) < outlineRef) break;
    motor(1,120);  motor(2,-80);
    delay(1);
  }
  return _lastPosition/(NUM_SENSOR-1);
}

/**
 * Out right of curve -> turn "Left"
 * @param void 
 * @return int : Mid line position
 */
void out_Right(){
  Serial.print(F("----- R ----- ")); Serial.print(_lastPosition/(NUM_SENSOR-1));
  while(true){
    if(analogRead(_sensorPins[2]) < outlineRef || analogRead(_sensorPins[1]) < outlineRef) break;
    motor(1,120);  motor(2,-80);
    delay(1);
  }
  return _lastPosition/(NUM_SENSOR-1);
}