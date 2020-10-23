
void BeepCommand(int beepType)
{
  switch (beepType)
  {
  case 1: 
    tone(pinSpeaker, 1000, 30);
    break;
  case 2: 
    tone(pinSpeaker, 400, 700);
    break;
  case 3: // Error
    tone(pinSpeaker, 3200, 30);
    break;
  }

  // Disable annoying noise from speaker
  digitalWrite(pinSpeaker, HIGH);
}

void printLcdStr(int x, int y, char* str)
{
  lcd.setCursor(x, y);
  lcd.print(str);  
}

void processCommand(int cmdId, byte* data, int len)
{
  switch (cmdId) {
  case 0:
    BeepCommand(data[0]);
    break;
  case 1:
    lcd.clear();
    break;
  case 2:
    printLcdStr(data[0], data[1], &data[2]);
    break;
  }
  
  //char str[32];
  //sprintf(str, "%dC %d.%dV", (int)(data[0]), (int)(data[1]/10), (int)(data[1]%10));
  //lcd.setCursor(0, 0); lcd.print(str);
}

void cycleCommands()
{ 
  bool found = false;
  while (Serial.available()) {
    if (Serial.read() == '*') {
      found = true;
      break;
    }
  }

  static unsigned long s_onlineTime     = 0;
  static unsigned long s_offlineMsgTime = 0;
  if (!found) {
    if (millis() - s_onlineTime > 1000) {
      if (millis() - s_offlineMsgTime > 5000) {
        lcd.clear();
        printLcdStr(5, 1, "Offline...");
        tone(pinSpeaker, 3700, 10);
        s_offlineMsgTime = millis();
      }
    }      
    return;
  }
  s_onlineTime = millis();

  byte len;
  if (Serial.readBytes(&len, 1) != 1) return;
  if (len > 32) return;

  byte checkSum;
  if (Serial.readBytes(&checkSum, 1) != 1) return;

  byte cmdId;
  if (Serial.readBytes(&cmdId, 1) != 1) return;

  byte data[32] = {0};
  if (Serial.readBytes(data, len) != len) return;

  if (GetCheckSum(data, len) != checkSum) return;

  static bool firstCommand = true;
  if (firstCommand) {
    lcd.clear();
    firstCommand = false;
  }

  processCommand(cmdId, data, len);
}
