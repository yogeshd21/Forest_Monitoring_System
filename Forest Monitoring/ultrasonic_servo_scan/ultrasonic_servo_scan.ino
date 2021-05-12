#include <Servo.h>
int trigPin= 12;
int echoPin=13;
float ultra_distance[90]= {0};
float ultra_distance_main[90]= {0};
int angle[91]={0};
//int angle_main[91]={0};

Servo my_servo;  // create servo object to control a servo

int measure_distance() {
  long duration, distance;
  digitalWrite(trigPin, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(trigPin, HIGH);
//  delayMicroseconds(1000); - Removed this line
  delayMicroseconds(10); // Added this line
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;
  return distance;
}

//void distance_template(){
//  my_servo.write(0);
//  delay(600);
//  int j=0;
//  int index=-2;
//
//  for (int k=0;k<=180;k+=5)
//  {
//    my_servo.write(k);
//    delay(50);
//    angle[j]= k;
//    ultra_distance_main[j]= measure_distance();
//    delay(1000);
//  }
//  
//}


void setup() {
  // put your setup code here, to run once:
  my_servo.attach(9);
  //my_servo.write(600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  digitalWrite(trigPin,LOW);
  digitalWrite(echoPin,LOW);
//  distance_template();
  Serial.begin(9600);
  
}

void loop() {
  // put your main code here, to run repeatedly:
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


  for(int i=0;i<=36;i++)
{

 ultra_distance[i]=ultra_distance_main[i];

}

  

}







