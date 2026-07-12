#include <Wire.h>

#define SIOD_GPIO_NUM 26
#define SIOC_GPIO_NUM 27

#define SCCB_ADDR 0x30

bool writeReg(uint8_t reg, uint8_t val) {
  Wire.beginTransmission(SCCB_ADDR);
  Wire.write(reg);
  Wire.write(val);
  return Wire.endTransmission() == 0;
}

bool readReg(uint8_t reg, uint8_t &val) {
  Wire.beginTransmission(SCCB_ADDR);
  Wire.write(reg);

  if (Wire.endTransmission(false) != 0)
    return false;

  if (Wire.requestFrom((uint8_t)SCCB_ADDR, (uint8_t)1) != 1)
    return false;

  val = Wire.read();
  return true;
}

void dumpRegister(uint8_t reg)
{
  uint8_t value;

  if(readReg(reg,value))
  {
    Serial.printf("0x%02X = 0x%02X\n",reg,value);
  }
  else
  {
    Serial.printf("0x%02X = READ FAIL\n",reg);
  }
}

void setup()
{
  Serial.begin(115200);
  delay(1000);

  Serial.println();
  Serial.println("===== CAMERA SCCB SCAN =====");

  Wire.begin(SIOD_GPIO_NUM,SIOC_GPIO_NUM);

  Serial.println("Scanning I2C...");

  for(uint8_t addr=1;addr<127;addr++)
  {
    Wire.beginTransmission(addr);

    if(Wire.endTransmission()==0)
    {
      Serial.printf("Found device: 0x%02X\n",addr);
    }
  }

  Serial.println();

  dumpRegister(0x00);
  dumpRegister(0x01);
  dumpRegister(0x0A);
  dumpRegister(0x0B);
  dumpRegister(0x1C);
  dumpRegister(0x1D);
  dumpRegister(0x1E);
  dumpRegister(0x1F);
  dumpRegister(0xA0);
  dumpRegister(0xA1);

  Serial.println();
  Serial.println("Bruteforce register dump:");

  for(int i=0;i<256;i++)
  {
    uint8_t value;

    if(readReg(i,value))
    {
      Serial.printf("%02X : %02X\n",i,value);
    }
  }

  Serial.println("DONE");
}

void loop()
{
}