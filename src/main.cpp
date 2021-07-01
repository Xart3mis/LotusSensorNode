#include <Arduino.h>
#include <ArduinoWebsockets.h>
#include <ESP8266WiFi.h>
#include <secrets.h>

using namespace websockets;

void onMessageCallback(WebsocketsMessage message) {
  Serial.print("Got Message: ");
  Serial.println(message.data());
}

void onEventsCallback(WebsocketsEvent event, String data) {
  if (event == WebsocketsEvent::ConnectionOpened) {
    Serial.println("Connnection Opened");
  } else if (event == WebsocketsEvent::ConnectionClosed) {
    Serial.println("Connnection Closed");
  } else if (event == WebsocketsEvent::GotPing) {
    Serial.println("Got a Ping!");
  } else if (event == WebsocketsEvent::GotPong) {
    Serial.println("Got a Pong!");
  }
}

WebsocketsClient client;
void setup() {
  Serial.begin(115200);

  WiFi.begin(SECRET_SSID, SECRET_PASS);

  for (int i = 0; i < 10 && WiFi.status() != WL_CONNECTED; i++) {
    Serial.print(".");
    delay(1000);
  }

  client.onMessage(onMessageCallback);
  client.onEvent(onEventsCallback);
  client.connect(SECRET_ENDPOINT);
  client.ping();
}

char str[20];
auto Node(int node) { return sprintf(str, "hello from node %d", node); }

void loop() {
  client.poll();
  Node(2);
  client.send(str);
  Serial.println(str);
  delay(5000);
}