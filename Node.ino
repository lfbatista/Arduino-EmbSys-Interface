/** NODE SCHEDULE **

0. The node is in hibernation mode
1. The node wakes up at the scheduled time in the day before and starts communicating
   with the Base Station. It sends it´s ID.
2. Awaits for the answer.
3. if error in the connection, tries it back after ... minutes/seconds.

4. Receives th information from the Base Station: time, activation time, pin
   and duration.
5. Clock syncronization.
6. Hybernates.
7. Awakes at scheduled once again and activates the pin for the actuator. The action
   lasts as long as defined from the Base Station.
8. Hybernates.

... On the next day the routine repeats.

*/

/** LIBRARIES **/

/** WEEK DAYS **/
const int SUNDAY = 1;
const int MONDAY = 2;
const int TUESDAY = 3;
const int WEDNESDAY = 4;
const int THURSDAY = 5;
const int FRIDAY = 6;
const int SATURDAY = 7;

/** VARIABLES **/
float program[7][8];
int weekDay = 0;
float duration;

/** PINS **/
const int pinLED = 13;
int pin;
int count = 0;

//  In this function, Serial Port is initiated, pinLED defined as output to use the board LED
//  The first head of the program array is filled in according each week day
void setup(){
  pinMode(pinLED, OUTPUT);
  Serial.begin(9600);
  
  for (int i=0; i<7 ; i++){
    program[i][0] = i+1;
  }
}


void loop(){
  //Empties buffer
  getMessReceived();          

  //Calls for the Base Station
  Serial.print("Node01");

  delay(5000);
  count ++;
  
  //Gets Base Station answer
  String message;
  while (message ==""){
    message = getMessReceived();
  }
      
  if (message == "BSDataNode01"){
    //HAND-SHAKE
    Serial.print("-OK");
    delay(5000);
    
    //Stores data received from the Base Station in the array "program"
    setData();  
 
    //Lights up the boar LED for five seconds to indicate that data is stored
    digitalWrite(pinLED,HIGH);
    delay(5000);
    digitalWrite(pinLED,LOW);
    
    //Waits. Waiting period determinated according to Base Station instructions
    duration = program[weekDay-1][1]*60*60*1000 + program[weekDay-1][2]*60*1000-((count - 1) * 5000 + 16000);
    delay(duration);
    
    //Starts actuator. The pin is choosen according to Base Station instructions
    //    as well as the time it lasts.
    //During Activation period, the board LED is light up indicating the order
    pin = program[weekDay-1][5];
    pinMode(pin, OUTPUT);  
    digitalWrite(pin,HIGH);
    digitalWrite(pinLED,HIGH);
    duration = program[weekDay-1][6]*60*60*1000 + program[weekDay-1][7]*60*1000;
    delay(duration);
    digitalWrite(pin,LOW);
    digitalWrite(pinLED,LOW);
    
    //Waits. Waiting period determinated according to Base Station instructions
    duration = (program[weekDay-1][3]-program[weekDay-1][1]-program[weekDay-1][6])*60*60*1000 + (program[weekDay-1][4]-program[weekDay-1][2]-program[weekDay-1][7])*60*1000;
    delay(duration);

    //Resets count variable    
    count = 0;
  }
}

//Method to read Serial Port
String getMessReceived(){
  String stringMess;
  while (Serial.available() > 0){
    char mess=Serial.read();
    if (mess > 0) stringMess += String(mess);
  }  
  return stringMess;
}

void setData(){

  //The board LED blinks indicating that the storing procedure is about to begin
  blinkLED();
  
  //Gets the Base Station data
  String message;
  while (message == ""){
    message = getMessReceived();
  }
  
  //Synchronization
  String sWeekDay = message.substring(0,1);
  weekDay = sWeekDay.toInt();
    
  //Saves the Data:
  //Day
  String sDay = message.substring(0,1);
  int day = sDay.toInt();
 
  //Alarm 1
  String sA1H = message.substring(2,4);
  program[day-1][1] = sA1H.toInt();
  String sA1M = message.substring(4,6);
  program[day-1][2] = sA1M.toInt();

  //Alarm 2
  String sA2H = message.substring(7,9);
  program[day-1][3] = sA2H.toInt();
  String sA2M = message.substring(9,11);
  program[day-1][4] = sA2M.toInt();

  //Pin
  String sPin = message.substring(12,14);
  program[day-1][5] = sPin.toInt();
  
  //Duration of actuator
  String sDurH = message.substring(15,17);
  program[day-1][6] = sDurH.toInt();
  String sDurM = message.substring(17,19);
  program[day-1][7] = sDurM.toInt();
}

//Blinking method
void blinkLED(){
  for(int i=0;i<4;i++){
    digitalWrite(pinLED,HIGH);
    delay(100);
    digitalWrite(pinLED,LOW);
    delay(100);
  }  
}

