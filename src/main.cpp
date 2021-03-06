#include <PubSubClient.h>
#include <WiFi.h>
#include <Arduino.h>

#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  30       /* Time ESP32 will go to sleep (in seconds) */

void callback(char* topic, byte* payload, unsigned int length);

const char* ssid = "SKYPEMHG";
const char* password = "8NHetSWQAJ75";
const char* mqttServer = "192.168.0.67";
const int mqttPort = 1883;
const char* mqttUser = "";
const char* mqttPassword = "";
const char* mqttTopic = "GlenCroft/Kitchen/Temperature";

WiFiClient espClient;
PubSubClient client(espClient);

//--------- WIFI -------------------------------------------

void wifi_connect() {
  Serial.print("Connecting to WiFi.");
  delay(10);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
}

//------------------ MQTT ----------------------------------
void mqtt_setup() {
  client.setServer(mqttServer, mqttPort);
  //  client.setCallback(callback);
    Serial.print("Connecting to MQTT....");
    while (!client.connected()) {        
        String clientId = "ESP32Client-";
        clientId += String(random(0xffff), HEX);
        if (client.connect(clientId.c_str(), mqttUser, mqttPassword )) {
            Serial.print("connected, server: ");
            Serial.println(mqttServer);
        } else {
            Serial.print("failed with state  ");
            Serial.println(client.state());
            delay(2000);
        }
    //client.subscribe(mqttTopic);    // not needed for this project
    }
}
/* not used in this project
void callback(char* topic, byte* payload, unsigned int length) {

    Serial.print("Message arrived for topic: "); Serial.println(topic);

    String byteRead = "";
    Serial.print("Message: ");
    for (int i = 0; i < length; i++) {
        byteRead += (char)payload[i];
    }    
    Serial.println(byteRead);
} */

void setup() {  
  Serial.begin(115200); 
  delay(500); // avoid wake up bug? 
  
  wifi_connect();
  mqtt_setup();  
  client.loop();

  char charBuf[10];   
  dtostrf((temperatureRead()-32)/1.8,5,2,charBuf);
  
  client.publish(mqttTopic, charBuf,true); //retain

  Serial.print("Sent: ");  Serial.print(mqttTopic); Serial.print(" = "); Serial.println(charBuf); 
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  Serial.print("Going to sleep for "); Serial.print(TIME_TO_SLEEP); Serial.println(" seconds");


  esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH,   ESP_PD_OPTION_OFF);
  esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_SLOW_MEM, ESP_PD_OPTION_OFF);
  esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_FAST_MEM, ESP_PD_OPTION_OFF);
  esp_sleep_pd_config(ESP_PD_DOMAIN_XTAL,         ESP_PD_OPTION_OFF);
  esp_deep_sleep_start();

 }

void loop() {
 //nothing to do here.
}