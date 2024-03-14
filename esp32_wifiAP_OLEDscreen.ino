#include <WiFi.h>
#include "esp_wifi.h"
#include <SSD1306Wire.h>

const char *ssid = "ESP32 WIFI AP";        // SSID of your access point
const char *password = "YourPassword"; // Password of your access point

SSD1306Wire display(0x3c, 21, 22); // I2C address, SDA pin, SCL pin

unsigned long uptime = 0; // Variable to store uptime in milliseconds

void setup() {
  Serial.begin(115200);

  // Initialize the OLED display
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);

  // Setting up the access point
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);

  Serial.println("Access Point Started");
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());

  uptime = millis();
}

void loop() {
  // Get the number of connected devices
  wifi_sta_list_t stationList;
  esp_wifi_ap_get_sta_list(&stationList);
  int numDevices = stationList.num;
  Serial.print("Number of devices connected: ");
  Serial.println(numDevices);
  // Calculate uptime in seconds
  unsigned long currentMillis = millis();
  unsigned long uptimeSeconds = (currentMillis - uptime) / 1000;

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

  // Display information on the oled screen
  display.clear();
  display.drawString(0, 0, "Access Point: ");
  display.drawString(0, 15, ssid);
  display.drawString(0, 30, "Connected Devices: ");
  display.drawString(120, 30, String(numDevices));
  display.drawString(0, 42, "Uptime: ");
  display.drawString(60, 42, formatTime(uptimeSeconds));
  display.display();


  delay(1000); // Update every 5 seconds
}

// Function to format uptime in HH:MM:SS format
String formatTime(unsigned long seconds) {
  unsigned long hours = seconds / 3600;
  unsigned long minutes = (seconds % 3600) / 60;
  unsigned long secs = seconds % 60;

  char buffer[9];
  sprintf(buffer, "%02d:%02d:%02d", hours, minutes, secs);
  return String(buffer);
}
