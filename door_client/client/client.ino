#include "SecretConstant.h"
#include <ESP8266WiFi.h>
#include <stdio.h>

// 와이파이명
const char* ssid = constant_ssid;
const char* password = constant_password;

int echoPin = D2;
int tringPin = D1; 


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(10);

  pinMode(tringPin, OUTPUT);
  pinMode(echoPin, INPUT);


  int n = WiFi.scanNetworks();
  Serial.println("Scan Done");
  if(n == 0) {
    Serial.println("no networks found");
  } else {
    Serial.print(n);
    Serial.println(" networks found");

    for(int i = 0; i<n; i++) {
     Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*");
      delay(10);
    }
  }



  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);


  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("connection Start");

}

void loop() {
  WiFiClient client;

  if(!client.connect(constant_client_ip, constant_port)) {
    Serial.println("connection failed"); // 서버 접속에 실패
    delay(500);
  }

  // put your main code here, to run repeatedly:
  	// 초음파를 보낸다. 다 보내면 echo가 HIGH 상태로 대기하게 된다.
  digitalWrite(tringPin, LOW);
  digitalWrite(echoPin, LOW);
  delayMicroseconds(2);
  digitalWrite(tringPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(tringPin, LOW);

	
	// echoPin 이 HIGH를 유지한 시간을 저장 한다.
  unsigned long duration = pulseIn(echoPin, HIGH); 
	// HIGH 였을 때 시간(초음파가 보냈다가 다시 들어온 시간)을 가지고 거리를 계산 한다.
	float distance = ((float)(340 * duration) / 10000) / 2;  
	
	Serial.print(distance);
	Serial.println("cm");
	// 수정한 값을 출력

  char s1[10];

  sprintf(s1, "%f", distance);

  // 서버 소켓 보내기
  client.write(s1);
  client.write("\n");

	delay(1400);
}
