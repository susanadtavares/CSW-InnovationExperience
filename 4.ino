#include <Servo.h>

Servo myservo;
int rightDistance = 0;
int leftDistance = 0;
int middleDistance = 0;
int backDistance = 0;
int flag = 0;
unsigned long space = 0;
unsigned long start_time = 0;

#define speedPinR 3   
#define RightDirectPin1 12   
#define RightDirectPin2 11    
#define speedPinL 6    
#define LeftDirectPin1 7   
#define LeftDirectPin2 8   
#define SERVO_PIN 9  
#define Echo 2 
#define BackRight 0
#define Trig 10  
#define BackLeft 5

void forward(){
  analogWrite(speedPinR, 150);
  analogWrite(speedPinL, 150);
  digitalWrite(LeftDirectPin1, HIGH);
  digitalWrite(LeftDirectPin2, LOW);
  digitalWrite(RightDirectPin2, LOW);
  digitalWrite(RightDirectPin1, HIGH);
}

void back() {
  analogWrite(speedPinR, 95);
  analogWrite(speedPinL, 95);
  digitalWrite(LeftDirectPin1, LOW);
  digitalWrite(LeftDirectPin2, HIGH);
  digitalWrite(RightDirectPin2, HIGH);
  digitalWrite(RightDirectPin1, LOW);
}

void left() {
  analogWrite(speedPinR, 150);
  analogWrite(speedPinL, 150);
  digitalWrite(LeftDirectPin1, LOW);
  digitalWrite(LeftDirectPin2, HIGH);
  digitalWrite(RightDirectPin2, LOW);
  digitalWrite(RightDirectPin1, HIGH);
}

void right() {
  analogWrite(speedPinR, 150);
  analogWrite(speedPinL, 150);
  digitalWrite(LeftDirectPin1, HIGH);
  digitalWrite(LeftDirectPin2, LOW);
  digitalWrite(RightDirectPin2, HIGH);
  digitalWrite(RightDirectPin1, LOW);
}

void stop() {
  digitalWrite(speedPinR, LOW);
  digitalWrite(speedPinL, LOW);
}

int getDistance() {
    digitalWrite(Trig, LOW);
    delayMicroseconds(2);
    digitalWrite(Trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(Trig, LOW);
    return (int)pulseIn(Echo, HIGH) / 58;
}

void setup() {
  myservo.attach(SERVO_PIN);
  Serial.begin(9600);

  pinMode(BackLeft, INPUT);
  pinMode(BackRight, INPUT);
  pinMode(Echo, INPUT);
  pinMode(Trig, OUTPUT);
  pinMode(LeftDirectPin1, OUTPUT);
  pinMode(LeftDirectPin2, OUTPUT);
  pinMode(RightDirectPin1, OUTPUT);
  pinMode(RightDirectPin2, OUTPUT);
  pinMode(speedPinR, OUTPUT);
  pinMode(speedPinL, OUTPUT);

  stop();
}

void loop() {
  myservo.write(180); // servo angle
  delay(500);
  middleDistance=getDistance();
  //backDistance=getBackDistance(); // mudar isto!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

  while(flag==0 && middleDistance < 20){ // looking for parking spot
    forward();
    middleDistance=getDistance();
  }

  while(flag==0 && middleDistance >= 20){ // looking for a spot
    start_time=millis();
    flag=1; 
    forward();
    middleDistance=getDistance();
  }

  while(flag==1 && middleDistance < 20){ // found a spot
    unsigned long cur_time=millis();
    space=(cur_time - start_time);
    stop();
    delay(500);
    
    if(1000<space && space<1300){ // start parking
      flag=2;
      back();
      delay((int)space * 0.2);
    }
  }

  while(flag==2){ // adjusting (turning right)
    right();
    delay(400);
    flag=3;
  }

  while(flag==3){ // adjusting (going back)
    //delay(1200);
    rightDistance = digitalRead(BackRight);
    leftDistance = digitalRead(BackLeft);
    while (rightDistance == 1 && leftDistance == 1 ) {
      back();
      rightDistance = digitalRead(BackRight);
      leftDistance = digitalRead(BackLeft);
    }
    stop();
    left();
    delay(400);
    stop();
    //delay(1000);
    flag=4;
  }

  while(flag==4){ // finish parking mode 
    forward();
    delay(200);
    stop();
    flag=5;
  }

  while (flag==5) {}
}