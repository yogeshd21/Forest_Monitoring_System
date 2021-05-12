#include <Servo.h>
int trigPin= 12;
int echoPin=13;
float ultra_distance[90]= {0};
float ultra_distance_main[90]= {0};
int angle[91]={0};


Servo my_servo;  // create servo object to control a servo

long duration, distance;

// IR SENSOR
int irObstaclePin1 = 7;  // This is our input pin
int irObstaclePin2 = 8;  // This is our input pin
int Obstacle1 = LOW;  // HIGH MEANS NO OBSTACLE
int Obstacle2 = LOW;  // HIGH MEANS NO OBSTACLE
int count1=0, count2=0,dir;
int inside=0,outside=0;
void setup() {
  // put your setup code here, to run once:
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

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(trigPin, LOW);  
  delayMicroseconds(2); 
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); 
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;
Obstacle1 = digitalRead(irObstaclePin1);
 Obstacle2 = digitalRead(irObstaclePin2);
 if (Obstacle1 == HIGH)
 {
   Serial.println("OBSTACLE!!, OBSTACLE!!"); 
   count1++; 
   delay(5000);
   my_servo.write(0);
  delay(600);
  int j=0;
  int index=-2;

  for (int i=0,j=0;i<=180;i+=5,j++)
  {
    my_servo.write(i);
    delay(50);
    angle[j]= i;
    ultra_distance_main[j]= distance;
    delay(1000);
  }
  for (int i=0,j=0;i<=180;i+=5,j++)
  {
    my_servo.write(i);
    delay(50);
    angle[j]= i;
    ultra_distance[j]= distance;
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

  for (int i=0;i<=36;i++)
  {
    int check= ultra_distance_main[i]-ultra_distance[i];
    if (check<-5|| check>5)       // moved 
    {
      Serial.print("Animal moved away or towards from sensor module");
      int distance_animal=ultra_distance[j];
      int angle_animal=angle[i];
      
    }
  }
 }
  if (Obstacle2 == HIGH)
 {
   Serial.println("OBSTACLE!!, OBSTACLE!!"); 
   count2++;
   delay(5000); 
   my_servo.write(0);
  delay(600);
  int j=0;
  int index=-2;

  for (int i=0,j=0;i<=180;i+=5,j++)
  {
    my_servo.write(i);
    delay(50);
    angle[j]= i;
    ultra_distance_main[j]= distance;
    delay(1000);
  }
  for (int i=0,j=0;i<=180;i+=5,j++)
  {
    my_servo.write(i);
    delay(50);
    angle[j]= i;
    ultra_distance[j]= distance;
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

  for (int i=0;i<=36;i++)
  {
    int check= ultra_distance_main[i]-ultra_distance[i];
    if (check<-5|| check>5)       // moved 
    {
      Serial.print("Animal moved away or towards from sensor module");
      int distance_animal=ultra_distance[j];
      int angle_animal=angle[i];
      
    }
  }
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
