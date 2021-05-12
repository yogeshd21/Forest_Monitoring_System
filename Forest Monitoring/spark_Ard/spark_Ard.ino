// UV SENSOR SCAN
#include <Servo.h>
#include "DHT.h"
#define DHTPIN 5    // what digital pin we're connected to
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
int trigPin= 12;
int echoPin=13;
float ultra_distance[90]= {0};
float ultra_distance_main[90]= {0};
int angle[91]={0};


Servo my_servo;  // create servo object to control a servo

// IR SENSOR
int irObstaclePin1 = 7;  // This is our input pin
int irObstaclePin2 = 8;  // This is our input pin
int Obstacle1 = HIGH;  // HIGH MEANS NO OBSTACLE
int Obstacle2 = HIGH;  // HIGH MEANS NO OBSTACLE
int count1=0, count2=0,dir;
int inside=50;
int outside=0;

int measure_distance() {
  long duration, distance;
  digitalWrite(trigPin, LOW);  
  delayMicroseconds(2); 
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); 
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;
  return distance;
}




void setup() {
Serial.println("DHTxx test!");

  dht.begin();
 Serial.println("Sensor Data\n \n\n");
 // UV SENSOR SCAN
  
  my_servo.attach(9);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  digitalWrite(trigPin,LOW);
  digitalWrite(echoPin,LOW);

// IR SENSOR:
pinMode(irObstaclePin1, INPUT);
pinMode(irObstaclePin2, INPUT);
 Serial.begin(9600);  
}
void mysens(){
   // Wait a few seconds between measurements.
  delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(f);
  Serial.print(" *F\t");
  Serial.print("Heat index: ");
  Serial.print(hic);
  Serial.print(" *C ");
  Serial.print(hif);
  Serial.println(" *F");
  
}
void uv_detect()
{
  int maximum=0;
  int distance_animal;
  int angle_animal;
   my_servo.write(0);
  delay(600);
  int j=0;
  int index=-2;

  for (int i=0,j=0;i<=180;i+=5,j++)
  {
    my_servo.write(i);
    delay(50);
    angle[j]= i;
    ultra_distance_main[j]= measure_distance();
    delay(1000);
  }
  for (int i=0,j=0;i<=180;i+=5,j++)
  {
    my_servo.write(i);
    delay(50);
    angle[j]= i;
    ultra_distance[j]= measure_distance();
    delay(1000);
  }


  for (int i=0;i<=36;i++)
  {
    Serial.print( ultra_distance_main[i]-ultra_distance[i]);
    Serial.print("\t");
    if(i%6==0){
      Serial.print("\n");
    }
  }

  Serial.print("\n");

  for (int i=0;i<=36;i++)
  {
    int check= ultra_distance_main[i]-ultra_distance[i];
    if (check<-5|| check>5)       // moved 
    {
      if(ultra_distance[i]>maximum){
      maximum= ultra_distance[i];
      distance_animal=ultra_distance[i];
      angle_animal=angle[i];
      }
            
    }
  }
  Serial.print(distance_animal);
  Serial.print("\n");
  Serial.print(angle_animal);
  Serial.print("\n");
  Serial.print("\n");

  my_servo.write(angle_animal); 
}


void loop() 
{
  mysens();
 count1=0;
 count2=0;
 Obstacle1 = digitalRead(irObstaclePin1);
 Obstacle2 = digitalRead(irObstaclePin2);
 if (Obstacle1 == LOW)
 {
   Serial.println("OBSTACLE!!, OBSTACLE!!"); 
   count1++; 
   delay(5000);
   uv_detect();
 }

 if (Obstacle2 == LOW)
 {
   Serial.println("OBSTACLE!!, OBSTACLE!!"); 
   count2++;
   delay(5000); 
   uv_detect();
 }

 else
{
   Serial.println("CLEAR");
}


dir=count1-count2;
if (dir > 0)
{
  Serial.println("Entering Region");
  inside++;
  outside--;
}



 if (dir < 0)
{
  Serial.println("Exiting Region");
  outside++;
  inside--;
}

Serial.println("Inside the region");
Serial.println(inside);
Serial.println("Outside the region");
Serial.println(outside);



}
