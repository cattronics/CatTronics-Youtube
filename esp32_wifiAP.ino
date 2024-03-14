#include <WiFi.h>
#include "esp_wifi.h"

const char *ssid = "YourSSID";        // SSID of your access point
const char *password = "YourPassword"; // Password of your access point

void setup() {
  Serial.begin(115200);

  // Setting up the access point
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);

  Serial.println("Access Point Started");
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());
}

void loop() {
  // Get the number of connected devices
  wifi_sta_list_t stationList;
  esp_wifi_ap_get_sta_list(&stationList);
  int numDevices = stationList.num;
  Serial.print("Number of devices connected: ");
  Serial.println(numDevices);

  // Display connected device information in serial monitor
  Serial.println("Connected devices:");
  for (int i = 0; i < numDevices; i++) {
    wifi_sta_info_t station = stationList.sta[i];
    Serial.print("MAC Address: ");
    for (int j = 0; j < 6; j++) {
      Serial.printf("%02X", station.mac[j]);
      if (j < 5) {
        Serial.print(":");
      }
    }
    Serial.println();
  }
  
  delay(5000); 
}