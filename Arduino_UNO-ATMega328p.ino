#define trig 8
#define echo 7
void setup() {
 Serial.begin(9600);
 pinMode(7,INPUT);//echo pin of ultraSonic
 pinMode(8,OUTPUT);//trig pin of ultraSonic
 pinMode(10,OUTPUT);// relay
 pinMode(9,OUTPUT);// buzzer pin // put your setup code here, to run once:

}

void loop() {
  digitalWrite(trig, LOW);
  delayMicroseconds(2) ;
  digitalWrite(trig, HIGH);
  delayMicroseconds(10) ;
  digitalWrite(trig, LOW);

  long t = pulseIn(echo, HIGH);
  long cm = t/29/2 ;

  Serial.print(cm);
  Serial.print(\n);
  delay(10);  

  if(Serial.available()){
    char val = Serial.read();
    Serial.println(val);     
  }

  if(val=='1')
    digitalWrite()
}
