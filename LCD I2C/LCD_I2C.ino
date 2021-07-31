#include <Wire.h>
void setup() {
      Wire.begin();                         // to send the start bit
      delayMicroseconds(100);               // small delay for the LCD to reset

      // Referred from the library which initialises the LCD
      I2C_write4(0b00110000);    
      delayMicroseconds(4200);
      I2C_write4(0b00110000);
      delayMicroseconds(150);
      I2C_write4(0b00110000);
      delayMicroseconds(37);
      I2C_write4(0b00100000);
      delayMicroseconds(37);

      // set the LCD at 4 bit addressing mode  with 2 line mode in 5x7 font
      I2C_write4(0b00101000); 
      delayMicroseconds(37);
      I2C_write8(0b00101000);
      delayMicroseconds(37);


      //command to turn display on cursor off
      I2C_write4(0b00001100);
      delayMicroseconds(37);  
      I2C_write8(0b00001100);
      delayMicroseconds(37); 

      //command to clear the display
      I2C_write4(0b00000001);
      delayMicroseconds(37);  
      I2C_write8(0b00000001);
      delayMicroseconds(1600);

      // Command to increment cursor and shift the cursor to the left automatically
      I2C_write4(0b00000110);
      delayMicroseconds(37);  
      I2C_write8(0b00000110);
      delayMicroseconds(37);

      // to display C
      I2C_write4C(0b01000011);
      delayMicroseconds(37);
      I2C_write8C(0b01000011);
      delayMicroseconds(41);

      // to display A
      I2C_write4C(0b01000001);
      delayMicroseconds(37);
      I2C_write8C(0b01000001);
      delayMicroseconds(41);

      // to display R
      I2C_write4C(0b01010010);
      delayMicroseconds(37);
      I2C_write8C(0b01010010);
      delayMicroseconds(41);
}

void loop() {
}

void I2C_write4(uint8_t data){          // to send the upper 4 bits for commands
  uint8_t d =0b00001000;                // turning backlight on
  d |= (data & 0xF0);                   // to select the upper four bits
  d |= 1<<2;                            // to set the enable bit high
  Wire.beginTransmission(39);           // 39 is the I2C address
  Wire.write(d);                        // sending the data to the device
  delayMicroseconds(1);                 // to send high to low
  d &= ~(1<<2);                         // turning the enable bit high
  Wire.write(d);                        // sending the data to the device
  Wire.endTransmission();               // to send stop bit
}

void I2C_write8(uint8_t data){          // to send the lower 4 bits for commands
  uint8_t d =0b00001000;
  d |= ((data & 0x0F) << 4);            // to select the lower four bits
  d |= 1<<2;
  Wire.beginTransmission(39);
  Wire.write(d);
  delayMicroseconds(1);
  d &= ~(1<<2);
  Wire.write(d);
  Wire.endTransmission();
}

void I2C_write4C(uint8_t data){         // to send upper for bits for data
  uint8_t d =0b00001000;
  d |= (data & 0xF0); 
  d |= 1<<2;  
  d |= 1<<0;                            // setting register select as 1
  Wire.beginTransmission(39);
  Wire.write(d);
  delayMicroseconds(1);
  d &= ~(1<<2);
  Wire.write(d);
  Wire.endTransmission();
}

void I2C_write8C(uint8_t data){        // to send lower for bits for data
  uint8_t d =0b00001000;
  d |= ((data & 0x0F) << 4); 
  d |= 1<<2;
  d |= 1<<0;                          // setting register select as 1
  Wire.beginTransmission(39);
  Wire.write(d);
  delayMicroseconds(1);
  d &= ~(1<<2);
  Wire.write(d);
  Wire.endTransmission();
}
