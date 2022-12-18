#include "user_config.h"
#include <Arduino.h>
#include "i2c_soft_lib.h"

i2c_soft_bus i2c_soft1;

i2c_soft_device bme_280 = {
  .type = I2C_SOFT_BME,
  .addr = BME280_ADDRESS,
  .bus_ptr = &i2c_soft1
};

void pull_sda(bool pull)
{
  if (pull)
  {
    pinMode(I2C_SDA_PIN, OUTPUT);
    digitalWrite(I2C_SDA_PIN, LOW);
  }
  else {
    pinMode(I2C_SDA_PIN, INPUT);
  }
}

void pull_scl(bool pull)
{
  if (pull)
  {
    pinMode(I2C_SCL_PIN, OUTPUT);
    digitalWrite(I2C_SCL_PIN, LOW);
  }
  else {
    pinMode(I2C_SCL_PIN, INPUT);
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
    .pull_sda_ptr = &pull_sda,
    .pull_scl_ptr = &pull_scl,
    .delay_micros = &delayMicroseconds
  };

  i2c_soft_init(&i2c_soft1, &init_struct);
  uint8_t data = BME280_ID_REG;

  LOG(println, "Start\n");
  if (i2c_soft_read(&bme_280, &data, 1)){
    LOG(println, "Success\n");
  }
  else{
    LOG(println, "Fail\n");
  }

}

void loop() {
  delay(1000);
  uint8_t data = BME280_ID_REG;
  if (i2c_soft_read(&bme_280, &data, 1)){
    LOG(println, "Success\n");
  }
  else {
    LOG(println, "Fail\n");
  }
  // put your main code here, to run repeatedly:

}