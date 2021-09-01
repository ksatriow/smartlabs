#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
 
/************************* WiFi Access Point *********************************/
#define WLAN_SSID       "HELLU" // fill with your ssid
#define WLAN_PASS       ""  // fill with your wifi password
 
 
#define AIO_SERVER      "192.168.43.234"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    ""
#define AIO_KEY         ""
#define LEDPIN D6

WiFiClient client;
 
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
 
Adafruit_MQTT_Subscribe brightness = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/brightness");
 
void MQTT_connect();
 
void setup() {
  Serial.begin(115200);
  delay(10);
  pinMode(LEDPIN,OUTPUT);
  Serial.println(F("Lab Elektro"));
 
  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);
 
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
 
  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());
 
  mqtt.subscribe(&brightness);
}
 
uint32_t x=0;
 
void loop() {
  MQTT_connect();
 
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000))) {
    if (subscription == &brightness) {
      Serial.print(F("Got: "));
      String brightnessValue = (char *)brightness.lastread;
      Serial.println(brightnessValue);
      analogWrite(LEDPIN, brightnessValue.toInt());
       
    }
  }
 
}
 
void MQTT_connect() {
  int8_t ret;
 
  if (mqtt.connected()) {
    return;
  }
 
  Serial.print("Connecting to MQTT... ");
 
  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { 
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  
       retries--;
       if (retries == 0) {
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}
