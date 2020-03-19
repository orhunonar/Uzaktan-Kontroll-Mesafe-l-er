
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

String data;
String cata;

int trigPin = 3;
int echoPin = 2;

long sure;
long uzaklik;

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position
RF24 radio(9, 10); // CE, CSN
const byte address[6] = "00001";

void setup() {
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
  myservo.attach(6);  // attaches the servo on pin 9 to the servo object
  pinMode(trigPin, OUTPUT); /* trig pini çıkış olarak ayarlandı */
  pinMode(echoPin, INPUT); /* echo pini giriş olarak ayarlandı */
  Serial.begin(9600);
radio.setChannel(101);

}
void loop() {
  while (Serial.available()) {
    delay(3);
    char c = Serial.read();
    data += c;
  }
  if (data == "ON")
  { cata = "";
    cata += data;
    data = "";
  }
  if (data == "OFF") {
    cata = "";
    cata += data;
    data = "";
  } if (cata == "ON") {
    for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
      // in steps of 1 degree
      myservo.write(pos);    // tell servo to go to position in variable 'pos'
      digitalWrite(trigPin, LOW); /* sensör pasif hale getirildi */
      delayMicroseconds(5);
      digitalWrite(trigPin, HIGH); /* Sensore ses dalgasının üretmesi için emir verildi */
      delayMicroseconds(10);
      digitalWrite(trigPin, LOW);  /* Yeni dalgaların üretilmemesi için trig pini LOW konumuna getirildi */
      sure = pulseIn(echoPin, HIGH); /* ses dalgasının geri dönmesi için geçen sure ölçülüyor */
      uzaklik = sure / 29.1 / 2; /* ölçülen sure uzaklığa çevriliyor */
     Serial.println(" " );
      Serial.println(uzaklik); /* hesaplanan uzaklık bilgisayara aktarılıyor */
     
      delay(10);                       // waits 15ms for the servo to reach the position
      unsigned char text = uzaklik;
      radio.write(&text, sizeof(unsigned char));
      delay(10);
    }
    for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      digitalWrite(trigPin, LOW); /* sensör pasif hale getirildi */
      delayMicroseconds(5);
      digitalWrite(trigPin, HIGH); /* Sensore ses dalgasının üretmesi için emir verildi */
      delayMicroseconds(10);
      digitalWrite(trigPin, LOW);  /* Yeni dalgaların üretilmemesi için trig pini LOW konumuna getirildi */
      sure = pulseIn(echoPin, HIGH); /* ses dalgasının geri dönmesi için geçen sure ölçülüyor */
      uzaklik = sure / 29.1 / 2; /* ölçülen sure uzaklığa çevriliyor */
      Serial.println(" " );
      Serial.println(uzaklik); /* hesaplanan uzaklık bilgisayara aktarılıyor */
    
    
      delay(10);                       // waits 15ms for the servo to reach the position
      unsigned char text = uzaklik;
      radio.write(&text, sizeof(unsigned char));
      delay(10);
    }
  }
  if (cata == "OFF") {
    pos = 0;
  }

}
