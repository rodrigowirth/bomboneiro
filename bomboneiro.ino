#include "HX711.h"
// HX711.DOUT  - pin A0
// HX711.PD_SCK - pin A1
#define ledPin 7
#define lightPin A5
HX711 scale(A1, A0);


const int foutainPin = 13;
const bool replacing = false;

//Times
const long initialDelayForStartReading = 60000;//60 seconds
const long alarmDuration = 15000;//15 seconds
const long changeGaloonWaitingTime = 600000;//10 minutes
int alarmCount = 0;

/*
    FOUNTAIN: Liga e desliga o refrigerados de agua
    LED: Liga e desliga o giroflex
*/

void setup() {
  Serial.begin(9600);

  //Defines pin modes
  pinMode(ledPin, OUTPUT);
  pinMode(foutainPin, OUTPUT);
  pinMode(lightPin, INPUT);

  //Start fountain and led ligados
  digitalWrite(foutainPin, HIGH);
  digitalWrite(ledPin, HIGH);

  //Setup the scale  
  scale.set_scale(20500);
  scale.tare();

  //Turn the led off
  digitalWrite(ledPin, LOW); 
  
  //Delay before start reading
  delay(initialDelayForStartReading);
}

void loop() 
{
  //Gets the weight in kg
  double value = scale.get_units(20) * 1; 
  Serial.print("Weight :");
  Serial.print(value , 2);
  Serial.println("KG");

  //Turn on the giroflex if it should to
  if (value < 0.3)
  {
    alarmCount ++;
    if(alarmCount > 3)
    {
      Serial.println("Giroflex turned on");
      digitalWrite(ledPin, HIGH);
      delay(alarmDuration);
      digitalWrite(ledPin, LOW); 
      Serial.println("Giroflex turned off");
      
      Serial.println("Start waiting time...");
      delay(changeGaloonWaitingTime);

      alarmCount = 0;
    }
  }
  else
  {
    digitalWrite(ledPin, LOW);
    alarmCount = 0;
  }

  //Turn on or off the fountain
  double light = analogRead(lightPin);
  Serial.println("light  \t\t ");
  Serial.print(light , 2);
  if(light > 20)
  {
    Serial.println("Turn on fountain!! ");
    digitalWrite(foutainPin,HIGH);
  }
  else
  {
    Serial.println("Turn off fountain!! ");
    digitalWrite(foutainPin,LOW);
  }

  //Put the ADC in sleep mode
  scale.power_down();
  delay(1000);
  scale.power_up();  

}

