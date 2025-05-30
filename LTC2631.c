/*

# STM32 ile LTC2631 DAC Sürücü Kütüphanesi

Bu proje, STM32 mikrodenetleyicisi ile I2C protokolü kullanarak LTC2631 dijitalden analoğa çevirici (DAC) entegresini kontrol etmek için yazılmış bir sürücü kütüphanesidir.

## Donanım Gereksinimi

- STM32 mikrodenetleyici (ör. STM32F4 serisi)
- LTC2631 DAC (Z tipi veya M tipi)
- I2C bağlantısı

---

## Dosyalar

- `LTC2631.h / .c` : DAC sürücü fonksiyonları
- `main.c` : Örnek uygulama kodu

---

## Fonksiyonlar ve Açıklamaları

### `void init(LTC2631* dac, I2C_HandleTypeDef* hi2c, LTC2631_Address address, uint8_t resolution);`
> DAC modülünü başlatır. I2C bağlantısı, adres ve resolution i belirlenir. Varsayılan olarak dahili referans kullanılır ve çıkış sıfırlanır.

---

### `HAL_StatusTypeDef SetOutput(LTC2631* dac, uint16_t value);`
> Verilen dijital değeri DAC çıkışına gönderir. 12-bit çözünürlükte 0-4095 arası değer alır.

---

### `HAL_StatusTypeDef SetOutputVoltage(LTC2631* dac, float volt, float refVoltage);`
> Volt cinsinden çıkış değeri ayarlar. Referans voltajına göre hesap yapılarak uygun dijital değere çevrilir.

---

### `void SetInternalReference(LTC2631* dac);`
> DAC’ın iç referans voltaj kaynağını (genellikle 2.5V) kullanmasını sağlar.

---

### `void SetExternalReference(LTC2631* dac);`
> Harici voltaj kaynağını referans olarak kullanmasını sağlar (örneğin 5V kullanıcı tarafından verilir).

---

### `void Power_Off(LTC2631* dac);`
> DAC’ı güç tasarrufu moduna alır, çıkış kapatılır.

---

### `HAL_StatusTypeDef Control(LTC2631* dac, uint8_t cmd_byte);`
> Sadece komut byte’ı gönderir. DAC’ı kontrol etmek için kullanılır (örneğin referans seçimi veya kapatma işlemleri).

---

### `HAL_StatusTypeDef WriteCommand(LTC2631* dac, uint8_t command, uint8_t data_high, uint8_t data_low);`
> Komut ve veri byte’larını birlikte gönderir. Daha gelişmiş kontrol sağlar.

---

### `void Set_I2C_Clock(I2C_HandleTypeDef *hi2c, uint32_t clk_speed);`
> I2C iletişim hızını (clock speed) değiştirir. Örneğin 100 kHz'den 1 MHz'e ayarlamak için kullanılır.


*/



#include "LTC2631.h"


// Verilen değeri çözünürlüğe göre MSB ve LSB'ye dönüştürür



static void SetData(uint64_t data,uint8_t* MSB,uint8_t *LSB,uint8_t rsln)
	{

	  data = data << (16 - rsln);

	  *MSB =data >> 8;
	  *LSB  =data&0xFF;
	}





// DAC sürücüsünü başlatır


void init(LTC2631* dac, I2C_HandleTypeDef* hi2c, LTC2631_Address address, uint8_t resolution) {
	    dac->hi2c = hi2c;
	    dac->address = address;
	    dac->resolution = resolution;

	    // Varsayılan olarak dahili referansı seç
	    SetInternalReference(dac);

	    // Başlangıçta DAC çıkışını 0 yap
	    SetOutput(dac, 0);
	}





// DAC çıkış değerini ayarlar Digital deger alan fonksıyon

HAL_StatusTypeDef SetOutput(LTC2631* dac, uint16_t value)
{
	uint8_t msb, lsb;
	SetData(value, &msb, &lsb, dac->resolution);

	  // Yaz ve güncelle komutunu gönder
	return WriteCommand(dac, (LTC2631_CMD_WRITE_AND_UPDATE << 4) | 0x0F, msb, lsb);
}

// DAC çıkış değerini ayarlar Analog değer alan fonksiyon
HAL_StatusTypeDef SetOutputVoltage(LTC2631* dac, float volt, float refVoltage) {

	// input dogrulama
	if (volt > refVoltage) volt = refVoltage;
    if (volt < 0) volt = 0;

    uint16_t maxVal = (1 << dac->resolution) - 1;
    uint16_t value = (uint16_t)((volt / refVoltage) * maxVal);

    return SetOutput(dac, value);
}


// Dahili referansı seçer



void SetInternalReference(LTC2631* dac ){

 uint8_t code =(LTC2631_CMD_SELECT_INTERNAL_REF<<4)|0x0F;
 Control(dac,code);



	}


// Harici referansı seçer


void SetExternalReference(LTC2631* dac){

	uint8_t code = LTC2631_CMD_SELECT_EXTERNAL_REF <<4| 0x0f  ;
	Control(dac,code);



	}




// DAC'ı güç tasarrufu moduna alır (kapatır)
void Power_Off(LTC2631* dac) {
		uint8_t code = (LTC2631_CMD_POWER_DOWN << 4) | 0x0F;
		Control(dac, code);

	}




// Komut ve veri byte'larını I2C üzerinden gönderir
HAL_StatusTypeDef WriteCommand(LTC2631* dac, uint8_t command, uint8_t data_high, uint8_t data_low)
	{
		uint8_t buffer[3];
		buffer[0] = command;     // command byte: (COMMAND << 4) | CHANNEL
		buffer[1] = data_high;   // upper data byte
		buffer[2] = data_low;    // lower data byte

		return HAL_I2C_Master_Transmit(dac->hi2c, dac->address << 1, buffer, 3, HAL_MAX_DELAY);
	}





// Sadece komut byte'ı gönderir (kontrol işlemleri için)


HAL_StatusTypeDef Control(LTC2631* dac, uint8_t cmd_byte)
{
		return HAL_I2C_Master_Transmit(dac->hi2c, dac->address << 1, &cmd_byte, 1, HAL_MAX_DELAY);
}




// I2C saat hızını değiştirir
void Set_I2C_Clock(I2C_HandleTypeDef *hi2c, uint32_t clk_speed) {

	hi2c->Init.ClockSpeed = clk_speed;
	hi2c->Init.DutyCycle = I2C_DUTYCYCLE_2;
	HAL_I2C_DeInit(hi2c);
	HAL_I2C_Init(hi2c);
	}






