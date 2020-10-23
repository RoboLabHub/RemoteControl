const unsigned long kDoubleClickPeriod = 1000; // in msec

class Button
{
public:
  Button(int pin) : m_pin(pin)
  {
    Reset();
  }

  void Init()
  {
    pinMode(m_pin, INPUT);
    digitalWrite(m_pin, HIGH); // Set internal pullup resistor
  }

  void Reset()
  {
    m_clickedTime = 0;
    m_on = m_clicked = m_doubleClicked = m_changed = m_prevState = false;
  }
  
  void Cycle()
  {
    m_on = (digitalRead(m_pin) == 0) ? true : false;

    if (m_on != m_prevState) {
      if (true == m_on) {
        m_clicked = m_changed = true;
        if (millis() - m_clickedTime < kDoubleClickPeriod) m_doubleClicked = true;
        
        m_clickedTime = millis();
      }

      m_prevState = m_on;
    }    
  }

  bool On() { return m_on; }

  bool Clicked()
  {
    bool ret = m_clicked;
    m_clicked = m_doubleClicked = false;
    return ret;
  }

  bool DoubleClicked()
  {
    bool ret = m_doubleClicked;
    m_clicked = m_doubleClicked = false;
    return ret;
  }

  bool Changed()
  {
    if (m_changed) {
      // Return changed status only after 2 sec after last click
      if (millis() - m_clickedTime > 2000) {
        m_changed = false;
        return true;
      }
    }
    return false;
  }

private:
  int m_pin;
  bool m_on;
  bool m_prevState;
  bool m_clicked;
  bool m_changed;
  bool m_doubleClicked;

  unsigned long m_clickedTime;
};

////////////////////////////

Button btnLeft (2);
Button btnRight(3);

Button btnL1(4);
Button btnL2(5);
Button btnL3(6);

Button btnR1(9);
Button btnR2(8);
Button btnR3(7);

const int kBtnCount = 8;
Button* arrButtons[kBtnCount] = 
{
  &btnLeft, 
  &btnRight,
  &btnL1, &btnL2, &btnL3,
  &btnR1, &btnR2, &btnR3,
};

///////////////////////////

void init_buttons()
{
  for (int i = 0; i < kBtnCount; ++i) {
    arrButtons[i]->Init();
  }
}

void cycle_buttons()
{
  for (int i = 0; i < kBtnCount; ++i) {
    arrButtons[i]->Cycle();
  }
}
