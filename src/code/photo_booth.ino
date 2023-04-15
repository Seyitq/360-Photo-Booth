#include <IRremote.h>

// IR kumanda
int RECV_PIN = 11;
IRrecv irrecv(RECV_PIN);
decode_results results;

// Motor kontrol
const int enableA = 9;
const int in1 = 7;
const int in2 = 8;

// Ayarlar
int motorHiz = 0;
int motorYon = 0;
int motorZaman = 0;

void setup() {
  Serial.begin(9600);

  // IR kumanda
  irrecv.enableIRIn();

  // Motor kontrol
  pinMode(enableA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
}

void loop() {
  // IR kumanda sinyalini alıyorum
  if (irrecv.decode(&results)) {
    // Kumandanın gönderdiği sinyal işliyorum.
    switch(results.value) {
      // Hız ayarı
      case 0xFF629D: // Yavaş
        motorHiz = 100;
        break;
      case 0xFFE21D: // Orta
        motorHiz = 200;
        break;
      case 0xFF22DD: // Hızlı
        motorHiz = 255;
        break;

      // Yön ayarı
      case 0xFF02FD: // İleri
        motorYon = 1;
        break;
      case 0xFFC23D: // Geri
        motorYon = -1;
        break;

      // Zaman ayarı
      case 0xFF10EF: // 5sn
        motorZaman = 5;
        break;
      case 0xFF5AA5: // 10sn
        motorZaman = 10;
        break;
      case 0xFF4AB5: // 15sn
        motorZaman = 15;
        break;

      default:
        break;
    }

    irrecv.resume(); // IR kumanda için 
  }

  // Motor çalıştırırım
  if (motorZaman > 0) {
    digitalWrite(in1, motorYon == 1 ? HIGH : LOW);
    digitalWrite(in2, motorYon == -1 ? HIGH : LOW);
    analogWrite(enableA, motorHiz);
    delay(motorZaman * 1000);
    analogWrite(enableA, 0);
    motorZaman = 0;
  }
}
