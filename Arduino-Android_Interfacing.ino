int trig_pin = 8;
int echo_pin = 9;
int pump = 11;
long echotime;   //in micro seconds
float distance;  //in cm
float depth;     //in cm
int percentage;  //%
char btd;        //bluetooth data

void setup() {
  Serial.begin(9600);
  pinMode(trig_pin, OUTPUT);
  pinMode(echo_pin, INPUT);
  digitalWrite(trig_pin, LOW);  //Start with trigger LOW
}

void loop() {

  //trigger a pulse-echo measurement
  digitalWrite(trig_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_pin, LOW);

  //get the result
  echotime = pulseIn(echo_pin, HIGH);
  distance = 0.0001 * ((float)echotime * 340.0) / 2.0;
  depth = 55 - distance;
  percentage = round((depth / 50) * 100);

  //send over Bluetooth
  Serial.print("*T" + String(echotime) + "*");
  Serial.print("*D" + String(distance, 1) + "*");
  Serial.print("*W" + String(depth) + "*");
  Serial.print("*X" + String(percentage) + "*");

  if (depth < 10) {
    Serial.print("*LR255G0B0*");  //Red
    Serial.print("*YLOW*");
  }
  if (depth >= 10 && depth <= 30) {
    Serial.print("*LR255G200B0*");  //Orange
    Serial.print("*YMEDIUM*");
  }
  if (depth > 30) {
    Serial.print("*LR0G255B0*");  //Green
    Serial.print("*YHIGH*");
  }
  if (pump == HIGH) {
    Serial.print("*PON*");
    Serial.print("*pR0G255B0*");  //Green
  } else {
    Serial.print("*POFF*");
    Serial.print("*pR255G0B0*");  //Red
  }

  /*
  //recieve over Bluetooth
  if (Serial.available()) {
    btd = Serial.read();
    Serial.println(btd);
  }

  //manual pump control
  if (btd == "1") {
    digitalWrite(pump, HIGH);
  } else if (btd == "0") {
    digitalWrite(pump, LOW);
  }

  //automatic pump control
  if (btd != "1" && btd != "0") {
    if (percentage <= 10) {
      digitalWrite(pump, HIGH);
    } else if (percentage >= 100) {
      digitalWrite(pump, LOW);
    }
  }*/

  delay(100);
}