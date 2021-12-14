#include <Servo.h>
#define servoRightPin 12    // 오른쪽 모터 -> D12
#define servoLeftPin 13     // 왼쪽 모터 -> D13
#define trigPin 11          // 초음파 센서 trig -> D11
#define echoPin 10          // 초음파 센서 echo -> D10
#define sensorRight 7       // 오른쪽 적외선 센서 -> D7
#define sensorLeft 6        // 왼쪽 적외선 센서 -> D6

// 모터 선언 및 센서값 변수 초기화
Servo servoLeft;
Servo servoRight;
int sensorRightVal = LOW;   // 적외선 센서 기본값 = LOW
int sensorLeftVal = LOW;    // 적외선 센서 기본값 = LOW

// 모터 작동 함수 선언
void straight();
void stopMotor();
void rightTurn();
void leftTurn();

void setup() {
  Serial.begin (9600);              // 테스트 중 시리얼 모니터를 사용하기 위해 보드레이트를 9600으로 설정
  pinMode(trigPin, OUTPUT);         // trigPin 핀을 출력핀으로 설정
  pinMode(echoPin, INPUT);          // echoPin 핀을 입력핀으로 설정
  tone(4, 3000, 1000);
  delay(1000);
  pinMode(sensorRight, INPUT);      // 오른쪽 적외선 센서를 입력핀으로 설정
  pinMode(sensorLeft, INPUT);      // 왼쪽 적외선 센서를 입력핀으로 설정
  servoLeft.attach(servoLeftPin);
  servoRight.attach(servoRightPin);
}  

void loop() {
  long duration, distance;
  digitalWrite(trigPin, LOW);             // trigPin에 LOW를 출력
  delayMicroseconds(2);                   // 2ms 딜레이
  digitalWrite(trigPin, HIGH);            // trigPin에 HIGH를 출력
  delayMicroseconds(10);                  // 10ms 딜레이
  digitalWrite(trigPin, LOW);             // trigPin에 LOW를 출력
  duration = pulseIn(echoPin, HIGH);      // echoPin핀에서 펄스값 받기
  distance = duration * 17 / 1000;        //  duration을 연산하여 센싱한 거리값을 distance에 저장

  // 거리가 15cm가 넘으면
  if (distance >= 15) {
    sensorRightVal = digitalRead(sensorRight);
    sensorLeftVal = digitalRead(sensorLeft);

    // 적외선센서가 검정색 인식 -> HIGH
    if (sensorRightVal == HIGH && sensorLeftVal == HIGH){
      // 모터 작동 중지
      servoLeft.detach();
      servoRight.detach();
    }
    else if (sensorRightVal == HIGH) { 
      rightTurn(); 
    }
    else if (sensorLeftVal == HIGH){
      leftTurn();
    }
    else {
      straight();
    }
  }
  else {
    stopMotor();
  }
  
}

// 전진
void straight() {
  servoLeft.writeMicroseconds(1700);
  servoRight.writeMicroseconds(1300);
}

// 정지
void stopMotor() {
  servoLeft.writeMicroseconds(1525);
  servoRight.writeMicroseconds(1530);
}

// 우회전
void rightTurn() {
  servoLeft.writeMicroseconds(1700);
  servoRight.writeMicroseconds(1530);
}

// 좌회전
void leftTurn() {
  servoLeft.writeMicroseconds(1525);
  servoRight.writeMicroseconds(1300);
}
