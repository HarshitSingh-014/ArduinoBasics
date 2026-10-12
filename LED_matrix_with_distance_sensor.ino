// this programme uses a LCD display to show velocity of the obstruction , a proxitmity sensor and a led matrix , where we can move the LED light 
// if we keep the obstruction between 30 and 15 centimeter , the led moves in y direction but moves in x if the obstruction if closer than 15 centimeter 
// it also playes a buzzer sound when the object changes it's region...
//in future versions i will be adding a humidity sensor for accurate predcition of velocity by more accurate speed of sound



#include <LedControl.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);


const int trigPin = 9;
const int echoPin = 10;
const int buzp = 4;
long duration;
float distance;
int state = 2;
float velocity = 0;

// DIN = 12, CLK = 11, CS = 6, 1 matrix
LedControl matrix = LedControl(12, 11, 6, 1);
long total_delay=0;
float prev_dist=0;
int coords[] = {0,0};

void setup() {

  lcd.init();
  lcd.backlight();

  Serial.begin(9600);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzp, OUTPUT);
  matrix.shutdown(0, false);  // Turn on matrix
  matrix.setIntensity(0, 8);  // Brightness (0-15)
  matrix.clearDisplay(0); 
  matrix.setLed(0, coords[0],coords[1], true);
      // Clear all LEDs
}

void loop() {
  // Turn ON all LEDs
  
  if(coords[1]>=8){
    coords[1]=-1;
  }
  if(coords[0]>=8){
    coords[0]=-1;
  }
  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH, 30000);

  if (duration == 0) {
    

    if (state!=-1){
      digitalWrite(buzp,HIGH);
      delay(200);
      digitalWrite(buzp,LOW);
      state=-1;
    }
    
    Serial.println("No echo detected");



  } else {

    
    
    distance = duration * 0.0343 / 2.0;

    matrix.setLed(0, 0, 0, false);
    matrix.setLed(0, coords[0], coords[1], false);



    if(distance >15 && distance <=30){
      if (state!=1){
      
      digitalWrite(buzp,HIGH);
      delay(200);
      digitalWrite(buzp,LOW);
      state=1;
      total_delay+=200;
    }
    
      
      if(coords[1]==7){
          coords[1]=0;
      }
      else{
        coords[1]=coords[1]+1;
      }
      
      
    matrix.setLed(0, coords[0], coords[1], true);
      
    }

    else if(distance >0 && distance <=15){
      if (state!=0){
      digitalWrite(buzp,HIGH);
      delay(200);
      digitalWrite(buzp,LOW);
      state=0;
      total_delay+=200;
    }
    
      
      if(coords[0]==7){
          coords[0]=0;
      }
      else{
        coords[0]=coords[0]+1;
      }
      
      matrix.setLed(0, coords[0], coords[1], true);
      
    }
    else if(distance >30){



      if (state!=2){
      digitalWrite(buzp,HIGH);
      delay(200);
      digitalWrite(buzp,LOW);
      state=2;
      total_delay+=200;
    }
      
    matrix.setLed(0, 0, 0, true);
    }


    
    velocity=(distance - prev_dist)/(duration/1000000+total_delay/1000);
    
    prev_dist= distance;

    lcd.clear();
    lcd.print("Velocity : ");
    lcd.setCursor(0,1);
    lcd.print(velocity);

    Serial.print("Velocity ");
    Serial.println(velocity);
   
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.print(" cm");
    Serial.print("  PREV_Distance: ");
    Serial.print(prev_dist);
    Serial.println(" cm");
    delay(100);
  }

  





  
}
