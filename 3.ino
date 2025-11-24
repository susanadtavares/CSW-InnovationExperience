#include <Servo.h>

// const
const unsigned long READ_INTERVAL = 60;
const unsigned long LOWER_DISTANCE_THRESH = 30; // in cm
const unsigned long HIGHER_DISTANCE_THRESH = 30;

// globals
int flag = 0;
int distance = 0;
unsigned long lastReadTime = 0;

#define speedPinR 3   // e1
#define RightDirectPin1  12    //  m3
#define RightDirectPin2  11    // m4
#define speedPinL 6        //  e2
#define LeftDirectPin1  7    // m1
#define LeftDirectPin2  8   // m2

// Ultrasonic Sensor Pins
#define echoPin 2
#define trigPin 10

// Servo
Servo myservo;
#define servoPin 9

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
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    return (int)pulseIn(echoPin, HIGH) / 58;
}

void setup(){
  myservo.attach(servoPin);

  pinMode(LeftDirectPin1, OUTPUT);
  pinMode(LeftDirectPin2, OUTPUT);
  pinMode(RightDirectPin1, OUTPUT);
  pinMode(RightDirectPin2, OUTPUT);
  pinMode(speedPinR, OUTPUT);
  pinMode(speedPinL, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);

  stop();
}



void loop(){
  
  // wall search
  while(flag == 0){
    myservo.write(0);
    delay(100);
    forward();
    delay(100);
    stop();

    // right
    if(getDistance() <= LOWER_DISTANCE_THRESH){flag =  1; break;}
  }

  // wall follow
  while(flag == 1){
    forward();

    unsigned long currentTime = millis();
    if(currentTime - lastReadTime >= READ_INTERVAL){
      lastReadTime = currentTime;
      distance = getDistance();
    }

    if(distance > HIGHER_DISTANCE_THRESH){flag = 2; break;}
  }

  // wall end (turn)
  while(flag == 2){
    forward();
    delay(200);
    stop();

    right();
    delay(700);
    stop();

    forward();
    delay(450);
    stop();

    right();
    delay(700);
    stop();

    flag = 3;
  }

  while(flag == 3){
    myservo.write(180);
    if(getDistance() <= LOWER_DISTANCE_THRESH){flag =  4; break;}
  }

  while(flag == 4){
    forward();

    unsigned long currentTime = millis();
    if(currentTime - lastReadTime >= READ_INTERVAL){
      lastReadTime = currentTime;
      distance = getDistance();
    }

    if(distance > HIGHER_DISTANCE_THRESH){flag = 5; break;}
  }

  while(flag == 5){
    forward();
    delay(200);
    stop();

    left();
    delay(700);
    stop();

    forward();
    delay(450);
    stop();
 
    left();
    delay(700);
    stop();
    flag = 6;
  }

  while(flag == 6){
    forward();

    unsigned long currentTime = millis();
    if(currentTime - lastReadTime >= READ_INTERVAL){
      lastReadTime = currentTime;
      distance = getDistance();
    }

    if(distance > HIGHER_DISTANCE_THRESH){forward(); delay(150); stop(); flag = 7;}
  }

  while(flag == 7){delay(50000);}

}