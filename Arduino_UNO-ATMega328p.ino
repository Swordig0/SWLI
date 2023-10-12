int trig_pin = 8;
int echo_pin = 9;
int pump = 11;
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
  level = 23 - distance;
  percentage = round((level /14) * 100);

  if (percentage <= 5) {
    Serial.println("*YEMPTY*");
  } else if (percentage <= 30) {
    Serial.println("*YLOW*");
  } else if (percentage <= 60) {
    Serial.println("*YMEDIUM*");
  } else if (percentage <= 95) {
    Serial.println("*YHIGH*");
  } else if (percentage <= 100) {
    Serial.println("*YFULL*");
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
