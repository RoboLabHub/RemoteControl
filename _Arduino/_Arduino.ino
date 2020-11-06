#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

const int pinSpeaker = 11;
const int kBTCount = 3;

#include "Buttons.h"
#include "Joystick.h"
#include "Utils.h"
#include "Lcd.h"
#include "Commands.h"

struct {
  char* name;
  char* addr;
}
g_BT[kBTCount] = {
  { "Develop", "98d3:31:fc10fa" },
  { "Dog V2",  "0021:12:084A30" }, // Put your robodog BT address here
};

bool g_changeBT = false;
int  g_currBT = 0;

void SendData(byte* data, int len)
{
  Serial.write((byte)'*');
  Serial.write((byte)len);
  Serial.write((byte)GetCheckSum(data, len));
  Serial.write(data, len);
}

void setup()
{
  init_lcd();
  init_buttons();

  Beep( 300, 100);
  Beep( 600, 100);
  Beep(1000, 150);

  start_lcd();

  cycle_buttons();
  if (btnR3.On()) {
    digitalWrite(12, HIGH); // Set AT mode for bluetooth adapter

    lcd.clear();
    lcd.setCursor( 0, 2); lcd.print("Change BT receiver");

    g_changeBT = true;
    Serial.begin(38400);

    // Wait while button is pressed
    while (btnR3.On()) {
      cycle_buttons();
      delay(100);
    }
    btnR3.Reset();

    lcd.setCursor(10, 0); lcd.print("> Select <"); 
  }
  else {
    Serial.begin(115200);
    Serial.setTimeout(20);
  }
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

void cycleSendData()
{
  static unsigned long s_lastSend = millis();
  if (millis() - s_lastSend < 125) return; // 10Hz

  cycle_buttons();
  cycle_joystick();
  
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
  s_lastSend = millis();

  char str[32];
  sprintf(str, "%4d%4d %4d%4d", (int)data.x1, (int)data.y1, (int)data.x2, (int)data.y2);
  lcd.setCursor(1, 3); lcd.print(str);  

  lcd.setCursor(0, 0); lcd.print(btnL3.On()   ? "X" : "-");
  lcd.setCursor(0, 1); lcd.print(btnL2.On()   ? "X" : "-");
  lcd.setCursor(0, 2); lcd.print(btnL1.On()   ? "X" : "-");
  lcd.setCursor(0, 3); lcd.print(btnLeft.On() ? "X" : "-");

  lcd.setCursor(19, 0); lcd.print(btnR3.On()    ? "X" : "-");
  lcd.setCursor(19, 1); lcd.print(btnR2.On()    ? "X" : "-");
  lcd.setCursor(19, 2); lcd.print(btnR1.On()    ? "X" : "-");
  lcd.setCursor(19, 3); lcd.print(btnRight.On() ? "X" : "-");
}

void loopChangeBT()
{
  cycle_buttons();
  
  if (btnR3.Clicked()) {
    if (++g_currBT >= kBTCount) g_currBT = 0;

    lcd.clear();
    lcd.setCursor(12, 0); lcd.print(g_BT[g_currBT].name);
  }

  if (btnR3.Changed()) {
    lcd.clear();
    lcd.setCursor(0, 0); lcd.print("Re-binding...");
        
    Serial.println("AT+RMAAD");
    lcd.setCursor(0, 1); lcd.print(Serial.readStringUntil('\n'));
    
    Serial.print("AT+BIND=");
    Serial.println(g_BT[g_currBT].addr);
    lcd.setCursor(0, 2); lcd.print(Serial.readStringUntil('\n'));

    Serial.println("AT+RESET");
    lcd.setCursor(0, 3); lcd.print(Serial.readStringUntil('\n'));

    delay(1000);
    Serial.begin(115200);
    Serial.setTimeout(20);
    lcd.clear();
    g_changeBT = false;
  }  
}

void loop()
{
  if (g_changeBT) {
    loopChangeBT();
  }
  else {
    cycleCommands();
    cycleSendData();
  }

  delay(10);
}
