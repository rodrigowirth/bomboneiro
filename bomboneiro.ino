#include "HX711.h"
// HX711.DOUT  - pin A0
// HX711.PD_SCK - pin A1
#define ledPin 7
#define lightPin A5
HX711 scale(A1, A0);

int foutainPin = 13;
bool replacing = false;
int initialDelayForStartReading = 60000;//60 seconds
int alarmDuration = 15000;//15 seconds
int changeGaloonWaitingTime = 600000;//10 minutes
int alarmCount = 0;

void setup() {
  // put your setup code here, to run once:
 Serial.begin(9600);

  pinMode(ledPin, OUTPUT);
  pinMode(foutainPin, OUTPUT);
  pinMode(lightPin, INPUT);
  
  digitalWrite(ledPin, HIGH);
  
  Serial.println("HX711 Demo");
  Serial.println("Before setting up the scale:");
  Serial.print("read: \t\t ");
  Serial.println(scale.read());      // print a raw reading from the ADC 
  
  Serial.print("read average:\t\t ");
  Serial.println(scale.read_average(20));       // print the average of 20 readings from the ADC
  
  Serial.print("get value: \t\t ");
  Serial.println(scale.get_value(5));   
// print the average of 5 readings from the ADC minus the tare weight (not set yet)
 
  Serial.print("get units: \t\t ");
  Serial.println(scale.get_units(5), 1);  
// print the average of 5 readings from the ADC minus tare weight (not set) divided by the SCALE parameter (not set yet) </p><p>  scale.set_scale(2280.f);  
 // this value is obtained by calibrating the scale with known weights; see the README for details
  scale.set_scale(20500);
  scale.tare();   // reset the scale to 0
    
 Serial.println("After setting up the scale:");
 
 Serial.print("read: \t\t");
 Serial.println(scale.read());                        // print a raw reading from the ADC
 
 Serial.print("read average:\t\t ");
 Serial.println(scale.read_average(20));       // print the average of 20 readings from the ADC
 
 Serial.print("get value: \t\t ");    
// print the average of 5 readings from the ADC minus the tare weight, set with tare()
Serial.println(scale.get_value(5));
Serial.print("get units: ");
Serial.println(scale.get_units(5), 1);      
// print the average of 5 readings from the ADC minus tare weight, divided by the SCALE parameter set with set_scale
 
Serial.println("Readings:");

digitalWrite(ledPin, LOW); 
delay(initialDelayForStartReading);
  
}

void loop() 
{
  
  /*Serial.print("get units after tare: \t\t ");
  Serial.println(scale.get_units(5));
  Serial.print("read: \t\t ");
  Serial.println(scale.read()); 
  delay(2000);*/
  Serial.print("Weight :");                        
  /* ---------Weight in terms of KG-------------*/
  double value = scale.get_units(20)*1;
  Serial.print(value , 2);
  Serial.println("KG  \t\t ");
  
  if (value < 0.3)
  {
    alarmCount ++;
    if(alarmCount >3)
    {
      digitalWrite(ledPin, HIGH);
      delay(alarmDuration);
      digitalWrite(ledPin, LOW); 
      delay(changeGaloonWaitingTime);
      alarmCount =0;
    }
  }
  else
  {
    alarmCount =0;
    digitalWrite(ledPin, LOW);
  }

  double light = analogRead(lightPin);
  Serial.println("light  \t\t ");
  Serial.print(light , 2);
  if(light > 20)
  {
    digitalWrite(foutainPin,HIGH);
  }
  else
  {
    digitalWrite(foutainPin,LOW);
  }
  
  
  scale.power_down();             // put the ADC in sleep mode
  delay(1000);
  scale.power_up();  
 
}
