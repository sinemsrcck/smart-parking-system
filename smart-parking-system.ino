

#include <Servo.h>

// -------------------- PIN TANIMLARI --------------------
const int trigGiris = 2;
const int echoGiris = 3;
const int trigCikis = 4;
const int echoCikis = 5;
const int yesilLedPin = 6;
const int kirmiziLedPin = 7;
const int buzzerPin = 8;
const int servoGirisPin = 9;
const int butonPin = 10;

// -------------------- NESNELER --------------------
Servo servoGiris;


// -------------------- SISTEM DEGISKENLERI --------------------
int toplamKapasite = 4;
int bosYer = 4;

// Mesafe eşikleri (cm)
const int aracAlgilamaMesafesi = 50;
const int aracUzaklasmaMesafesi = 80;

// Araç algılama durum flagleri
bool girisAracAktif = false;
bool cikisAracAktif = false;

/*
  Giriş sensöründen mesafe ölçer
*/
long mesafeOlc() {
  digitalWrite(trigGiris, LOW);
  delayMicroseconds(2);

  digitalWrite(trigGiris, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigGiris, LOW);

  long sure = pulseIn(echoGiris, HIGH, 30000);

  if (sure == 0) {
    return 300;// sensör okumazsa maksimum değer dön
  }

  long mesafe = sure * 0.034 / 2;
  return mesafe;
}

/*
  Gürültüyü azaltmak için 5 ölçümün ortalamasını alır (Giriş)
*/
long ortalamaMesafeOlc() {
  long toplam = 0;
  for (int i = 0; i < 5; i++) {
    toplam += mesafeOlc();
    delay(20);
  }
  return toplam / 5;
}

/*
  Çıkış sensöründen mesafe ölçer
*/
long mesafeOlcCikis() {
  digitalWrite(trigCikis, LOW);
  delayMicroseconds(2);

  digitalWrite(trigCikis, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigCikis, LOW);

  long sure = pulseIn(echoCikis, HIGH, 30000);

  if (sure == 0) {
    return 300;
  }

  long mesafe = sure * 0.034 / 2;
  return mesafe;
}

/*
  Bariyeri açıp kapatır (servo kontrolü)
*/
void bariyerAcKapat() {
  servoGiris.write(80);//aç
  delay(2000);
  servoGiris.write(10);//kapat
}
/*
  Çıkış için ortalama mesafe ölçümü
*/
long ortalamaMesafeOlcCikis() {
  long toplam = 0;
  for (int i = 0; i < 5; i++) {
    toplam += mesafeOlcCikis();
    delay(20);
  }
  return toplam / 5;
}
/*
  LED'leri boş yer durumuna göre günceller
*/
void ledDurumunuGuncelle() {
  if (bosYer > 0) {
    digitalWrite(yesilLedPin, HIGH);
    digitalWrite(kirmiziLedPin, LOW);
  } else {
    digitalWrite(yesilLedPin, LOW);
    digitalWrite(kirmiziLedPin, HIGH);
  }
}
/*
  Sistem başlatma ve pin ayarları
*/
void setup() {
  Serial.begin(9600);

  pinMode(trigGiris, OUTPUT);
  pinMode(echoGiris, INPUT);

  pinMode(trigCikis, OUTPUT);
  pinMode(echoCikis, INPUT);

  pinMode(yesilLedPin, OUTPUT);
  pinMode(kirmiziLedPin, OUTPUT);

  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);

  pinMode(butonPin, INPUT_PULLUP);

  servoGiris.attach(servoGirisPin);
  servoGiris.write(0);

  ledDurumunuGuncelle();

  Serial.println("Akilli Otopark Sistemi Basladi");
  Serial.print("Toplam Kapasite: ");
  Serial.println(toplamKapasite);
  Serial.print("Baslangic Bos Yer: ");
  Serial.println(bosYer);
}
/*
  Ana çalışma döngüsü:
  - Giriş kontrolü
  - Buton kontrolü
  - Çıkış kontrolü
*/
void loop() {
  long girisMesafe = ortalamaMesafeOlc();
 long cikisMesafe = ortalamaMesafeOlcCikis();

  Serial.print("Giris Mesafe: ");
  Serial.print(girisMesafe);
  Serial.print(" | Cikis Mesafe: ");
  Serial.print(cikisMesafe);
  Serial.print(" | Bos Yer: ");
  Serial.println(bosYer);

  // GIRIS
  if (!girisAracAktif && girisMesafe <= aracAlgilamaMesafesi) {
    girisAracAktif = true;

    if (bosYer > 0) {
      Serial.println("Arac algilandi. Bariyer aciliyor...");
      bariyerAcKapat();

      bosYer--;
      ledDurumunuGuncelle();

      Serial.print("Arac giris yapti. Kalan bos yer: ");
      Serial.println(bosYer);
    } else {
      Serial.println("Otopark dolu. Girise izin verilmedi.");

      digitalWrite(buzzerPin, HIGH);
      delay(500);
      digitalWrite(buzzerPin, LOW);

      ledDurumunuGuncelle();
    }
  }

  if (girisAracAktif && girisMesafe >= aracUzaklasmaMesafesi) {
    girisAracAktif = false;
    Serial.println("Giris sistemi yeni arac icin hazir.");
  }

  // BUTON
  if (digitalRead(butonPin) == LOW) {
    if (bosYer > 0) {
      Serial.println("Buton ile bariyer acildi!");
      bariyerAcKapat();
      bosYer--;
    ledDurumunuGuncelle();
    } else {
      Serial.println("Otopark dolu. Buton ile acma engellendi.");
      digitalWrite(buzzerPin, HIGH);
      delay(300);
      digitalWrite(buzzerPin, LOW);
    }
    delay(300);
  }

  // CIKIS
  if (!cikisAracAktif && cikisMesafe <= aracAlgilamaMesafesi) {
    cikisAracAktif = true;

   if (bosYer < toplamKapasite) {
    Serial.println("Arac cikis yapti.");
    bariyerAcKapat();
    bosYer++;
    ledDurumunuGuncelle();

      Serial.print("Yeni bos yer sayisi: ");
      Serial.println(bosYer);
    }
  }

  if (cikisAracAktif && cikisMesafe >= aracUzaklasmaMesafesi) {
    cikisAracAktif = false;
    Serial.println("Cikis sistemi yeni arac icin hazir.");
  }

  delay(300);
}
