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

/*
가끔식 비정확한 값이 수신되기 때문에,
3번 이상 닫힘/열림 동일한 값이 들어와야 문 상태를 업데이트 하도록 바꿈
*/
int prevState = 0;
int openCnt = 0;
int closeCnt = 0;


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
  // 블루투스 수신값이 있다면 블루투스를, 없다면 문 잠금잠치 
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
        if(openCnt < consecutive_standard) openCnt++;
        closeCnt = 0;
      } else {
        // 닫힘
        newState = 0;
        if(closeCnt < consecutive_standard) closeCnt++;
        openCnt = 0;
      }
    }

    // 만약 세 번 이상 연속으로 문 열림 신호가 오고, 문이 닫혀있다면
    if(openCnt == consecutive_standard && isDoorOpen == 0) {
      lightOn();
      isDoorOpen = 1;
    } else if(closeCnt == consecutive_standard && isDoorOpen == 1) {
      // 만약 세 번 이상 연속으로 문 닫힘 신호가 오고, 문이 열려있다면
      lightOff();
      isDoorOpen = 0;
    }

    // if(newState != -1 && isDoorOpen != newState) {
    //   Serial.println("Point0");
    //   if(newState == 1 && openCnt >= 3) {
    //     lightOn();
    //     Serial.println("Light On Activate!");
    //   } else if (newState == 0 && closeCnt >= 3) {
    //     lightOff();
    //     Serial.println("Light Off Activate!!!");
    //   }

    //   isDoorOpen = newState;
    // }
    

    client.stop();
    Serial.println("Client Disconnected");
    delay(200);
  }
}


void doorStateChange() {

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