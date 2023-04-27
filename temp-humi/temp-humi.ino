#define dhtpin D1
#define dhttype DHT11

#include "SecretConstant.h"
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <stdio.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <DHT.h>

// 와이파이명
const char* ssid = constant_ssid;
const char* password = constant_password;
String humiTempUrl = "(here Your Url)";

DHT dht(dhtpin, dhttype);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(10);
  dht.begin();

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
  Serial.println("Point1");
  if((WiFi.status() == WL_CONNECTED)) {
    WiFiClient wifiClient;
    HTTPClient httpClient;

    // URL은 요청을 보내고자 하는 URL 입력
    httpClient.begin(wifiClient,humiTempUrl);
    // 필요한 헤더 추가
    httpClient.addHeader("Content-Type", "application/json");

    // JSON 오브젝트
    StaticJsonDocument<200> json;

    // 온습도 모듈에서 습도, 온도를 입력받음
    float humi = dht.readHumidity();
    float temp = dht.readTemperature();
    
    // json에 데이터 저장
    json["humidity"] = humi;
    json["temperature"] = temp;

    // 시리얼 모니터에 출력 (생략가능)
    Serial.print("humi : ");
    Serial.println(humi);
    Serial.print("temp : ");
    Serial.println(temp);

    String parsedJsonToString;
    // json을 String으로 변환
    serializeJson(json, parsedJsonToString);

    // POST 요청 보내기
    int httpResponseCode = httpClient.POST(parsedJsonToString);
 
    // HTTP 응답이 왔다면
    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);

      String response = httpClient.getString(); // 응답 본문을 문자열로 읽어옴
      Serial.println(response); // 시리얼 모니터에 출력
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }

    httpClient.end();
    delay(1000);
  }
}
