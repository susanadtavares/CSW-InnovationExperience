//Right Motor connects to pins 12 and 11
//Left Motor connects to pins 7 and 8
#include <Servo.h>

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

//**********5 Channel IR Sensor Connection**********//
#define ir1 A0
#define ir2 A1
#define ir3 A2
#define ir4 A3
#define ir5 A4
//*************************************************//

#define OBSTACLE_THRESHOLD 15
const unsigned long READ_INTERVAL = 70;
unsigned long lastReadTime = 0;

long readUltrasonicDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  return pulseIn(echoPin, HIGH) * 0.034 / 2;
}

void turnLeft() {
  analogWrite(speedPinR, 150);
  analogWrite(speedPinL, 150);
  digitalWrite(LeftDirectPin1, LOW);
  digitalWrite(LeftDirectPin2, HIGH);
  digitalWrite(RightDirectPin1, HIGH);
  digitalWrite(RightDirectPin2, LOW);
}

void turnRight() {
  analogWrite(speedPinR, 150);
  analogWrite(speedPinL, 150);
  digitalWrite(LeftDirectPin1, HIGH);
  digitalWrite(LeftDirectPin2, LOW);
  digitalWrite(RightDirectPin1, LOW);
  digitalWrite(RightDirectPin2, HIGH);
}

void stopMotors() {
  digitalWrite(LeftDirectPin1, LOW);
  digitalWrite(LeftDirectPin2, LOW);
  digitalWrite(RightDirectPin1, LOW);
  digitalWrite(RightDirectPin2, LOW);
}

void forward() {
  analogWrite(speedPinR, 120);
  analogWrite(speedPinL, 120);
  digitalWrite(LeftDirectPin1, HIGH);
  digitalWrite(LeftDirectPin2, LOW);
  digitalWrite(RightDirectPin1, HIGH);
  digitalWrite(RightDirectPin2, LOW);
}

//*************************************************//

