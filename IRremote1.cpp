//hardware connections and code explanation can be found at : https://danielalapat.hashnode.dev/ir-remote-1

#include <IRremote.h>               // initalzing the remote library 

#define onbut 2                     //defining on button as pin 2
#define imbut 4                     //defining regular input button as pin 3
#define hdmi 5                      //defining the tv to hdmi button as pin 5
#define tv 6                        //defining the hdmi to tv button as pin 6

int im=0;                           //to store status of input button
int hd=0;                           //to store status of tv to hdmi button
int tv2=0;                          //to store status of hdmi to tv button

IRsend irsend;                      //initializing library send

void setup() {
  pinMode(onbut, INPUT); 
  pinMode(imbut, INPUT);
  pinMode(hdmi, INPUT);
  pinMode(tv, INPUT);
  attachInterrupt(0,power,RISING); //defining interrupt for power button
}

void loop() {
  im=digitalRead(imbut);            //reading input button
  hd=digitalRead(hdmi);             //reading hdmi to tv button
  tv2=digitalRead(tv);              //reading tv to hdmi button

  if (hd==1){
    for (int i=0;i<6;i++){ 
      irsend.sendNEC(0x20DFD02F , 32); //sending signal 6 times and waiting 1 second 
      delay(1000);}}

  else if (tv==1){
    for (int i=0;i<3;i++){             //sending signal 3 times and waiting 1 second
      irsend.sendNEC(0x20DFD02F , 32);
      delay(1000);}}

  else if (im==1){
    irsend.sendNEC(0x20DFD02F , 32);
  }
  delay(1000);
}

void power(){
  irsend.sendNEC(0x20DF10EF , 32);}     //sending power button signal
