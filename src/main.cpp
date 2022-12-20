#include "user_config.h"
#include <Arduino.h>
#include "i2c_soft_lib.h"

i2c_soft_bus i2c_soft1;

i2c_soft_device bme_280 = {
  .type     = I2C_SOFT_BME,
  .addr     = BME280_ADDRESS,
  .bus_ptr  = &i2c_soft1,
  .speed    = I2C_SOFT_SPEED_USE_BUS
};

void set_sda(bool release_line)
{
  if (release_line)
  {
    pinMode(I2C_SDA_PIN, INPUT);
  }
  else {
    pinMode(I2C_SDA_PIN, OUTPUT);
    digitalWrite(I2C_SDA_PIN, LOW);
  }
}

void set_scl(bool release_line)
{
  if (release_line)
  {
    pinMode(I2C_SCL_PIN, INPUT);
  }
  else {
    pinMode(I2C_SCL_PIN, OUTPUT);
    digitalWrite(I2C_SCL_PIN, LOW);
  }
}

bool read_sda(void)
{
  return digitalRead(I2C_SDA_PIN);
}
bool read_scl(void)
{
  return digitalRead(I2C_SCL_PIN);
}



void setup() {
  // put your setup code here, to run once:
  Serial.begin(MONITOR_SPEED);
  i2c_soft_init_struct init_struct = {
    .read_sda_ptr = &read_sda,
    .read_scl_ptr = &read_scl,
    .set_sda_ptr = &set_sda,
    .set_scl_ptr = &set_scl,
    .delay_micros = &delayMicroseconds,
    .speed        = I2C_SOFT_SPEED_400K
  };

  i2c_soft_init(&i2c_soft1, &init_struct);
  uint8_t data = BME280_ID_REG;

  LOG(println, "Start\n");
}

void loop() {
  // put your main code here, to run repeatedly:
  uint8_t data = BME280_ID_REG;
  static uint8_t speed = I2C_SOFT_SPEED_10K;
  bme_280.speed = (i2c_soft_speed)speed++;
  speed %= I2C_SOFT_SPEED_USE_BUS;
  if (i2c_soft_read(&bme_280, &data, 1)){
    LOG(printf, "Success %#0x\n", data);
  }
  else {
    LOG(printf, "Fail %#0x\n", data);
  }

  delay(1000);
}