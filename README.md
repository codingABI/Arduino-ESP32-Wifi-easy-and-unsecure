# ESP32Wifi Easy and unsafe?
I used Wifi on ESP32 microcontrollers in the past and liked how easy it is to use. But now I discovered that is seems to be risk because the Wifi credentials are stored in an unsecure way.

To show you my concerns:
This is a common Arduino Sketch to create a Wifi connection on an ESP32:
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
  } else Serial.print("Connection failed");
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
Now I overwrite the ESP32 with a complete another sketch, where I use no Wifi credentials:
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
    Serial.print("SSID ");
    Serial.println(WiFi.SSID());
    Serial.print("PSK ");
    Serial.println(WiFi.psk());
  } else Serial.print("Connection failed");

}
void loop() {
}
```
After starting the second sketch the serial output shows that Wifi is still working and I can read the Wifi key/password/psk in cleartext:
```
Connect without credentials
18:16:02.219 -> .
18:16:02.219 -> Successfully connected and ESP got IP 192.168.170.26
18:16:02.219 -> SSID mysid
18:16:02.219 -> PSK mySecretPassword1#
```
Isn't this crazy? It makes no difference whether I power off and on the ESP32 or push the Reset button. The Wifi Wifi key/password/psk seems to be stored on the ESP32 after the first connection and can be read by every sketch runs on the same ESP32.
