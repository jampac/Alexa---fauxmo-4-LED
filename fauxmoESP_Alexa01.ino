#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "fauxmoESP.h"
#define WIFI_SSID "entropy"
#define WIFI_PASS "xxxxxx"

#define SERIAL_BAUDRATE                 115200
#define LED                             2

//#include <Wire.h>
//#include "font.h"
////#define offset 0x00    // SDD1306                      // offset=0 for SSD1306 controller
//#define offset 0x02    // SH1106                       // offset=2 for SH1106 controller
//#define OLED_address  0x3c                             // all the OLED's I have seen have this address

fauxmoESP fauxmo;

// -----------------------------------------------------------------------------
// Wifi
// -----------------------------------------------------------------------------


void wifiSetup() {
  // Set WIFI module to STA mode
  WiFi.mode(WIFI_STA);

  // Connect
  //Serial.printf("[WIFI] Connecting to %s ", WIFI_SSID);
  //WiFi.begin(WIFI_SSID, WIFI_PASS);

  // Wait
  int wifiwait = 0;

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    wifiwait++;
    digitalWrite(BUILTIN_LED, HIGH);
    digitalWrite(5, HIGH);
    delay(100);
    digitalWrite(BUILTIN_LED, LOW);
    digitalWrite(5, LOW);
    delay(100);
    if (wifiwait > 100) {
      wifiwait = 0;
      WiFi.disconnect();
      delay(100);
      Serial.printf("[WIFI] Connecting to %s ", WIFI_SSID);
      WiFi.begin(WIFI_SSID, WIFI_PASS);
    }
  }
  Serial.println();

  // Connected!
  Serial.printf("[WIFI] STATION Mode, SSID: %s, IP address: %s\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());

}


void setup() {

//  Wire.begin(0, 2);                               // Initialize I2C and OLED Display
//  init_OLED();                                    //
//  reset_display();

  // Init serial port and clean garbage

  Serial.begin(SERIAL_BAUDRATE);
  Serial.println();
  Serial.println();

  Serial.println("fauxmoESP_Alexa01 070617");

  pinMode(14, OUTPUT);
  digitalWrite(14, 1); // D5
  delay(500);
  digitalWrite(14, 0); // D5
  delay(500);
  pinMode(12, OUTPUT);
  digitalWrite(12, 1); // D6
  delay(500);
  digitalWrite(12, 0); // D6
  delay(500);
  pinMode(13, OUTPUT);
  digitalWrite(13, 1); // D7
  delay(500);
  digitalWrite(13, 0); // D7
  delay(500);
  pinMode(15, OUTPUT);
  digitalWrite(15, 1); // D8
  delay(500);
  digitalWrite(15, 0); // D8
  delay(500);


  // Wifi
  wifiSetup();

  // LED
  pinMode(14, OUTPUT);
  digitalWrite(14, 1); // D5
  delay(500);
  digitalWrite(14, 0); // D5
  delay(500);

  // Fauxmo
  fauxmo.addDevice("light one");
  fauxmo.addDevice("light two");
  fauxmo.addDevice("light three");
  fauxmo.addDevice("light four");

  // fauxmoESP 2.0.0 has changed the callback signature to add the device_id, this WARRANTY
  // it's easier to match devices to action without having to compare strings.
  fauxmo.onMessage([](unsigned char device_id, const char * device_name, bool state) {

    Serial.printf("[MAIN] Device #%d (%s) state: %s\n", device_id, device_name, state ? "ON" : "OFF");
    //digitalWrite(12, state);


    if ((strcmp(device_name, "light one") == 0)) {
      digitalWrite(14, state); // D5
      Serial.printf("Light one: switched");
    }
    if ((strcmp(device_name, "light two") == 0)) digitalWrite(12, state); // D6
    if ((strcmp(device_name, "light three") == 0)) digitalWrite(13, state); // D7
    if ((strcmp(device_name, "light four") == 0)) digitalWrite(15, state); // D8

  });

}

void loop() {

  // Since fauxmoESP 2.0 the library uses the "compatibility" mode by
  // default, this means that it uses WiFiUdp class instead of AsyncUDP.
  // The later requires the Arduino Core for ESP8266 staging version
  // whilst the former works fine with current stable 2.3.0 version.
  // But, since it's not "async" anymore we have to manually poll for UDP
  // packets
  fauxmo.handle();


  static unsigned long last = millis();
  if (millis() - last > 5000) {
    last = millis();
    //Serial.printf("[MAIN] Free heap: %d bytes\n", ESP.getFreeHeap());
  }

}



