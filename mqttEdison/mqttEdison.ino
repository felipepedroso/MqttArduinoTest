
#include <WiFi.h>
#include <PubSubClient.h>

int wifiStatus;

WiFiClient wifiClient;
PubSubClient client(wifiClient);

IPAddress server(198, 41, 30, 241);

void setup() {
  Serial.begin(9600);

  client.setServer(server, 1883);
  client.setCallback(callback);

  connectToWifi("AP 82", "AwesomeInstead");
  // Allow the hardware to sort itself out
  delay(1500);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("arduinoClient")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("wearableOut", "hello world");
      // ... and resubscribe
      client.subscribe("wearableIn");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void connectToWifi(char* ssid, char* password) {
  while (wifiStatus != WL_CONNECTED) {
    wifiStatus = WiFi.begin(ssid, password);
    String message = "Connecting to " + String(ssid);
    Serial.println(message);
    delay(1000);
  }

  Serial.println("Connected!");
  delay(500);
}
