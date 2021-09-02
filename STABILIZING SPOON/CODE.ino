//importing libaries required
#include <Wire.h>     //  libary to comunicated with the sesnor through I2C protocol
#include <Servo.h>    //  libary to control the servo motors
#include <Console.h>  //  libary to get data from gyroscope

//creating objects 
Servo ser;      //  object for servo motor 1
Servo ser1;     //  object for servo motor 2

//initalizing variables
int s2 = 7;    
int s2a;
int a = 3,b;
float x = 0, y = 0, z = 0;                    //initialzing x,y,z values of possision
int16_t rx, ry, rz;                           //initialzing using 16 bit values for the rotational values
float gyroXoffset, gyroYoffset, gyroZoffset;  //initilazing variable for gyroscope
float accX, accY, accZ, gyroX, gyroY, gyroZ;  //initilazing variable for gyroscope and accelorometer
float angleGyroX, angleGyroY, angleGyroZ,angleAccX, angleAccY, angleAccZ; //initilazing variable for gyroscope and accelorometer
float angleX, angleY, angleZ;                 //initializing vaiables for angles of rotation
float accCoef = 0.02f;                        //initialzing value from datasheet 
float gyroCoef = 0.98f;                       //initialzing value from datasheet 
float interval;                               //initialzing to set a sampling rate
long preInterval;                             //initialzing to get values from sensor
long gX, gY, gZ;                              //initialzing values for gravitational movement
float roX, roY, roZ;                          //initialzing values for rotation
float rgx, rgy, rgz, rax, ray, raz, temp;     //initialzing values for rotation for gyroscope and accelrometer


//to set up the micro controller
void setup() {
 //for initializing the sensor
 Wire.begin();        //to start I2C comunication
 setupMPU();          //to set up registers inside the gyroscope
 cgo();               //to get values from the gyroscope for x,y,z which will used as refernec x,y,z

 //for initialzing the servo motor
 ser.attach(9);       //using digital pin 9 of the arduino for servomotor1
 ser.write(90);       //setting the motor to its intital possion (alliging the tips)
 ser1.attach(10);     //using digital pin 10 of the arduino for servomotor2
 ser1.write(90);      //setting the motor to its intital possion (alliging the tips)
 
 delay(100);          //0.1 second delay to let the servo motor move
}

// this fucntion will keep happeneing in loop for ever
void loop() {
 updatemp();      //this function calculates the current possision of the spoon and calculates how much it has been moved and in what direction
 b = angleY;      //this is a value we need to move the seriver motor 2
 s2a = angleX;    //this is a value we need to move the seriver motor 2 by
 ser1.write(90 + (b));    //function to move the servo motor2
 ser.write(90 + (s2a));   //function to move the servo motor1
}

//this function is to set up the sensor, this can be found in the datasheet of the sensor
void setupMPU(){
  //sensor is cumicated with I2C protocal there is 3 parts to sending data across in I2C  
  //first we have to send the adress of the sensor followed by the address of the register
  //we need read/write, followed by the data we need to read/write. to end transmission we neeed to send a stop bit
  
  //to set 0x6B register to 1
  Wire.beginTransmission(0b1101000);  //this is to send the I2C adress of the device
  Wire.write(0x6B);                   //this is the register in the sensor we want to set up (8B)
  Wire.write(0b00000001);             //this is the value of the register we want to set
  Wire.endTransmission();             //to send stop bit

  //to set 0x1B register to 8
  Wire.beginTransmission(0b1101000);
  Wire.write(0x1B);
  Wire.write(0x08);
  Wire.endTransmission();

  //to set 0x1c register to 0
  Wire.beginTransmission(0b1101000);
  Wire.write(0x1C);
  Wire.write(0x00);
  Wire.endTransmission();

  //to set 0x19 register to 0
  Wire.beginTransmission(0b1101000);
  Wire.write(0x19);
  Wire.write(0x00);
  Wire.endTransmission();

  //to set 0x1A register to 0
  Wire.beginTransmission(0b1101000);
  Wire.write(0x1A);
  Wire.write(0x00);
  Wire.endTransmission();
  
  updatemp();               //to get current ofset
  angleGyroX = 0;
  angleGyroY = 0;
  angleX = angleAccX;
  angleY = angleAccY;
  preInterval = millis();   //returns the number of seconds since the start up 
}


