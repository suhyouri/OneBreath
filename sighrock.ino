#include <SoftwareSerial.h>
#define Motor1Pin_S 2 //Start Motor_1 
#define Motor2Pin_S 3 //Start Motor_2~
#define Motor1Pin_F 4 //Finish Motor_3 
#define Motor2Pin_F 5 //Finish Motor_4~
#define Sol1Pin_V 7 //Finish Motor_3 
#define Sol2Pin_V 6 //Finish Motor_4~

//const int pin1 =  9;
//const int pin2 =  10;

unsigned long previousMillis = 0;        
long interval = 1000;
long randomNumber;

#include <NewPing.h>

#define TRIGGER_PIN_L A0
#define ECHO_PIN_L A1
#define TRIGGER_PIN A2
#define ECHO_PIN A3
#define TRIGGER_PIN_R A4
#define ECHO_PIN_R A5
#define MAX_DISTANCE 200


#define LED_BOTTOM 9
#define LED_TOP 10
#define LED_INTERVAL 50
unsigned long before_millis = 0;

//LED
int brightness_top = 0;         
int brightness_bottom = 0;         
float fadeAmount = 15;

//Smoothing 
const int numReadings = 10;
int readings_l[numReadings];      // the readings from the analog input
int readings[numReadings];      // the readings from the analog input
int readings_r[numReadings];      // the readings from the analog input
int readIndex_l = 0;              // the index of the current reading
int readIndex = 0;              // the index of the current reading
int readIndex_r = 0;              // the index of the current reading
int total_l = 0;                  // the running total
int total = 0;                  // the running total
int total_r = 0;                  // the running total
int dist_average_l = 0;
int dist_average = 0;
int dist_average_r = 0;

// NewPing setup of pins and maximum distance
NewPing sonar_L(TRIGGER_PIN_L, ECHO_PIN_L, MAX_DISTANCE);
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); 
NewPing sonar_R(TRIGGER_PIN_R, ECHO_PIN_R, MAX_DISTANCE);  

void setup() {
  Serial.begin(9600);
  pinMode(LED_BOTTOM, OUTPUT);
  pinMode(LED_TOP, OUTPUT);
  pinMode(Motor1Pin_S, OUTPUT);             
  pinMode(Motor2Pin_S, OUTPUT);             
  pinMode(Motor1Pin_F, OUTPUT);             
  pinMode(Motor2Pin_F, OUTPUT);             
  pinMode(Sol1Pin_V, OUTPUT);             
  pinMode(Sol2Pin_V, OUTPUT); 
  
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
   } 
}

