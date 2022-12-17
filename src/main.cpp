#include "user_config.h"
#include <Arduino.h>
#include "i2c_soft_lib.h"

i2c_soft_bus i2c_soft1;

bool read_sda(void)
{
  return digitalRead(21);
}
bool read_scl(void)
{
  return digitalRead(22);
}



void setup() {
  // put your setup code here, to run once:
  Serial.begin(MONITOR_SPEED);
  i2c_soft_init_struct init_struct = {
    .read_sda_ptr = &read_sda,
    .read_scl_ptr = &read_scl
  };

  i2c_soft_init(&i2c_soft1, &init_struct);
}

void loop() {
  // put your main code here, to run repeatedly:
}