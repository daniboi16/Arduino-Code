#define DT 1                //connect DT to digital pin 1
#define CLK 0               //connect CLK to digital pin 0

int vol=50;                 //variable for volume

int currentCLK;             //variables for rotary encoder
int lastCLK;                //variables for rotary encoder
int currentDT;

void setup() {
  Serial.begin(9600);       //to start serial monitor
  pinMode(DT,INPUT);        //setting pin as input
  pinMode(CLK,INPUT);       //setting pin as input
  lastCLK = digitalRead(CLK);                   //setting last state of CLK pin
}

void loop() {
    currentCLK = digitalRead(CLK);              //get the current state of of CLK pin
    if (currentCLK != lastCLK){                 //if the curent state is not the same as the preivious state it indicates movement  
      currentDT=digitalRead(DT);
      if (currentDT != currentCLK){             //once there is movement we check the other pin to see if it is clockwise of anticlockwise  
        vol = vol - 1;                          //if anticlockwise we decerement the index
        }
      else{                                     //if clockwise we increment the index
        vol = vol + 1;
        }
    Serial.println(vol);                       //display volume
    lastCLK = currentCLK;                      //update last value of CLK
    }
}
