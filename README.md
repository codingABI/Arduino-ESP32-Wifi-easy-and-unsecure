# ESP32 Wifi easy and unsafe?
I have used Wifi on ESP32 microcontrollers in the past and liked how easy it is to use. But now I discovered that is seems to be risk for my Wifi because the Wifi credentials are stored in an unsecure way.

To show you my concerns:

This is a common [Arduino sketch](src/WiFiWithCredentials.ino) to use a Wifi connection on an ESP32:
```
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
```
After compiling und uploading this sketch to the ESP32 the serial output shows that Wifi is working fine:
```
Connect Wifi with credentials
.
Successfully connected and ESP got IP 192.168.170.26
```
Now I overwrite the ESP32 with a complete another [sketch](src/WiFiWithoutCredentials.ino), which contains NO Wifi credentials:
```
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
```
After compiling and uploading the second sketch the serial output shows that Wifi is still working and I can read the Wifi key/password/psk in plain text:
```
Connect without credentials
.
Successfully connected and ESP got IP 192.168.170.26
SSID mysid
PSK mySecretPassword1#
```
Isn't this crazy? It makes no difference whether I power off and on the ESP32 or push the Reset button. The Wifi Wifi key/password/psk seems to be stored on the ESP32 after the first sketch and can be read by every sketch runs on the same ESP32 => If somebody uploads a new sketch on my ESP32-devices he can read my Wifi credentials.
## Workaround
Clearing Wifi configuration after connection attempt like in this [sketch](src/WifiWithSecureCredentials.ino).

```
#include <esp_wifi.h>
...
wifi_config_t conf;
memset(&conf, 0, sizeof(wifi_config_t));
if(esp_wifi_set_config(WIFI_IF_STA, &conf)){
  log_e("clear config failed!");
}
```
I expect the functions like WiFi.reconnect() will not work after clearing the Wifi configuration.
