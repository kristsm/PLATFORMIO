#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WebSocketsServer.h>
#include <Hash.h>
WebSocketsServer webSocket = WebSocketsServer(81);
const char *ssid = "ESP";
const char *password = "12345678";
const int bluePin = 16;

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght) {

    switch(type) {
        case WStype_DISCONNECTED:

            break;
        case WStype_CONNECTED:
         {
                IPAddress ip = webSocket.remoteIP(num);


            }
            break;
        case WStype_TEXT:
        {

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


void setup() {

    Serial.begin(115200);
    pinMode(16,OUTPUT);

   WiFi.softAP(ssid, password);

 IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
    webSocket.begin();
    webSocket.onEvent(webSocketEvent);
}

void loop() {
    webSocket.loop();
}
