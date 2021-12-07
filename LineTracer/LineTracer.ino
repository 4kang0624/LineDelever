#include <Servo.h>
#define servoRightPin 12
#define servoLeftPin 13
#define trigPin 11                           // trigPin을 11으로 설정합니다.
#define echoPin 10                            // echoPin을 10로 설정합니다.
#define sensorRight 7
#define sensorLeft 6

Servo servoLeft;                             // Declare left and right servos
Servo servoRight;
int sensorRightVal = HIGH;  // HIGH는 장애물이 없음을 뜻함
int sensorLeftVal = HIGH;  // HIGH는 장애물이 없음을 뜻함

void straight();
void stopMotor();
void rightTurn();
void leftTurn();

void setup() {
  Serial.begin (9600);              // 시리얼 모니터를 사용하기 위해 보드레이트를 9600으로 설정합니다.
  pinMode(trigPin, OUTPUT);         // trigPin 핀을 출력핀으로 설정합니다.
  pinMode(echoPin, INPUT);          // echoPin 핀을 입력핀으로 설정합니다.
  tone(4, 3000, 1000);
  delay(1000);
  pinMode(sensorRight, INPUT);           //적외선 센서
  pinMode(sensorRight, INPUT);  
  servoLeft.attach(servoLeftPin);
  servoRight.attach(servoRightPin);
}  

void loop() {
  long duration, distance;                   // 각 변수를 선언합니다.
  digitalWrite(trigPin, LOW);                 // trigPin에 LOW를 출력하고
  delayMicroseconds(2);                    // 2 마이크로초가 지나면
  digitalWrite(trigPin, HIGH);                // trigPin에 HIGH를 출력합니다.
  delayMicroseconds(10);                  // trigPin을 10마이크로초 동안 기다렸다가
  digitalWrite(trigPin, LOW);                // trigPin에 LOW를 출력합니다.
  duration = pulseIn(echoPin, HIGH);   // echoPin핀에서 펄스값을 받아옵니다.
  distance = duration * 17 / 1000;          //  duration을 연산하여 센싱한 거리값을 distance에 저장합니다.

  // 거리가 25cm가 넘으면
  if (distance >= 25) {
    sensorRightVal = digitalRead(sensorRight);
    sensorLeftVal = digitalRead(sensorLeft);

    if (sensorRightVal == LOW && sensorLeftVal == LOW){
      servoLeft.detach();
      servoRight.detach();
    }
    else if (sensorRightVal == LOW) { 
      rightTurn(); 
    }
    else if (sensorLeftVal == LOW){
      leftTurn();
    }
    else {
      straight();
    }
  }
  else {
    stopMotor();
  }
  
  delay(500);
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
