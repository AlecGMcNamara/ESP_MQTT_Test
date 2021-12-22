#include <PubSubClient.h>
#include <WiFi.h>
#include <Arduino.h>

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
int intCounter = 0;

//--------- WIFI -------------------------------------------

void wifi_connect() {
  Serial.print("Starting connecting WiFi.");
  delay(10);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

//------------------ MQTT ----------------------------------
void mqtt_setup() {
  client.setServer(mqttServer, mqttPort);
    client.setCallback(callback);
    Serial.println("Connecting to MQTT…");
    while (!client.connected()) {        
        String clientId = "ESP32Client-";
        clientId += String(random(0xffff), HEX);
        if (client.connect(clientId.c_str(), mqttUser, mqttPassword )) {
            Serial.println("connected");
        } else {
            Serial.print("failed with state  ");
            Serial.println(client.state());
            delay(2000);
        }
    }
}

void callback(char* topic, byte* payload, unsigned int length) {

    Serial.print("Message arrived in topic: ");
    Serial.println(topic);

    String byteRead = "";
    Serial.print("Message: ");
    for (int i = 0; i < length; i++) {
        byteRead += (char)payload[i];
    }    
    Serial.println(byteRead);
    intCounter = byteRead.toInt() + 1;
}

void setup() {  
  Serial.begin(115200);  
  wifi_connect();
  mqtt_setup();  
  client.subscribe(mqttTopic);
 }

void loop() {
static int delay = millis() + 2500;
char charBuf[10]; 
String(intCounter).toCharArray(charBuf, 10);

    client.loop();
    if(millis() > delay){
      client.publish(mqttTopic, charBuf ,true); //retain
      intCounter++;
      delay = millis() + 5000; //every 5 seconds
    }
}