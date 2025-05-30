# STM32 LTC2631 DAC Kontrol Kütüphanesi

Bu proje, **STM32CubeIDE** kullanılarak STM32 mikrodenetleyicisi ile **LTC2631 DAC** entegresinin I2C üzerinden haberleşmesini sağlar. 12-bit dijital değer gönderme veya belirli bir çıkış voltajı ayarlama işlevlerini içerir.

## Özellikler

- LTC2631 (Z veya M  tipi) ile I2C üzerinden haberleşme
- DAC çıkışını şu yöntemlerle ayarlama:
- resolutiona göre  dijital değer gönderme
- Hedef çıkış voltajı belirleme (dahili veya harici referans ile)
- Hafif ve kolay entegre edilebilir yapı
- STM32Cube HAL  ile uyumlu C dilinde yazılmıştır



### 2. Bağlantı  (I2C)

| LTC2631 Pin | STM32 Pin |
|-------------|-----------|
| SDA         | I2C SDA   |
| SCL         | I2C SCL   |
| VCC         | 3.3V      |
| GND         | GND       |


# STM32 ile LTC2631 DAC  Kütüphanesi

Bu proje, STM32 mikrodenetleyicisi ile I2C protokolü kullanarak LTC2631 dijitalden analoğa çevirici (DAC) entegresini kontrol etmek için yazılmış bir sürücü kütüphanesidir.

---

## Dosyalar

- `LTC2631.h / .c` : DAC  fonksiyonları (dosaynın içinde fonksiyonların açıklarmaları mevcuttur)
- `main.c` : Örnek uygulama kodu