void setup() {
  pinMode(LeftDirectPin1, OUTPUT);
  pinMode(LeftDirectPin2, OUTPUT);
  pinMode(RightDirectPin1, OUTPUT);
  pinMode(RightDirectPin2, OUTPUT);
  pinMode(speedPinR, OUTPUT);
  pinMode(speedPinL, OUTPUT);
  pinMode(ir1, INPUT);
  pinMode(ir2, INPUT);
  pinMode(ir3, INPUT);
  pinMode(ir4, INPUT);
  pinMode(ir5, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  unsigned long currentTime = millis();
  long distance;

  if(currentTime - lastReadTime >= READ_INTERVAL){
    lastReadTime = currentTime;
    distance = readUltrasonicDistance();

    Serial.println(distance);
    Serial.print("\n");
  }


  if(distance < OBSTACLE_THRESHOLD){
    stopMotors();
    delay(200);

    turnLeft();
    delay(650);
    stopMotors();

    forward();
    delay(400);
    stopMotors();

    turnRight();
    delay(650);
    stopMotors();

    forward();
    delay(550);
    stopMotors();

    turnRight();
    delay(650);
    stopMotors();

    forward();
    delay(400);
    stopMotors();

    turnLeft();
    delay(650);
    stopMotors();
  }


  //Reading Sensor Values
  int s1 = digitalRead(ir1);  //Left Most Sensor
  int s2 = digitalRead(ir2);  //Left Sensor
  int s3 = digitalRead(ir3);  //Middle Sensor
  int s4 = digitalRead(ir4);  //Right Sensor
  int s5 = digitalRead(ir5);  //Right Most Sensor
 
 
  //if only middle sensor detects black line
  if((s1 == 1) && (s2 == 1) && (s3 == 0) && (s4 == 1) && (s5 == 1))
  {
    //going forward with full speed 
    analogWrite(speedPinR, 120); //you can adjust the speed of the motors from 0-255
    analogWrite(speedPinL, 120); //you can adjust the speed of the motors from 0-255
    digitalWrite(LeftDirectPin1, HIGH);
    digitalWrite(LeftDirectPin2, LOW);
    digitalWrite(RightDirectPin1, HIGH);
    digitalWrite(RightDirectPin2, LOW);
  }
  
  //if only left sensor detects black line
  if((s1 == 1) && (s2 == 0) && (s3 == 1) && (s4 == 1) && (s5 == 1))
  {
    //going left
    analogWrite(speedPinR, 150); //you can adjust the speed of the motors from 0-255
    analogWrite(speedPinL, 150); //you can adjust the speed of the motors from 0-255
    digitalWrite(LeftDirectPin1, LOW);
    digitalWrite(LeftDirectPin2, LOW);
    digitalWrite(RightDirectPin1, HIGH);
    digitalWrite(RightDirectPin2, LOW);
    

  }
  
  //if only left most sensor detects black line
  if((s1 == 0) && (s2 == 1) && (s3 == 1) && (s4 == 1) && (s5 == 1))
  {
    //going left with full speed 
    analogWrite(speedPinR, 180); //you can adjust the speed of the motors from 0-255
    analogWrite(speedPinL, 180); //you can adjust the speed of the motors from 0-255
    digitalWrite(LeftDirectPin1, LOW);
    digitalWrite(LeftDirectPin2, HIGH);
    digitalWrite(RightDirectPin1, HIGH);
    digitalWrite(RightDirectPin2, LOW);
    

  }

  //if only right sensor detects black line
  if((s1 == 1) && (s2 == 1) && (s3 == 1) && (s4 == 0) && (s5 == 1))
  {
    //going right
    analogWrite(speedPinR, 150); //you can adjust the speed of the motors from 0-255
    analogWrite(speedPinL, 150); //you can adjust the speed of the motors from 0-255
    digitalWrite(LeftDirectPin1, HIGH);
    digitalWrite(LeftDirectPin2, LOW);
    digitalWrite(RightDirectPin1, LOW);
    digitalWrite(RightDirectPin2, LOW);
  }

  //if only right most sensor detects black line
  if((s1 == 1) && (s2 == 1) && (s3 == 1) && (s4 == 1) && (s5 == 0))
  {
    //going right with full speed 
    analogWrite(speedPinR, 180); //you can adjust the speed of the motors from 0-255
    analogWrite(speedPinL, 180); //you can adjust the speed of the motors from 0-255
    digitalWrite(LeftDirectPin1, HIGH);
    digitalWrite(LeftDirectPin2, LOW);
    digitalWrite(RightDirectPin1, LOW);
    digitalWrite(RightDirectPin2, HIGH);
  }

  //if middle and right sensor detects black line
  if((s1 == 1) && (s2 == 1) && (s3 == 0) && (s4 == 0) && (s5 == 1))
  {
    //going right
    analogWrite(speedPinR, 150); //you can adjust the speed of the motors from 0-255
    analogWrite(speedPinL, 150); //you can adjust the speed of the motors from 0-255
    digitalWrite(LeftDirectPin1, HIGH);
    digitalWrite(LeftDirectPin2, LOW);
    digitalWrite(RightDirectPin1, LOW);
    digitalWrite(RightDirectPin2, LOW);
  }

  //if middle and left sensor detects black line
  if((s1 == 1) && (s2 == 0) && (s3 == 0) && (s4 == 1) && (s5 == 1))
  {
    //going left
    analogWrite(speedPinR, 150); //you can adjust the speed of the motors from 0-255
    analogWrite(speedPinL, 150); //you can adjust the speed of the motors from 0-255
    digitalWrite(LeftDirectPin1, LOW);
    digitalWrite(LeftDirectPin2, LOW);
    digitalWrite(RightDirectPin1, HIGH);
    digitalWrite(RightDirectPin2, LOW);
  }

  //if middle, left and left most sensor detects black line
  if((s1 == 0) && (s2 == 0) && (s3 == 0) && (s4 == 1) && (s5 == 1))
  {
    //going left
    analogWrite(speedPinR, 150); //you can adjust the speed of the motors from 0-255
    analogWrite(speedPinL, 150); //you can adjust the speed of the motors from 0-255
    digitalWrite(LeftDirectPin1, LOW);
    digitalWrite(LeftDirectPin2, LOW);
    digitalWrite(RightDirectPin1, HIGH);
    digitalWrite(RightDirectPin2, LOW);
  }

  //if middle, right and right most sensor detects black line
  if((s1 == 1) && (s2 == 1) && (s3 == 0) && (s4 == 0) && (s5 == 0))
  {
    //going right
    analogWrite(speedPinR, 150); //you can adjust the speed of the motors from 0-255
    analogWrite(speedPinL, 150); //you can adjust the speed of the motors from 0-255
    digitalWrite(LeftDirectPin1, HIGH);
    digitalWrite(LeftDirectPin2, LOW);
    digitalWrite(RightDirectPin1, LOW);
    digitalWrite(RightDirectPin2, LOW);
  }

  //if all sensors are on a black line
  if((s1 == 0) && (s2 == 0) && (s3 == 0) && (s4 == 0) && (s5 == 0))
  {
    //stop
    digitalWrite(LeftDirectPin1, LOW);
    digitalWrite(LeftDirectPin2, LOW);
    digitalWrite(RightDirectPin1, LOW);
    digitalWrite(RightDirectPin2, LOW);
  }

  if((s1 == 1) && (s2 == 1) && (s3 == 1) && (s4 == 1) && (s5 == 1))
  {
    //stop
    digitalWrite(LeftDirectPin1, LOW);
    digitalWrite(LeftDirectPin2, LOW);
    digitalWrite(RightDirectPin1, LOW);
    digitalWrite(RightDirectPin2, LOW);
  }
}