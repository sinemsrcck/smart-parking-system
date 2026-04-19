# Akıllı Otopark Sistemi 🚗

Bu proje Arduino kullanılarak geliştirilmiş bir otopark otomasyon sistemidir. 
Sensörler ile araç giriş-çıkışı algılanır ve servo motor ile bariyer kontrolü sağlanır.

## Özellikler
- Giriş ve çıkış için ultrasonik sensör kullanımı
- Servo motor ile bariyer kontrolü
- LED ile doluluk durumu gösterimi
- Buzzer ile uyarı sistemi
- Otopark kapasite takibi

## Kullanılan Donanımlar
- Arduino Uno
- Ultrasonik sensör (HC-SR04)
- Servo motor
- LED (Kırmızı & Yeşil)
- Buzzer
- Buton

## Pin Bağlantıları
- trigGiris → 2
- echoGiris → 3
- trigCikis → 4
- echoCikis → 5
- Yeşil LED → 6
- Kırmızı LED → 7
- Buzzer → 8
- Servo → 9
- Buton → 10

## Nasıl Çalışır?
- Araç giriş sensörüne yaklaştığında sistem algılar
- Boş yer varsa bariyer açılır
- Araç çıktıktan sonra boş yer sayısı güncellenir
- Otopark doluysa giriş engellenir ve uyarı verilir

## Kurulum
1. Arduino IDE ile `.ino` dosyasını aç
2. Kart ve port seçimini yap
3. Kodu Arduino'ya yükle
4. Devre bağlantılarını tamamla
