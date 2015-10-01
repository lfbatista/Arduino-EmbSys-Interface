/** BASE STATION **

0. 24 on 24 Listening.

1. Receives a call from a node.
2. Checks it Identity and makes a HAND-SHAKE.
3. Receives confirmation about the HAND-SHAKE.
3. It sends then the data concerning Synch, time for turning on the actuator 
   pin for using and duration.

4. Returns to listening mode.

*/

/** WEEK DAYS **/
const int SUNDAY = 1;
const int MONDAY = 2;
const int TUESDAY = 3;
const int WEDNESDAY = 4;
const int THURSDAY = 5;
const int FRIDAY = 6;
const int SATURDAY = 7;

/** VARIABLES **/
int weekDay = 1;

//  In this function, Serial Port is initiated
void setup(){
  Serial.begin(9600);
}

void loop() {
  //Base Station is listening
  String node = getMessReceived();
  String confirmationMess = "BSData";

  //If something is detected
  if (node != ""){
    confirmationMess += node;
    Serial.print(confirmationMess);
    delay(5000);
  }
  
  //If the node "is in data base  
  if (node == "Node01"){// Node from DB Server
    delay(3000);
    String checkMessage;
    while (checkMessage == ""){
      checkMessage = getMessReceived();
    }
    
    //Check HAND-SHAKE
    if (checkMessage == node + "-OK"){
      nodeData(node,weekDay);
    }
  }
}

//Method to read Serial Port
String getMessReceived(){
  String stringMess;
  while (Serial.available() > 0){
    char cha=Serial.read();
    if (cha > 0) stringMess += String(cha);
  }
  return stringMess;
}

//Method which receives the information from DB Server and sends it to the node
//Simulation!!
void nodeData(String node, int thisWeekDay){

    switch (thisWeekDay){
      case SUNDAY:
        Serial.print("1A0003A0010P10D0003S0000");
        delay(5000);
        break;
      case MONDAY:
        Serial.print("2A0002A0010P10D0004S0000");
        delay(5000);
        break;
      case TUESDAY:
        Serial.print("3A0004A0010P10D0003S0000");
        delay(5000);
        break;
      case WEDNESDAY:
        Serial.print("4A0002A0010P10D0005S0000");
        delay(5000);
        break;
      case THURSDAY:
        Serial.print("5A0003A0010P10D0005S0000");
        delay(5000);
        break;
      case FRIDAY:
        Serial.print("6A0002A0010P10D0004S0000");
        delay(5000);
        break;
      case SATURDAY:
        Serial.print("7A0003A0010P10D0002S0000");
        delay(5000);
        break;
    }
  // The week day is increased in here. In the final product, such information comes from the DB server.
  weekDay ++;
  if (weekDay > 7) weekDay -= 7; 
}

/** DATA WITHIN THE STRING **
  FIRST NUMBER: day of the week;
  A FROM FIRST ALARM:  first two digits are hours,
                       next two digits are minutes,
                       Time Lapse till ring and activates the irrigation;
  A FROM SECOND ALARM: first two digits are hours,
                       next two digits are minutes,
                       Time Lapse till ring and contacts the base station;
  P FROM PIN: number of the pin to be activated;
  D FROM DURATION:     first two digits are hours,
                       next two digits are minutes,
                       duration of the pin activation;
  S FROM SYNCH:        first two digits are hours,
                       next two digits are minutes,
                       data to proceed with the node synch.

*/
