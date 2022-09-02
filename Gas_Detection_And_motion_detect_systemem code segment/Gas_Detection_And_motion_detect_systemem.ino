//Blyank
//Template ID,Device Name and Auth Token are provide by the Blyank. Cloud
// See the Device Info tab , or  Template Setting 
#define BLYNK_TEMPLATE_ID "TMPLEtwYAiZ1"
#define BLYNK_DEVICE_NAME "Motion Detect and Gas  leak measuring"
#define BLYNK_AUTH_TOKEN "BoIP5x0fMxFihHZNBsftZ2_K4603gSTL"


//Comment this out to disabl prints and save space 
#define BLYNK_PRINT Serial

//Libraries 
#include<Blynk.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

//Your WIFI Credentails 
char auth[] = "BoIP5x0fMxFihHZNBsftZ2_K4603gSTL ";// Enter your Auth token
char ssid[] = "Dialog 4G 137";//Enter your WIFI SSIS
char pass[] = "BA9D29FF";//Enter your WIFI password

BlynkTimer timer;

//MQ-2 and PIR enable/disable key
int pinValue = 0;
bool pirbutton = 0;
//Light sensor 
int LIGValue=0;

//Define Componets pin
#define MQ2 A0
#define PIR D0
#define Buzzer D5
#define Realy D1
#define LIG D4
#define GreenLED D6
#define RedLED D7

void setup() {
 
 // Setup pins 
  pinMode(GreenLED, OUTPUT);
  pinMode(RedLED, OUTPUT);
  pinMode(Buzzer, OUTPUT);
  pinMode(Realy , OUTPUT);
  pinMode(MQ2, INPUT);
  pinMode(LIG, INPUT);
  pinMode(PIR, INPUT);
  Blynk.begin(auth, ssid, pass);// Setup the connection
 
   //Step a function to be called every second 
   timer.setInterval(100L, gasleveldetector);
   timer.setInterval(100L, pirsensor);
   timer.setInterval(100L, Relay) ;
 

}
//This Functionis called every time the virtual Pin 0 variable  
BLYNK_WRITE(V0) {
  pinValue = param.asInt();
 
}
BLYNK_WRITE(V1) {
  pirbutton = param.asInt();
 
}


void gasleveldetector() //This function  called detect gas leaak lavel  and doing those operation
{
  int sensor = analogRead(MQ2);
  Serial.println(sensor);
  sensor = map(sensor, 0, 1024, 0, 100);
  if (pinValue == 1) {
    if (sensor <= 30) {
      
      digitalWrite(GreenLED, HIGH);
      digitalWrite(RedLED, LOW);
      digitalWrite(Buzzer, LOW);
    
    } else if (sensor > 30) {
     
      Blynk.notify("Warning! Gas leak detected");//Notify  user
      digitalWrite(GreenLED, LOW);
      digitalWrite(RedLED, HIGH);
      digitalWrite(Buzzer, HIGH);
     
    }
      
    Blynk.virtualWrite(V2, sensor);//Update state
  } else {
    digitalWrite(RedLED, LOW);
    digitalWrite(Buzzer, LOW);
    digitalWrite(GreenLED, LOW);
   
  }
  
}

void pirsensor()
{
   bool value = digitalRead(PIR);
   if (pirbutton == 1) {
    if (value == 0) {
       digitalWrite(Buzzer, LOW);
       digitalWrite(GreenLED, HIGH);
       digitalWrite(RedLED, LOW);
  
    } else if (value == 1) {

       Blynk.notify("Warning! Please check your security");
       digitalWrite(Buzzer, HIGH);
       digitalWrite(GreenLED, LOW);
       digitalWrite(RedLED, HIGH);
       
     
    }
     // Blynk.virtualWrite(V1, value);//update state
  }

}

void Relay()
{
  int lig = digitalRead(LIG);
   if (LIGValue == 1) {
    if (lig == 0) {
       digitalWrite(Realy, LOW);
    } 
    else if (lig == 1) {
        digitalWrite(Realy, HIGH);
      
    }
 }
}

void loop() {
  Blynk.run();
  timer.run();
}
