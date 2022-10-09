#include <WiFi.h>
#define WIFIMAXRETRIES 30

void setup() {
  int wifiRetry = 0;

  Serial.begin(115200);

  Serial.println("Connect without credentials");
  WiFi.begin();

  while ((WiFi.status() != WL_CONNECTED) && (wifiRetry <= WIFIMAXRETRIES)) { // wait while connecting
    wifiRetry++;
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("Successfully connected and ESP got IP ");
    Serial.println(WiFi.localIP());
  } else Serial.println("Connection failed");
  Serial.print("SSID ");
  Serial.println(WiFi.SSID());
  Serial.print("PSK ");
  Serial.println(WiFi.psk());
}

void loop() {
}