void loop() {

  // 1. motor running 
  if (previousMillis + interval <= millis()) {
    randomNumber = random(0,3);
    interval = random(1000,3000);
    previousMillis = millis();
  }
  
  switch(randomNumber)
      {
        case 0: 
            Motor_inhale();
//            digitalWrite(pin1, HIGH);
//            digitalWrite(pin2, LOW);
//            //check
//            Serial.print("MOTOR A + previousMillis: ");
//            Serial.print(previousMillis); 
//            Serial.print("interval: "); 
//            Serial.println(interval); 
        break;
        case 1:
            Motor_exhale();
//            digitalWrite(pin1, LOW);
//            digitalWrite(pin2, HIGH);
//            //check
//            Serial.print("MOTOR B + previousMillis: ");
//            Serial.print(previousMillis); 
//            Serial.print("interval: "); 
//            Serial.println(interval);
        break;
        case 2: 
            DO_NOTHING();
//            digitalWrite(pin1, LOW);
//            digitalWrite(pin2, LOW);
//            //check
//            Serial.print("MOTOR C + previousMillis: ");
//            Serial.print(previousMillis); 
//            Serial.print("interval: "); 
//            Serial.println(interval);
        break;
   } 

   //2. LED on/off with dist sensor
   if( before_millis + LED_INTERVAL <= millis() ){
       unsigned int distance = sonar.ping_cm();
       unsigned int distance_L = sonar_L.ping_cm();
       unsigned int distance_R = sonar_R.ping_cm();
//       Serial.print("Left: ");
//       Serial.print(distance_L);
//       Serial.print("cm, ");
//       Serial.print("Mid: ");
//       Serial.print(distance);
//       Serial.print("cm, ");
//       Serial.print("Right: ");
//       Serial.print(distance_R);
//       Serial.println("cm");

      // (1) Average : Left 
      total_l = total_l - readings_l[readIndex_l];
      readings_l[readIndex_l] = distance_L;
      total_l = total_l + readings_l[readIndex_l];
      readIndex_l = readIndex_l + 1;
      if (readIndex_l >= numReadings) {
        readIndex_l = 0;
      }
      dist_average_l = total_l / numReadings;

      // (2) Average : Center
      total = total - readings[readIndex];
      readings[readIndex] = distance;
      total = total + readings[readIndex];
      readIndex = readIndex + 1;
      if (readIndex >= numReadings) {
        readIndex = 0;
      }
      dist_average = total / numReadings;

      // (3) Average : Right
      total_r = total_r - readings_r[readIndex_r];
      readings_r[readIndex_r] = distance_R;
      total_r = total_r + readings_r[readIndex_r];
      readIndex_r = readIndex_r + 1;
      if (readIndex_r >= numReadings) {
        readIndex_r = 0;
      }
      dist_average_r = total_r / numReadings;   

       Serial.print("Left: ");
       Serial.print(dist_average_l);
       Serial.print("cm, ");
       Serial.print("Mid: ");
       Serial.print(dist_average);
       Serial.print("cm, ");
       Serial.print("Right: ");
       Serial.print(dist_average_r);
       Serial.println("cm");

        // -------> Audience closer 
       if(dist_average_l < 7 || dist_average < 7 || dist_average_r < 7){
        // 1. digital test 
    //    digitalWrite(LED_BOTTOM, HIGH);
    //    digitalWrite(LED_TOP, LOW);
    
        // 2. analog test 
        analogWrite(LED_BOTTOM, brightness_bottom);
        brightness_bottom = brightness_bottom + fadeAmount;
        analogWrite(LED_TOP, brightness_top);
        brightness_top = brightness_top - fadeAmount;
          
          if (brightness_top >= 255) {
              brightness_top = 255;
          }else if(brightness_top <= 0){
              brightness_top = 0;
          }    
          if (brightness_bottom >= 255) {
              brightness_bottom = 255;
           }else if(brightness_bottom <= 0){
              brightness_bottom = 0;
          }     
        Serial.print("brightness top: ");
        Serial.println(brightness_top);
        Serial.print(", brightness bottom : ");
        Serial.println(brightness_bottom);
        
        }else if(dist_average_l > 6 || dist_average > 6 || dist_average_r > 6){
        // No Audience 
        
        // 1. digital test   
    //      digitalWrite(LED_BOTTOM, LOW);
    //      digitalWrite(LED_TOP, HIGH);
    
        // 2. analog test 
          analogWrite(LED_BOTTOM, brightness_bottom);
          brightness_bottom = brightness_bottom - fadeAmount;
          analogWrite(LED_TOP, brightness_top);
          brightness_top = brightness_top + fadeAmount;
          
          if (brightness_top >= 255) {
            brightness_top = 255;
          }else if(brightness_top <= 0){
            brightness_top = 0;
          }
          
          if (brightness_bottom >= 255) {
            brightness_bottom = 255;
           }else if(brightness_bottom <= 0){
            brightness_bottom = 0;
          }
          
          Serial.print("brightness top: ");
          Serial.println(brightness_top);
          Serial.print(", brightness bottom : ");
          Serial.println(brightness_bottom);
    
        }else{
          Serial.println("Hmm");
        }
        before_millis = millis();
    }       
}

void Motor_inhale(){
  digitalWrite(Motor1Pin_S, HIGH);        
  digitalWrite(Motor2Pin_S, LOW);
  digitalWrite(Motor1Pin_F, LOW);         
  digitalWrite(Motor2Pin_F, LOW);
  digitalWrite(Sol1Pin_V, LOW);         
  digitalWrite(Sol2Pin_V, LOW);
}

void Motor_exhale(){
  digitalWrite(Motor1Pin_S, LOW);        
  digitalWrite(Motor2Pin_S, LOW);
  digitalWrite(Motor1Pin_F, HIGH);         
  digitalWrite(Motor2Pin_F, LOW);
  digitalWrite(Sol1Pin_V, HIGH);         
  digitalWrite(Sol2Pin_V, LOW);
}

void DO_NOTHING(){
  digitalWrite(Motor1Pin_S, LOW);        
  digitalWrite(Motor2Pin_S, LOW);
  digitalWrite(Motor1Pin_F, LOW);         
  digitalWrite(Motor2Pin_F, LOW);
  digitalWrite(Sol1Pin_V, LOW);         
  digitalWrite(Sol2Pin_V, LOW);
}
