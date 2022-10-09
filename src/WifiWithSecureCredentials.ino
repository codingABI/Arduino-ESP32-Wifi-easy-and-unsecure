#include <WiFi.h>
#include <esp_wifi.h>

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
  
  // Clear Wifi credentials in ESP32 after connection
  wifi_config_t conf;
  memset(&conf, 0, sizeof(wifi_config_t));
  if(esp_wifi_set_config(WIFI_IF_STA, &conf)){
    log_e("clear config failed!");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("Successfully connected and ESP got IP ");
    Serial.println(WiFi.localIP());
  } else Serial.println("Connection failed");
}

void loop() {
}
