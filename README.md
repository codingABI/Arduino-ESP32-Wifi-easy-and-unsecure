# ESP32 Wifi easy and unsecure?
I have used Wifi on ESP32 microcontrollers in the past and liked how easy it is to use. But now I discovered that is seems to be a risk for my Wifi because the Wifi credentials are stored in an unsecure way (testet in arduino-esp32 1.0.6 and 2.0.5).

My concerns are: *Everybody how can upload a [sketch](src/WiFiWithoutCredentials.ino) to an ESP32 can read the previous used Wifi credentials in plain text.*

Now I show you how to reproduce my concerns:

## Easy
This is a common, easy [Arduino sketch](src/WiFiWithCredentials.ino) to use a Wifi connection on an ESP32:
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
I think many people use Wifi on ESP32 in this or a similar way. After compiling und uploading this sketch to the ESP32 the serial output shows that Wifi is working fine:
```
Connect Wifi with credentials
.
Successfully connected and ESP got IP 192.168.170.26
```
## Unsecure
Now I overwrite the ESP32 with a complete another [sketch](src/WiFiWithoutCredentials.ino), which contains **NO** Wifi credentials:
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
After compiling and uploading the second sketch the serial output shows that **Wifi is still working** and I can read the **Wifi key (=password/psk/preshared-key) in plain text:**
```
Connect without credentials
.
Successfully connected and ESP got IP 192.168.170.26
SSID mysid
PSK mySecretPassword1#
```
Isn't that crazy? It makes no difference whether I power off and on the ESP32 or push the reset button. The Wifi credentials seems to be stored on the ESP32 after the first [sketch](src/WiFiWithCredentials.ino) and can be read by every sketch runs on the same ESP32 => If somebody uploads a new sketch on my ESP32-devices he can read my Wifi credentials.
## My workarounds
I found three workarounds that seems to clear the Wifi credentials
1) Arduino IDE-Option: Erase All Flash Before Sketch Upload: "Enabled"
2) `WiFi.disconnect(true,true)` or `WiFi.disconnect(false,true)`
3) Clearing Wifi configuration after each connection (currently my favourite)
### Erase All Flash Before Sketch Upload: "Enabled"
This is only a workaround when I am uploading a sketch, but this can be used to reset your ESP32 before ou give the microcontroller to another person.
![EraseAllFlash](/assets/images/EraseAllFlash.png) 
### Wifi.disconnect(true/false,true)
`WiFi.disconnect(true,true)` or `WiFi.disconnect(false,true)` also switch off wifi, which would be a problem for wifi driven projects, but you can use these commands to reset your ESP32 before you give the microcontroller to another person.

Code example (`WiFi.begin` is needed to get `WiFi.disconnect` working):
```
WiFi.begin();
WiFi.disconnect(true,true);
```
### Clearing Wifi configuration after each connection
This will not disconnect the running wifi connection, but `WiFi.reconnect()` will not work after clearing the Wifi configuration. 

To clear the Wifi configuration after each connection you can use the `esp_wifi_set_config` command:
```
#include <esp_wifi.h>
...
wifi_config_t conf;
memset(&conf, 0, sizeof(wifi_config_t));
if(esp_wifi_set_config(WIFI_IF_STA, &conf)){
  log_e("clear config failed!");
}
```
like in this [sketch](src/WifiWithSecureCredentials.ino):
```
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
```
