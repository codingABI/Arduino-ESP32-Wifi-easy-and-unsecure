#include <WiFi.h>

#define WIFIMAXRETRIES 30
#define SSID "mysid"
#define PASSWORD "mySecretPassword1"

void setup() {
  int wifiRetry = 0;

  Serial.begin(115200);

  Serial.println("Connect Wifi with credentials");
  WiFi.begin(SSID,PASSWORD);

  while ((WiFi.status() != WL_CONNECTED) && (wifiRetry <= WIFIMAXRETRIES)) {
    wifiRetry++;
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("Successfully connected and ESP got IP ");
    Serial.println(WiFi.localIP());
  } else Serial.println("Connection failed");
}

void loop() {
}
