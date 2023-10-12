#define trig 2
#define echo 4
#define pump 10
char btd;
int levl;

void setup() {
 Serial.begin(9600);
 pinMode(echo,INPUT);//echo pin of ultraSonic
 pinMode(trig,OUTPUT);//trig pin of ultraSonic
 pinMode(pump,OUTPUT);// water pump
}

void loop() {
  digitalWrite(trig, LOW);
  delayMicroseconds(2) ;
  digitalWrite(trig, HIGH);
  delayMicroseconds(10) ;
  digitalWrite(trig, LOW);

  long t = pulseIn(echo, HIGH);
  long depth = t/29/2 ;
  long perc = (25-depth)/20*100;

  if(perc>=100){
    levl = "FULL";
  }
  else if(perc>=75 & perc<=100){
    levl = "HIGH";
  }
  else if(perc>=50 & perc<=75){
    levl = "MID";
  }
  else if(perc>=25 & depth<=50){
    levl = "LOW";
  }
  else{
    levl = "EMPTY";
  }

  Serial.print(depth);
  Serial.println("cm");

  if(perc<=10){
    digitalWrite(pump, HIGH);
  }
  if(perc>=100){
    digitalWrite(pump, LOW);
  }

  if(Serial.available()){
    btd = Serial.read();
    Serial.println(btd);     
  }

  if(btd=='1'){
    digitalWrite(pump, HIGH);
  }
  else if(btd=='2'){
    digitalWrite(pump, LOW);
  }

  if(pump==HIGH){
    Serial.print("pON");
  }
  else{
    Serial.print("pOFF");
  }
}
