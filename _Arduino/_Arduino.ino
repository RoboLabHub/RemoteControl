#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#include "Buttons.h"
#include "Joystick.h"
#include "Lcd.h"

bool g_offline = true;

byte GetCheckSum(byte* buff, int len)
{
  byte checkSum = 0;
  for (int i = 0; i < len; ++i) checkSum += buff[i];
  return checkSum;
}

void Beep(int hz = 1000, int period = 100)
{
  tone(11, hz, period);
  delay(period);
}

void CheckForTelemetry()
{
  bool found = false;
  while (Serial.available()) {
    if (Serial.read() == '*') {
      found = true;
      break;
    }
  }
  if (!found) return;

  const byte kDataSize = 3;

  if (!Serial.available()) return; // Error
  byte len = Serial.read();
  if (len != kDataSize) return;

  if (!Serial.available()) return; // Error
  byte checkSum = Serial.read();

  byte data[kDataSize];
  for (int i = 0; i < sizeof(data); ++i) {
    if (!Serial.available()) return; // Error
    data[i] = Serial.read();
  }

  if (GetCheckSum(data, sizeof(data)) != checkSum) return;

  if (g_offline) {
    lcd.clear();
    g_offline = false;
  }
  
  char str[32];
  sprintf(str, "%dC %d.%dV", (int)(data[0]), (int)(data[1]/10), (int)(data[1]%10));
  lcd.setCursor(0, 0); lcd.print(str);

  switch (data[2]) // Beep type
  {
  case 0: 
    Beep(1000, 30); 
    break;
  case 1: 
    tone(11, 400, 1000);
    break;
  }

  // Flush input buffer
  while (Serial.available()) Serial.read();
}

void SendData(byte* data, int len)
{
  byte header[] = { '*', 'A', 'A', 'E', 'E' };
  Serial.write(header, sizeof(header));
  Serial.write((byte)len);
  Serial.write((byte)GetCheckSum(data, len));
  Serial.write(data, len);
}

void BeepTrigger(bool isOn)
{
  if (isOn) {
    Beep(500, 100); Beep(1000, 100);
  }
  else {
    Beep(1000, 100); Beep(500, 100);
  }
}

void setup()
{
  Serial.begin(115200);

  init_lcd();
  init_buttons();

  Beep( 500, 150);
  Beep(1000, 200);
  Beep( 300, 250);

  start_lcd();
}

struct DATA_PACKET_1
{
  uint8_t type;

  int8_t x1;
  int8_t y1;
  int8_t x2;
  int8_t y2;
  
  uint8_t buttons;
};

void SendData1()
{
  static unsigned long s_nextSend = millis();
  if (millis() < s_nextSend) return;
  
  DATA_PACKET_1 data;

  data.type = 1;

  data.x1 = joy_x1;
  data.y1 = joy_y1;
  data.x2 = joy_x2;
  data.y2 = joy_y2;
  
  byte bt = 0;
  if (btnLeft .On()) bt |= 0b00000001;
  if (btnRight.On()) bt |= 0b00000010;
  if (btnL1   .On()) bt |= 0b00000100;
  if (btnL2   .On()) bt |= 0b00001000;
  if (btnL3   .On()) bt |= 0b00010000;
  if (btnR1   .On()) bt |= 0b00100000;
  if (btnR2   .On()) bt |= 0b01000000;
  if (btnR3   .On()) bt |= 0b10000000;
  data.buttons = bt;

  SendData((byte*)&data, sizeof(data));

  s_nextSend = millis() + 100; // 10Hz
}

bool g_trigger1 = false;
bool g_trigger2 = false;

void loop()
{
  cycle_buttons();
  cycle_joystick();

  CheckForTelemetry();

  SendData1();
}
