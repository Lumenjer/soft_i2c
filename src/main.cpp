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

const char* get_speed_str(i2c_soft_speed speed)
{
  ASSERT(speed < I2C_SOFT_SPEED_TOTAL)

  switch (speed)
  {
  case I2C_SOFT_SPEED_10K:
    return "I2C SPEED 10K";
    break;

  case I2C_SOFT_SPEED_100K:
    return "I2C SPEED 100K";
    break;

  case I2C_SOFT_SPEED_200K:
    return "I2C SPEED 200K";
    break;

  case I2C_SOFT_SPEED_400K:
    return "I2C SPEED 400K";
    break;

  case I2C_SOFT_SPEED_USE_BUS:
    return "I2C SPEED USE BUS";
    break;

  default:
    return "I2C SPEED DEFAULT";
    break;
  }

  return "";
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

  LOG(printf, "Set speed %s\n", get_speed_str(bme_280.speed));
  uint32_t start = micros();
  bool result = i2c_soft_write(&bme_280, &data, 1);

  uint32_t end = micros();
  LOG(printf, "Result write = %s %#02x\n", result ? "Success" : "Fail", data);
  LOG(printf, "Start - End diff %d\n", end - start);
  LOG(printf, "SDA = %d, SCL = %d\n", i2c_soft1.read_sda_ptr(), i2c_soft1.read_scl_ptr());

  if (result)
  {
    start = micros();
    result = i2c_soft_read(&bme_280, &data, 1);
    end = micros();

    LOG(printf, "Result read = %s\n", result ? "Success" : "Fail");
    LOG(printf, "Data     - %#02x\n"
                "Expected - %#02x\n%s",
                data,
                BME280_CHIP_ID,
                data != BME280_CHIP_ID ? " Wrong" : "");
    LOG(printf, "Start - End diff %d\n", end - start);
    LOG(printf, "SDA = %d, SCL = %d\n", i2c_soft1.read_sda_ptr(), i2c_soft1.read_scl_ptr());
  }

  data = BME280_ID_REG;
  start = micros();
  result = i2c_soft_write_read(&bme_280, &data, 1, 1);
  end = micros();

  LOG(printf, "Result write read = %s\n", result ? "Success" : "Fail");
  LOG(printf, "Data     - %#02x\n"
              "Expected - %#02x\n%s",
              data,
              BME280_CHIP_ID,
              data != BME280_CHIP_ID ? " Wrong" : "");
  LOG(printf, "Start - End diff %d\n", end - start);
  LOG(printf, "SDA = %d, SCL = %d\n", i2c_soft1.read_sda_ptr(), i2c_soft1.read_scl_ptr());

  delay(1000);
}