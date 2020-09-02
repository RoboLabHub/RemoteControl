class Button
{
public:
  Button(int pin) : m_pin(pin), m_on(false)
  {
  }

  void Init()
  {
    pinMode(m_pin, INPUT);
    digitalWrite(m_pin, HIGH); // Set internal pullup resistor
  }

  void Cycle()
  {
    m_on = (digitalRead(m_pin) == 0) ? true : false;
  }

  bool On() { return m_on; }

private:
  int m_pin;
  bool m_on;
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
