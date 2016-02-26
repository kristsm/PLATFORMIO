#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>
#include <Hash.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

const char *ssid = "SHARLS";
const char *password = "26316445";
const int bluePin = 16;

// WEBSOCKETS START
WebSocketsServer webSocket = WebSocketsServer(81);
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght) {
    switch(type) {
        case WStype_DISCONNECTED:
        break;
        case WStype_CONNECTED: {
        IPAddress ip = webSocket.remoteIP(num);
        }
            break;
        case WStype_TEXT: {
        String text = String((char *) &payload[0]);
          if(text=="LED"){

            digitalWrite(16,HIGH);
            delay(50);
            digitalWrite(16,LOW);
            Serial.println("led just lit");
            webSocket.sendTXT(num, "led just lit", lenght);
            }
                   }
            webSocket.sendTXT(num, payload, lenght);
            webSocket.broadcastTXT(payload, lenght);
            break;

            case WStype_BIN:
            hexdump(payload, lenght);
            // echo data back to browser
            webSocket.sendBIN(num, payload, lenght);
            break;
    }

  }
  // WEBSOCKETS END


void setup() {

    Serial.begin(115200);
    pinMode(16,OUTPUT);

    WiFi.begin(ssid, password);
    while(WiFi.status() != WL_CONNECTED) {
        delay(100);
        }
    Serial.println(WiFi.localIP());

    // WEBSOCKETS  SETUP
    webSocket.begin();
    webSocket.onEvent(webSocketEvent);
    // WEBSOCKETS  SETUP ENDS

    // OTA STARTS
        // Port defaults to 8266
        // ArduinoOTA.setPort(8266);

        // Hostname defaults to esp8266-[ChipID]
        // ArduinoOTA.setHostname("myesp8266");

        // No authentication by default
        // ArduinoOTA.setPassword((const char *)"123");

  ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
    // OTA ENDS
}


void loop() {
    ArduinoOTA.handle();
    webSocket.loop();
}