//this function is only called in the beginnning it is so that we can get the initial posision and orientation of the sensor
//this could be though of the calibration of the device, the spoon will use the values got during this funntion as the reference 0,0,0
void cgo(){
  for(int i = 0; i < 3000; i++) //we need to run this 3000 times to get best values
  {
    //the folowing 3 lines are w.r.t the I2C protocal
    Wire.beginTransmission(0b1101000);        //to sent the I2C adress of the sensor
    Wire.write(0x3B);                         // to write to the sensor the value 0x3B which enable the sensor
    Wire.endTransmission();                   //to send the stop bit to end communication
    Wire.requestFrom(0b1101000,14);           //to wait for the sensor to awknlodge this command  
    while(Wire.available() < 14);             //polling to wait for the sensor to send the data

    //the following lines of code is to read data from the sensor, with I2C protocal we are limited to 8 bits at a time
    //the first (8*4) bits are not required so we dont store it
    Wire.read()<<8|Wire.read();
    Wire.read()<<8|Wire.read();
    Wire.read()<<8|Wire.read();
    Wire.read()<<8|Wire.read();
    rx = Wire.read()<<8|Wire.read();
    ry = Wire.read()<<8|Wire.read();
    rz = Wire.read()<<8|Wire.read();

    //once we get the values from the sensor we get set up our refernece ground, this x,y and z will serve as our refernec 
    //this is all repeated 3000 times to get the best values and take into accound error
    x += ((float)rx) / 65.5;            
    y += ((float)ry) / 65.5;
    z += ((float)rz) / 65.5;
    } 
  co();                           //caling the cuntion to calculate ofset from refernece
}


//this function calculates ofset from the refernece and then corrects the motors to calculate for that
void co(){
 gyroXoffset = x / 3000;
 gyroYoffset = y / 3000;
 gyroZoffset = z / 3000;
 delay(3000);
 }
 

//this is the main function which calculates our movement and makes the nessasory movement for the servo motors
void updatemp(){

  //follwoing 4 lines is related to I2C protocol
  Wire.beginTransmission(0b1101000);    //to send the adress of the sensor
  Wire.write(0x3B);                     //to sent the value 0x3B to the sensor which will start the transmission
  Wire.endTransmission();               //to send the stop bit
  Wire.requestFrom(0b1101000,14);       //to read the data from the sensor on to the I2C bus
  
  while(Wire.available()<14);           //waiting for the sensor to awkledge our request
  
  //the follwing 7 lines read data from the sensor and store in the respective variable using I2C we are limited to 8 bits at a time
  rax = Wire.read()<<8|Wire.read();
  ray = Wire.read()<<8|Wire.read();
  raz = Wire.read()<<8|Wire.read();
  temp = Wire.read()<<8|Wire.read();
  rgx = Wire.read()<<8|Wire.read();
  rgy = Wire.read()<<8|Wire.read();
  rgz = Wire.read()<<8|Wire.read();

  //the follwing lines are mathematical calulations of the sensor and its possion which corresponds to how much it is moved and how much to move
  //the serovo motor finally it returns the 2 values whih is the yaw and pitch which is then sent to the servo motors
  temp = (temp + 12412.0) / 340.0; 
  accX = ((float)rax) / 16384.0;
  accY = ((float)ray) / 16384.0;
  accZ = ((float)raz) / 16384.0;
  angleAccX = atan2(accY, accZ + abs(accX)) * 360 / 2.0 / PI;
  angleAccY = atan2(accX, accZ + abs(accY)) * 360 / -2.0 / PI;
  gyroX = ((float)rgx) / 65.5;
  gyroY = ((float)rgy) / 65.5; 
  gyroZ = ((float)rgz) / 65.5;
  gyroX -= gyroXoffset;
  gyroY -= gyroYoffset;
  gyroZ -= gyroZoffset;
  interval = (millis() - preInterval) * 0.001;
  angleGyroX += gyroX * interval;
  angleGyroY += gyroY * interval;
  angleGyroZ += gyroZ * interval;
  angleX = (gyroCoef * (angleX + gyroX * interval)) + (accCoef * angleAccX);
  angleY = (gyroCoef * (angleY + gyroY * interval)) + (accCoef * angleAccY);
  angleZ = angleGyroZ;
  preInterval = millis();
}
