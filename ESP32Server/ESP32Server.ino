#include <stdint.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>

const char *ssid = "AGV";
const char *password = "Bachelor";

WiFiServer server(10000);

uint8_t Code[10000];
uint32_t i = 0;

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("Configuring access point...");
  if (!WiFi.softAP(ssid, password)) {
    log_e("Soft AP creation failed.");
    while (1) {}
  }
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.begin();

  Serial.println("Server started");
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New Client.");
    while (client.connected()) {
        if (client.available()) {
        Code[i] = client.read();
        // if (Code[i] != 194 && Code[i] != 195) {
          Serial.println(Code[i],HEX);
          i++;
        // }
        client.print(1);
      }
    }
    Serial.println(i);
    client.stop();
    Serial.println("Client Disconnected.");
    i = 0;
  }
}
