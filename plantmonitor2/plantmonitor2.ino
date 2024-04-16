#define BLYNK_TEMPLATE_ID "TMPLZAQBZS7q"
#define BLYNK_TEMPLATE_NAME "Plant Monitoring System"
#define BLYNK_AUTH_TOKEN "9QIyPHmvw7Z_dXYsqw-Na9YcKnY4pmlR"

//Include the library files
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

char auth[] = BLYNK_AUTH_TOKEN;     //Enter your Auth token
char ssid[] = "AndroidAP_4850";     //Enter your WIFI name
char pass[] = "00000000";           //Enter your WIFI password

BlynkTimer timer;
bool Relay = 0;

//Define component pins
#define sensor A0
#define waterPump D1
#define DHTPIN D4                    //Connect Out pin to D4 in NODE MCU
#define DHTTYPE DHT11  

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  pinMode(waterPump, OUTPUT);
  digitalWrite(waterPump, HIGH);
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 8080);
  dht.begin();
  //Call the function
  timer.setInterval(100L, soilMoistureSensor);
  
}

//Get the button value
BLYNK_WRITE(V3) {
  Relay = param.asInt();

  if (Relay == 1) {
    digitalWrite(waterPump, LOW);
    
  } else {
    digitalWrite(waterPump, HIGH);
    
  }
}

//Get the soil moisture values
void soilMoistureSensor() {
  int value = analogRead(sensor);
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  value = map(value, 0, 1024, 0, 100);
  value = (value - 100) * -1;

  if (isnan(h) || isnan(t)) {
    Serial.print("Cannot read from dht 11 : ");
    return;
  }

  Blynk.virtualWrite(V2, value);
  Blynk.virtualWrite(V6, t);
  Blynk.virtualWrite(V5, h);
  Serial.print("Moisture : ");
  Serial.print(value);
  Serial.print("   Temperature : ");
  Serial.print(t);
  Serial.print("    Humidity : ");
  Serial.println(h);

}

void loop() {
  Blynk.run();//Run the Blynk library
  timer.run();//Run the Blynk timer
}