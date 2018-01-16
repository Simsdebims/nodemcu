#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Sonnendeck";
const char* password = "Vjemwdbnk5yb";
const char* mqtt_server = "10.42.0.1";
const char* topic = "sensoren/aussen/test";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;

long duration;
double distance;
String distance_msg;

const int LED_PIN = D4;
const int US_TRIGGER_PIN = D4;
const int US_ECHO_PIN = D3;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(US_TRIGGER_PIN, OUTPUT); 
  pinMode(US_ECHO_PIN, INPUT);
  
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

void setup_wifi() {

  delay(10);
  Serial.println();
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

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
/*
  digitalWrite(US_TRIGGER_PIN, LOW);  // Get Start
  delayMicroseconds(2); // stable the line 
  digitalWrite(US_TRIGGER_PIN, HIGH); // sending 10 us pulse
  delayMicroseconds(10); // delay 

  digitalWrite(US_TRIGGER_PIN, LOW); // after sending pulse wating to receive signals 

  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(US_ECHO_PIN, HIGH); 
*/
  duration = 150;

  distance = duration*0.034/2;
  distance_msg = distance;  

  long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    Serial.print("Publish message: ");
    Serial.println(distance_msg);
    client.publish(topic, distance_msg.c_str());
  }
}
