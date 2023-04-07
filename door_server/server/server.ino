#include "SecretConstant.h"
#include <ESP8266WiFi.h>
#include <Servo.h>
#include <stdio.h>
#include <stdlib.h>
#include <SPI.h>
#include <Ethernet.h>
#include <SoftwareSerial.h>

// 와이파이명
const char* ssid = constant_ssid;
const char* password = constant_password;

WiFiServer server(constant_port);

// 블루투스
SoftwareSerial btSerial (D5, D6);

// 서보 모터
Servo servo;
int degree = 0; // 모터의 각도
int isDoorOpen = 0; // 1 - 열림, 0 - 닫힘
int lightState = 0; // 1 - 켜짐, 1 - 꺼짐

void setup() {
  servo.attach(D1);

  // put your setup code here, to run once:
  Serial.begin(115200);
  btSerial.begin(9600);

  delay(1000);

  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi . . .");
  }

  Serial.println("Connected to WiFi");

  // start the server
  server.begin();
  Serial.println("Server started");

 IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);

}

void loop() {
  if(btSerial.available()) {
    int command = btSerial.read();
    Serial.print("Bluetooth Input : ");
    Serial.println(command);
    
    if(command == '1') {
      lightOn();
    } else if(command == '0') {
      lightOff();
    }
  } else {
    WiFiClient client = server.available();
    int newState = -1;

    if(client) {
      Serial.println("Client Connected");

      String string = "";

      //모든 글자 읽어오기
      while(client.connected()) {
        if(client.available()) {
          char c = client.read();
          string += c;
        }
      }

      Serial.println(string);

      // string to float
      float distance = string.toFloat();


      if(distance > door_state_standard) {
        // 열림
        newState = 1;
      } else {
        // 닫힘
        newState = 0;
      }
    }

    Serial.print("isDoorOpen : ");
    Serial.println(isDoorOpen);
    Serial.print("newState : ");
    Serial.println(newState);

    if(newState != -1 && isDoorOpen != newState) {
      Serial.println("Point0");
      if(newState == 1) {
        lightOn();
      } else {
        lightOff();
      }

      isDoorOpen = newState;
    }
    

    client.stop();
    Serial.println("Client Disconnected");
    delay(200);
  }
}

void lightOn() {
  servo.write(0);
  delay(1200);
  servo.write(90);
  Serial.println("Point1");

  lightState = 1;
}


void lightOff() {
  servo.write(170);
  delay(1200);
  servo.write(90);
  Serial.println("Point2");

  lightState = 0;
}



