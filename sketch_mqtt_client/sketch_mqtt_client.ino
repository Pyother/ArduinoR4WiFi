#include <WiFiS3.h>
#include <ArduinoMqttClient.h>
#include <Servo.h>

#define SECRET_SSID ""
#define SECRET_PASS ""
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
  servo.attach(9);
  servo.write(40);
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
  Serial.println("Connected to WiFi");
  printWifiStatus();

  if(!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed, error code = ");
    Serial.println(mqttClient.connectError());
    while(1);
  }
  Serial.println("✓ Connected to MQTT broker.");
  mqttClient.subscribe(topic);
}

void loop() {
  int messageSize = mqttClient.parseMessage();
  if (messageSize) {
    // we received a message, print out the topic and contents
    Serial.print("Received a message with topic '");
    Serial.print(mqttClient.messageTopic());
    Serial.print("', length ");
    Serial.print(messageSize);
    Serial.println(" bytes:");

    // use the Stream interface to print the contents
    while (mqttClient.available()) {
      char value = mqttClient.read();
      // 
    }
    Serial.println();
  }

  delay(3000);
  mqttClient.beginMessage(topic);
  mqttClient.print(random(1000));
  mqttClient.endMessage();
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

