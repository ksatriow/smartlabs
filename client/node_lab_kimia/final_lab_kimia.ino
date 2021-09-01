#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <MQ2.h>
#include<Servo.h>

// Wifi
#define ssid       "HELLU" // fill with your ssid
#define password   ""  // fill with your wifi password

// MQTT Broker
#define mqtt_server "192.168.43.197" 
#define mqtt_topic_pub "kimia"   
#define mqtt_topic_sub "kimia"
#define mqtt_user ""   
#define mqtt_pwd ""
//
const uint16_t mqtt_port =  1883; 

#define PIN_MQ2 A0
#define red 16
#define green 5
#define buzzer 4
#define servoPin 0


MQ2 mq2_sensor = MQ2(PIN_MQ2);
WiFiClient espClient;
PubSubClient client(espClient);

long lastMsg = 0;
char msg[50]; 
int value = 0;

int pos = 0;

Servo myservo;

void setup() {
  Serial.begin(115200);
  //setup_wifi();
  client.setServer(mqtt_server, mqtt_port); 
  client.setCallback(callback);
  mq2_sensor.begin();
  myservo.attach(servoPin);
  pinMode(13,OUTPUT);
  myservo.write(0);
  pinMode(red,OUTPUT);
  pinMode(green,OUTPUT);
  //
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived []");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP8266Client",mqtt_user, mqtt_pwd)) {
      Serial.println("connected");
      client.publish(mqtt_topic_pub, "ESP_reconnected");
      client.subscribe(mqtt_topic_sub);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(1000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  float gas = mq2_sensor.readLPG();
  if (isnan(gas)) {
   Serial.println("Failed to read from sensor!");
   return;
  }
  
  long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    static char gasTemp[7];
    dtostrf(gas, 6, 2, gasTemp);
    client.publish("sensor/gas", gasTemp);
    Serial.print(" *C\t gas: ");   Serial.print(gas);
    Serial.print("\n ");
  }
  //
  if (gas > 500){
        digitalWrite(green, LOW); 
        digitalWrite(red, HIGH);
        pos = 180; 
        myservo.write(pos);
        digitalWrite(13,HIGH);
  }
  else {
       digitalWrite(red, LOW); 
       digitalWrite(green, HIGH);
       pos = 0; 
       myservo.write(pos);
       digitalWrite(13,LOW);
  }
  delay(1000);
}
