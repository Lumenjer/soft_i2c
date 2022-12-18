#ifndef USER_CONFIG_H
#define USER_CONFIG_H

#define DEVICE_MAX 10

#define I2C_SDA_PIN 21
#define I2C_SCL_PIN 22

#define BME280_ADDRESS                (0x76)
#define BME280_ID_REG                 (0xD0)
#define BME280_CHIP_ID                (0x60)

#define DEBUG

#ifdef LOG
  #undef LOG
#endif
#ifdef DEBUG
	#define LOG(func, ...) Serial.func(__VA_ARGS__)
#else
	#define LOG(func, ...) ;
#endif

#ifdef ASSERT
  #undef ASSERT
#endif
#define ASSERT(x) \
  if (!(x)){ \
    LOG(printf_P, PSTR("ASSERT: %s:%d\n"), __FILE__, __LINE__); \
    while(1){}; \
  }

#endif // USER_CONFIG_H
