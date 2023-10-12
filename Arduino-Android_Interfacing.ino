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
  depth = 55 - distance;
  percentage = round((depth / 50) * 100);

  //send over Bluetooth
  Serial.println("*T" + String(echotime) + "*");
  Serial.println("*D" + String(distance, 1) + "*");
  Serial.println("*W" + String(depth) + "*");
  Serial.println("*X" + String(percentage) + "*");


  //recieve over Bluetooth
  if (Serial.available()) {
    btd = Serial.read();
    Serial.println(btd);
  }

  //manual pump control
  if (btd == '1') {
    digitalWrite(pump, HIGH);
  } else if (btd == '0') {
    digitalWrite(pump, LOW);
  }

  //automatic pump control
  if (btd != '1' && btd != '0') {
    if (percentage <= 10) {
      digitalWrite(pump, HIGH);
    } else if (percentage >= 100) {
      digitalWrite(pump, LOW);
    }
  }


  delay(100);
}
