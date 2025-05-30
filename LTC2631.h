/*
 * LTC2631.h

 */

#ifndef INC_LTC2631_H_
#define INC_LTC2631_H_

#include "stm32f4xx_hal.h"
// LTC2631 cihaz adresleri (Z tipi ve M tipi)

typedef enum {
    // Z tipi
    LTC2631_Z_ADDR_GND_GND    = 0x10,
    LTC2631_Z_ADDR_GND_FLOAT  = 0x11,
    LTC2631_Z_ADDR_GND_VCC    = 0x12,
    LTC2631_Z_ADDR_FLOAT_GND  = 0x13,
    LTC2631_Z_ADDR_FLOAT_FLOAT= 0x14,
    LTC2631_Z_ADDR_FLOAT_VCC  = 0x15,
    LTC2631_Z_ADDR_VCC_GND    = 0x16,
    LTC2631_Z_ADDR_VCC_FLOAT  = 0x17,
    LTC2631_Z_ADDR_VCC_VCC    = 0x18,
    LTC2631_Z_ADDR_GLOBAL     = 0x37,

    // M tipi
    LTC2631_M_ADDR_GND        = 0x10,
    LTC2631_M_ADDR_FLOAT      = 0x11,
    LTC2631_M_ADDR_VCC        = 0x12,
    LTC2631_M_ADDR_GLOBAL     = 0x73
} LTC2631_Address;




// LTC2631 komut kodlalari
typedef enum {
    LTC2631_CMD_WRITE_INPUT          = 0x0,
    LTC2631_CMD_UPDATE_DAC           = 0x1,
    LTC2631_CMD_WRITE_AND_UPDATE     = 0x3,
    LTC2631_CMD_POWER_DOWN           = 0x4,
    LTC2631_CMD_SELECT_INTERNAL_REF  = 0x6,
    LTC2631_CMD_SELECT_EXTERNAL_REF  = 0x7
} LTC2631_Command;


// DAC icin  yapı
typedef struct {
    I2C_HandleTypeDef* hi2c;
    LTC2631_Address address;
    uint8_t resolution;
} LTC2631;

// // Public API fonksiyonlarin  prototipleri



void init(LTC2631* dac, I2C_HandleTypeDef* hi2c, LTC2631_Address address, uint8_t resolution);
// DAC çıkışını ayarlama
HAL_StatusTypeDef SetOutput(LTC2631* dac, uint16_t value);

// DAC çıkışını ayarlama analog
HAL_StatusTypeDef SetOutputVoltage(LTC2631* dac, float volt, float refVoltage);
// Kontrol komutlarını gönderme
HAL_StatusTypeDef Control(LTC2631* dac, uint8_t cmd_byte);
//Komut ve veri gönder
HAL_StatusTypeDef WriteCommand(LTC2631* dac, uint8_t command, uint8_t data_high, uint8_t data_low);

// dahili referens ayarlama
void SetInternalReference(LTC2631* dac);
// harici referens ayarlama
void SetExternalReference(LTC2631* dac);
// guc kesme
void Power_Off(LTC2631* dac);

//I2C CLOCK ayarlama
void Set_I2C_Clock(I2C_HandleTypeDef *hi2c, uint32_t clk_speed);
















#endif /* INC_LTC2631_H_ */
