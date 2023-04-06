#include<Servo.h> //Servo 라이브러리를 추가
Servo servo;      //Servo 클래스로 servo객체 생성
int value = 0;    // 각도를 조절할 변수 value
int pos = 0;


void setup() {
  servo.attach(D4);     //servo 서보모터 7번 핀에 연결
                       // 이때 ~ 표시가 있는 PWM을 지원하는 디지털 핀에 연결
}


void loop() {

  servo.write(0);
  delay(1300);
  servo.write(170);
  delay(1300);
}