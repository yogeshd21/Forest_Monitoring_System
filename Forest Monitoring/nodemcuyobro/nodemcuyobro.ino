#include <ESP8266WiFi.h>
#include "DHT.h"
#define DHTPIN 5     // what digital pin we're connected to
#define DHTTYPE DHT11   // DHT 112
#include <Servo.h>

DHT dht(DHTPIN, DHTTYPE);

int trigPin= 10;
int echoPin=9;
float ultra_distance[90]= {0};
float ultra_distance_main[90]= {0};
int angle[91]={0};

Servo my_servo;  // create servo object to control a servo

// IR SENSOR
int irObstaclePin1 = 4;  // This is our input pin
int irObstaclePin2 = 12;  // This is our input pin
int Obstacle1 = HIGH;  // HIGH MEANS NO OBSTACLE
int Obstacle2 = HIGH;  // HIGH MEANS NO OBSTACLE
int count1=0, count2=0,dir;
int inside=0,outside=0;

long duration, distance;

// Wi-Fi Settings
const char* ssid = "I AM"; // your wireless network name (SSID)
const char* password = "thinkofit"; // your Wi-Fi network password

WiFiClient client;

// ThingSpeak Settings
const int channelID =  418688;
String writeAPIKey = "EK5682DSDJCZMN4G"; // write API key for your ThingSpeak Channel
const char* server = "api.thingspeak.com";
const int postingInterval = 20 * 1000; // post data every 20 seconds

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  
  Serial.println("DHTxx test!");
  dht.begin();

  // put your setup code here, to run once:
 // UV SENSOR SCAN
  
  my_servo.attach(14);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  digitalWrite(trigPin,LOW);
  digitalWrite(echoPin,LOW);

// IR SENSOR:
pinMode(irObstaclePin1, INPUT);
pinMode(irObstaclePin2, INPUT);
}

void loop() {
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  if (client.connect(server, 80)) {
    
    // Measure Signal Strength (RSSI) of Wi-Fi connection
    long rssi = WiFi.RSSI();
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 long duration, distance;
 int distance_animal;
     int angle_animal;
  digitalWrite(trigPin, LOW);  
  delayMicroseconds(2); 
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); 
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;
 Obstacle1 = digitalRead(irObstaclePin1);
 Obstacle2 = digitalRead(irObstaclePin2);
 if (Obstacle1 == LOW  || Obstacle2 == LOW)
 {
   Serial.println("OBSTACLE!!, OBSTACLE!!");                         ////uv() starts
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
      int distance_animal=ultra_distance[j];                                ////////////////////////////////////
      int angle_animal=angle[i];
      
    }                                                                       
  }
for(int i=0;i<=36;i++)
{

 ultra_distance[i]=ultra_distance_main[i];

}                                                                           //////////////////////uv() ends here
 }
count1=0, count2=0 ;                                                        /////////////////// ir() strats here
  
 Obstacle1 = digitalRead(irObstaclePin1);
 if (Obstacle1 == LOW)
 {
   Serial.println("OBSTACLE!!, OBSTACLE!!"); 
   count1++; 
   delay(5000);
 }
 Obstacle2 = digitalRead(irObstaclePin2);
 if (Obstacle2 == LOW)
 {
   Serial.println("OBSTACLE!!, OBSTACLE!!"); 
   count2++;
   delay(5000); 
 }
 
// else
// {
//   Serial.println("clear");
// }


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


delay(500);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Construct API request body
    String body = "field1=";
           body += String(rssi);
           body += "&field2=";
           body += String(h);   //humidity variable here
           body += "&field3=";
           body += String(t);  //temperature variable here
           body += "&field4=";
           body += String(hic);  //heat index variable here
           body += "&field5=";
           body += String(distance_animal);  //distance variable here
           body += "&field6=";
           body += String(angle_animal);  //angle variable here
           body += "&field7=";
           body += String(inside); //count of animals going inside boundary
           body += "&field8=";
           body += String(outside);//count of animals going outside boundary
    
    Serial.print("RSSI: ");
    Serial.println(rssi); 
 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    

    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + writeAPIKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(body.length());
    client.print("\n\n");
    client.print(body);
    client.print("\n\n");

  }
  client.stop();

  // wait and then post again
  delay(postingInterval);
}
