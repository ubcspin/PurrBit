#include <Wire.h> 
byte Version[3];
int8_t x_data;
int8_t y_data;
int8_t z_data;
byte range=0x00;
float divi=16;
float x,y,z;

float x_last;
float y_last;
float z_last;

int LED_pin = 13;
int motor_pin = 5; // pwm

float mspeed = 0; // motor speed

void setup()
{ 
  Serial.begin(9600); 
  Wire.begin(); 
  Wire.beginTransmission(0x0A); // address of the accelerometer 
  // range settings
  Wire.write(0x22); //register address
  Wire.write(range); //can be set at"0x00""0x01""0x02""0x03", refer to Datashhet on wiki
  // low pass filter  
  Wire.write(0x20); //register address
  Wire.write(0x05); //can be set at"0x05""0x04"......"0x01""0x00", refer to Datashhet on wiki
  Wire.endTransmission();
  
  pinMode(LED_pin, OUTPUT);
  pinMode(motor_pin, OUTPUT);
} 
  
void AccelerometerInit() 
{ 
  Wire.beginTransmission(0x0A); // address of the accelerometer 
  // reset the accelerometer 
  Wire.write(0x04); // Y data
  Wire.endTransmission(); 
  Wire.requestFrom(0x0A,1);    // request 6 bytes from slave device #2
  while(Wire.available())    // slave may send less than requested
  { 
    Version[0] = Wire.read(); // receive a byte as characte
  }  
  x_data=(int8_t)Version[0]>>2;
  
  Wire.beginTransmission(0x0A); // address of the accelerometer 
  // reset the accelerometer 
  Wire.write(0x06); // Y data
  Wire.endTransmission(); 
  Wire.requestFrom(0x0A,1);    // request 6 bytes from slave device #2
  while(Wire.available())    // slave may send less than requested
  { 
    Version[1] = Wire.read(); // receive a byte as characte
  }  
  y_data=(int8_t)Version[1]>>2;
   
  Wire.beginTransmission(0x0A); // address of the accelerometer 
  // reset the accelerometer 
  Wire.write(0x08); // Y data
  Wire.endTransmission(); 
  Wire.requestFrom(0x0A,1);    // request 6 bytes from slave device #2
   while(Wire.available())    // slave may send less than requested
  { 
    Version[2] = Wire.read(); // receive a byte as characte
  }  
   z_data=(int8_t)Version[2]>>2; 
   
   x=(float)x_data/divi; 
   y=(float)y_data/divi;
   z=(float)z_data/divi;
   Serial.print("X=");   
   Serial.print(x);         // print the character
   Serial.print("  "); 
   Serial.print("Y=");   
   Serial.print(y);         // print the character
   Serial.print("  "); 
   Serial.print("Z=");           // print the character
   Serial.print(z);
   Serial.print("  ");
   Serial.print("s=");           // print the character
   Serial.print(mspeed);
   Serial.println();

  float threshold = 0.1;
  float dx = abs(x_last - x);
  float dy = abs(y_last - y);
  float dz = abs(z_last - z);
  
   if ( dx > threshold || dy > threshold || dz > threshold) {
      digitalWrite(LED_pin, HIGH);
//      digitalWrite(motor_pin  , HIGH);
//      analogWrite(motor_pin, 255);
   
   } else {
      digitalWrite(LED_pin, LOW);
//      digitalWrite(motor_pin, LOW);
//      analogWrite(motor_pin, 0);
   }
   
   analogWrite(motor_pin, 0); 
   
   x_last = x;
   y_last = y;
   z_last = z;
   
} 

void loop()
{ 
  switch(range)  //change the data dealing method based on the range u've set
  {
  case 0x00:divi=16;  break;
  case 0x01:divi=8;  break;
  case 0x02:divi=4;  break;
  case 0x03:divi=2;  break;
  default: Serial.println("range setting is Wrong,range:from 0to 3.Please check!");while(1); 
  }
  AccelerometerInit(); 
//  analogWrite(motor_pin, (int) mspeed);
//  mspeed = mspeed + 0.1;
//  if (mspeed > 255) {
//    mspeed = 0;
//  }
 delay(10);
}


