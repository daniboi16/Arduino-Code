//hardware connections and code explanation can be found at : https://danielalapat.hashnode.dev/tide-callender

//including liabray for the RTC
#include <Wire.h>
#include <RTClib.h>

//rotary encoder definition
#define CLK 2
#define DT 3
#define SW 4                                  //defining the reset pin

RTC_DS1307 rtc;                               //initiallising the RTC

//variables to store the current date and time
int chour=0;
int cminute=0;
int cday=0;
int cmonth=0;
int cyear=0;
int cweekday=0;

//array to find the day of the week
char week[7][12]= {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

//database
float table[3][10]={{2,4,140,1.31,713,0.76,1149,1.00,1835,0.13},{3,4,220,1.30,801,0.71,1242,0.97,1919,0.20},{4,4,300,1.28,850,0.67,1338,0.93,2001,0.29}};

int currentstateCLK;           //variables for rotary encoder
int laststateCLK;              //variables for rotary encoder
int refresh=0;                 //refreshes when it becomes when it is 1
int index=0;                   //stores the value of the index of the database which we are to display
int lowt1,lowt2,hight1,hight2; //curent 4 parameter veriables times
float low1,low2,high2,high1;   //current 4 parameter variable values

void setup() {
  Serial.begin(9600);         //setting up serial monitor

  pinMode(SW, INPUT);         //initialzing reset pin as input
  pinMode(CLK, INPUT);        //initialzing encoder as input
  pinMode(DT, INPUT);         //initialzing encoder as input

  laststateCLK=digitalRead(CLK);
  attachInterrupt(0,update,CHANGE);
  attachInterrupt(1,update,CHANGE);

  //code to set up RTC
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    abort();}
  if (! rtc.isrunning()) {
    Serial.println("setting the time");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));}


  refresh_time();                     //getting the current time from the RTC
  finding();                          //finding the index 
}

void loop() {
  refresh=digitalRead(SW);            //waing for reset pin to be pushed
    if (refresh==HIGH){
      refresh_time();
      finding();      
    }
    delay(1000);

}

void refresh_time(){
  //function to get the current date and time from RTC and update variables
  DateTime now = rtc.now();
  chour=now.hour();
  cminute=now.minute();
  cday=now.day();
  cmonth=now.month();
  cyear=now.year();
  cweekday=now.dayOfTheWeek();
}

void finding(){
  //finding the index of the database which corresponds to the current date and time
  for (int i=0;i<3;i++){                                // 3 should be changed 
    if ((table[i][1]==cmonth) && (table[i][0]==cday)){
      index=i+1;}
      }
  if (index == 0){
    Serial.print("not found");
  }
  else{
  assign();}}                                            // after we find the index we can assign the variables

void assign(){
  //functino to assign all the varibles dependign on the index value
  lowt1=table[index-1][2];
  low1=table[index-1][3];
  hight1=table[index-1][4];
  high1=table[index-1][5];
  lowt2=table[index-1][6];
  low2=table[index-1][7];
  hight2=table[index-1][8];
  high2=table[index-1][9];
  display_info();}                                        //after assigning the varibales we can display the info

void display_info(){
  //functino to display the data
  Serial.print("date:  ");
  Serial.print(cday);
  Serial.print(" / ");
  Serial.print(cmonth);
  Serial.print(" / ");
  Serial.print(cyear);
  Serial.print("  ");
  Serial.print(week[cweekday]);
  Serial.print("\n");
  Serial.print("time:  ");
  Serial.print(chour);
  Serial.print("  :  ");
  Serial.print(cminute);
  Serial.print("\n");
  Serial.print(lowt1);
  Serial.print("  ");
  Serial.print(low1);
  Serial.print("\n");
  Serial.print(hight1);
  Serial.print("  ");
  Serial.print(high1);
  Serial.print("\n");
  Serial.print(lowt2);
  Serial.print("  ");
  Serial.print(low2);
  Serial.print("\n");
  Serial.print(hight2);
  Serial.print("  ");
  Serial.print(high2);
  Serial.print("\n");
}

void update(){
  //function for rotary encoder
  currentstateCLK = digitalRead(CLK);
  if (currentstateCLK != laststateCLK && currentstateCLK ==1){
    if (digitalRead(DT) != currentstateCLK){
      index --;}
    else{
      index ++;}
  }
  laststateCLK = currentstateCLK;
  if ((index>0) && (index<4)){                            //4 has to be changed
    assign();}
  else{
    Serial.print("out of bounds");
  }
}
