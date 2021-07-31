//hardware connections and code explanation can be found at : https://danielalapat.hashnode.dev/ir-remote-2

#define IRLed 3                                               //pin connected to the IR LED
unsigned long input_code=0b00100000110111111101000000101111;  //this is the code for the input button of the remote
unsigned long power_code=0b00100000110111110001000011101111;  //this is the code for the power button of the remote

void setup() {
  pinMode(IRLed, OUTPUT);                                     //initializing the IR led as an output
}

void loop() {
  data_signal(power_code);
  delay(5000);
  for (int i=0;i<6;i++){
    data_signal(input_code);
    delay(3000);
  }
  delay(10000);
}

void carrier_signal(unsigned int t_micro){                    //function to turn on and of the LED correspoding to the carrier frequency
  for (int i=0; i<(t_micro / 26); i++){                       //26 because 1/38==26, 38 is the carrier frequency
    digitalWrite(IRLed, HIGH);
    delayMicroseconds(9);                                     //should be 13 but the function takes 3micro seconds to excicute which has to be compensated
    digitalWrite(IRLed, LOW);
    delayMicroseconds(9);                                     //should be 13 but the function takes 3micro seconds to excicute which has to be compensated
  }
}

void data_signal(unsigned long code){
  carrier_signal(9000);                                       //starting burst of 9ms
  delayMicroseconds(4500);                                    //wait time of 4.5ms

  int time_period=562;                                        //time for each one and each zero
  for (int j=0;j<32;j++){                                     //for each of the 32 bits in the signal code
    carrier_signal(time_period);                              //first bit is always low
      if (code & 0x80000000){                                 //mask to get msb, if bit is 1
        delayMicroseconds(3*time_period);                     //for high signal we need to send high signal 3 times
      }
      else{                                                   // if bit is 0
        delayMicroseconds(time_period);                       //for low signal we need to send the signal only once
      }
   code<<=1;                                                  //to bit shift to the next byte
  }
  carrier_signal(time_period);                                //seding stop bit
}
