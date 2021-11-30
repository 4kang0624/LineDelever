#include <Servo.h>                           // Include servo library
#define trigPin 11                           // trigPin을 11으로 설정합니다.
#define echoPin 10                            // echoPin을 10로 설정합니다.

Servo servoLeft;                             // Declare left and right servos
Servo servoRight;
int senceRpin = 7;  // This is our input pin
int senceR = HIGH;  // HIGH MEANS NO OBSTACLE

void setup()                                 // Built-in initialization block
{
  Serial.begin (9600);              // 시리얼 모니터를 사용하기 위해 보드레이트를 9600으로 설정합니다.
  pinMode(trigPin, OUTPUT);   // trigPin 핀을 출력핀으로 설정합니다.
  pinMode(echoPin, INPUT);    // echoPin 핀을 입력핀으로 설정합니다.
  tone(4, 3000, 1000);                       // Play tone for 1 second
  delay(1000);                               // Delay to finish tone
  pinMode(senceRpin, INPUT);           //적외선 센서
  servoLeft.attach(13);                      // Attach left signal to pin 13
  servoRight.attach(12);                     // Attach right signal to pin 12

  // Full speed forward
  /*servoLeft.writeMicroseconds(1700);         // 전진
  servoRight.writeMicroseconds(1300);        
  delay(3000);                               
   servoLeft.writeMicroseconds(1300);        //후진
  servoRight.writeMicroseconds(1700);
  delay(3000);
   servoLeft.writeMicroseconds(1700);       //우회전
  servoRight.writeMicroseconds(1700);
  delay(3000);
   servoLeft.writeMicroseconds(1300);       //좌회전
  servoRight.writeMicroseconds(1300);
  delay(3000);
 
  servoLeft.detach();                        // Stop sending servo signals
  servoRight.detach();
  */
}  

void loop()

{
  long duration, distance;                   // 각 변수를 선언합니다.
  digitalWrite(trigPin, LOW);                 // trigPin에 LOW를 출력하고
  delayMicroseconds(2);                    // 2 마이크로초가 지나면
  digitalWrite(trigPin, HIGH);                // trigPin에 HIGH를 출력합니다.
  delayMicroseconds(10);                  // trigPin을 10마이크로초 동안 기다렸다가
  digitalWrite(trigPin, LOW);                // trigPin에 LOW를 출력합니다.
  duration = pulseIn(echoPin, HIGH);   // echoPin핀에서 펄스값을 받아옵니다.
  distance = duration * 17 / 1000;          //  duration을 연산하여 센싱한 거리값을 distance에 저장합니다.

  if (distance >= 15)       // 거리가 15cm가 넘으면
  {
    servoLeft.writeMicroseconds(1700);         // 전진
    servoRight.writeMicroseconds(1300);
    senceR = digitalRead(senceRpin);
    if (senceR == LOW) { 
      servoLeft.writeMicroseconds(1700);         // 우회전
      servoRight.writeMicroseconds(1530);   
    }
    else {
      servoLeft.writeMicroseconds(1700);         // 전진
      servoRight.writeMicroseconds(1300);
    } 
  }
  else                                                    // 거리가 15cm가 넘지 않거나 0보다 작지 않으면
  {
    servoLeft.writeMicroseconds(1525);         // 멈춤
    servoRight.writeMicroseconds(1530);
  }
    delay(500);
  
  }
