#include <stdint.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>

const char *ssid = "ESP32";
const char *password = "12345678";

WiFiServer server(10000);

uint8_t Code[0x10000];
uint32_t i = 0;
uint8_t flag = 0;

void setup() {
  Serial.begin(115200);
  // Serial.println();
  // Serial.println("Configuring access point...");
  if (!WiFi.softAP(ssid, password)) {
    log_e("Soft AP creation failed.");
    while (1) {}
  }
  IPAddress myIP = WiFi.softAPIP();
  // Serial.print("AP IP address: ");
  // Serial.println(myIP);
  server.begin();

  // Serial.println("Server started");
}

void loop() {
  WiFiClient client = server.available();
  client.setNoDelay(1);
  if (client) {
    // Serial.println("New Client.");
    while (client.connected()) {
      if (client.available()) {
        Code[i] = client.read();
        // Serial.println(Code[i],HEX);
        i++;
      } else {
        client.print(1);
      }
    }
    // Serial.println(i);
    Serial.write((uint8_t)(i>>8));
    Serial.write((uint8_t)(i&0xFF));
    client.stop();
    flag = 1;
    // Serial.println("Client Disconnected.");
  }
  if (flag == 1) {
    Serial.write(Code, i);
    i = 0;
    flag = 0;
  }
}
