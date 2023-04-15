// BTS7960B Motor Sürücüsü
int PWMA = 3; // PWM sinyali çıkışı
int AIN1 = 9; // Yön kontrolü 1
int AIN2 = 8; // Yön kontrolü 2

// IR Kumanda
#include <IRremote.h>
int IR_RECEIVE_PIN = 11; // IR alıcı pini
IRrecv irrecv(IR_RECEIVE_PIN);
decode_results results;

// Motor ayarları
int motorHiz = 0;
int motorYon = 0;
int motorZaman = 0;

void setup() {
  // IR Kumanda
  irrecv.enableIRIn();
  Serial.begin(9600);

  // Motor Sürücüsü
  pinMode(PWMA, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
}

void loop() {
  // IR Kumanda sinyalini alıyoruz.
  if (irrecv.decode(&results)) {
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

  // Motor çalıştırıyom.
  if (motorZaman > 0) {
    digitalWrite(AIN1, motorYon == 1 ? HIGH : LOW);
    digitalWrite(AIN2, motorYon == -1 ? HIGH : LOW);
    analogWrite(PWMA, motorHiz);
    delay(motorZaman * 1000);
    analogWrite(PWMA, 0);
    motorZaman = 0;
  }
}
