#include <SoftwareSerial.h>
SoftwareSerial sim900aSerial(6, 7);  // RX, TX

int trig_pin = 8;
int echo_pin = 9;
int pump = 11;
int green = 3;
int yellow = 4;
int red = 5;
long echotime;   //in micro seconds
float distance;  //in cm
float level;     //in cm
int percentage;  //%
char btd;        //bluetooth data

void setup() {
  Serial.begin(9600);
  pinMode(trig_pin, OUTPUT);
  pinMode(echo_pin, INPUT);
  pinMode(pump, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(red, OUTPUT);
  digitalWrite(trig_pin, LOW);  //Start with trigger LOW
  sim900aSerial.begin(9600);    //Start serial communication with SIM900A module
  delay(2000);                  //SIM900A initialization
}

void loop() {

  //trigger a pulse-echo measurement
  digitalWrite(trig_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_pin, LOW);

  //get the result
  echotime = pulseIn(echo_pin, HIGH);
  distance = 0.0001 * ((float)echotime * 340.0) / 2.0;
  level = 23 - distance;
  percentage = round((level / 14) * 100);

  if (percentage <= 5) {
    Serial.println("*YEMPTY*");
    digitalWrite(red, HIGH);
    digitalWrite(yellow, LOW);
    digitalWrite(green, LOW);
  } else if (percentage <= 30) {
    Serial.println("*YLOW*");
    digitalWrite(red, HIGH);
    digitalWrite(yellow, LOW);
    digitalWrite(green, LOW);
  } else if (percentage <= 60) {
    Serial.println("*YMEDIUM*");
    digitalWrite(red, LOW);
    digitalWrite(yellow, HIGH);
    digitalWrite(green, LOW);
  } else if (percentage <= 95) {
    Serial.println("*YHIGH*");
    digitalWrite(red, LOW);
    digitalWrite(yellow, LOW);
    digitalWrite(green, HIGH);
  } else if (percentage <= 100) {
    Serial.println("*YFULL*");
    digitalWrite(red, LOW);
    digitalWrite(yellow, LOW);
    digitalWrite(green, HIGH);
  }

  //send over Bluetooth
  Serial.println("*T" + String(echotime) + "*");
  Serial.println("*D" + String(distance, 1) + "*");
  Serial.println("*W" + String(level) + "*");
  Serial.println("*X" + String(percentage) + "*");

  //recieve over Bluetooth
  if (Serial.available()) {
    btd = Serial.read();
    Serial.println(btd);
  }

  if (percentage <= 5) {
    Serial.begin(9600);
    sim900aSerial.begin(9600);
    delay(2000);
    sim900aSerial.println("AT+CMGF=1");
    delay(1000);
    sim900aSerial.println("AT+CMGS=\"+94714403360\"");
    delay(1000);
    sim900aSerial.println("Warning: Tank EMPTY");
    delay(1000);
    sim900aSerial.write(26);
    delay(1000);
    percentage = 6;
  }

  if (btd == '1') {  //manual pump control
    digitalWrite(pump, HIGH);
    Serial.println("*PON*");
    Serial.println("*LR0G255B0*");
  } else if (btd == '0') {
    digitalWrite(pump, LOW);
    Serial.println("*POFF*");
    Serial.println("*LR255G0B0*");
  } else if (btd == 'A') {  //automatic pump control
    if (percentage <= 10) {
      digitalWrite(pump, HIGH);
      Serial.println("*PON*");
      Serial.println("*LR0G255B0*");
    } else if (percentage >= 100) {
      digitalWrite(pump, LOW);
      Serial.println("*POFF*");
      Serial.println("*LR255G0B0*");
    }
  }


  delay(400);
}
