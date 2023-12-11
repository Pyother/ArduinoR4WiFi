#include <WiFiS3.h>
#include "ArduinoMqttClient.h"
#include "arduino_secrets.h"

#define MQTT_SERVER "test.mosquitto.org"
#define MQTT_PORT 1833

int   status   = WL_IDLE_STATUS;
char  ssid[]   = SECRET_SSID;        
char  pass[]   = SECRET_PASS;  
char  broker[] = "test.mosquitto.org";
char  topic[]  = "measurements";
int   port     = 1883;

int keyIndex = 0;  
unsigned int localPort = 2390;  
char packetBuffer[256]; 
char  ReplyBuffer[] = "acknowledged\n";       

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; 
  }

  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    delay(10000);
  }
  printWifiStatus();

  if(!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed, error code = ");
    Serial.println(mqttClient.connectError());
    while(1);
  }
  Serial.println("✓ Connected to MQTT broker.");
  Serial.println();
  mqttClient.subscribe(topic);
}

void loop() {
  int messageSize = mqttClient.parseMessage();
  if (messageSize) {
    Serial.print("Received a message: ");

    while (mqttClient.available()) {
      char value = mqttClient.read();
      Serial.println(value);
      if(value == 's') {
        function();
      }
    }
  }
}

void function() {
  Serial.println("S");
}

void printWifiStatus() {
  Serial.print("✓ Connected to network ");
  Serial.println(ssid);
}

