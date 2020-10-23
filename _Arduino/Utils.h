byte GetCheckSum(byte* buff, int len)
{
  byte checkSum = 0;
  for (int i = 0; i < len; ++i) checkSum += buff[i];
  return checkSum;
}

void Beep(int hz = 1000, int period = 100)
{
  tone(pinSpeaker, hz, period);
  delay(period);

  // Disable annoying noise from speaker
  digitalWrite(pinSpeaker, HIGH);  
}

bool WaitForSerial(int ms)
{   
  unsigned long startTime = millis();
  while (millis() - startTime < ms) {
    if (Serial.available()) return true;
    delay(1);
  }
  return false;
}
