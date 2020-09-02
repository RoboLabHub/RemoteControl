int joy_x1, joy_y1;
int joy_x2, joy_y2;

bool map_if_in_range(int& val, int from_min, int from_max, int to_min, int to_max)
{
  if (val >= from_min && val <= from_max)
  {
    val = map(val, from_min, from_max, to_min, to_max);
    return true;
  }

  return false;
}

void cycle_joystick()
{
  int x1 = analogRead(A2);
  int y1 = analogRead(A3);

  int x2 = analogRead(A0);
  int y2 = analogRead(A1);

  // Center the postitions of sticks
  int x1_0 = 162;
  int y1_0 = 167;
  int x2_0 = 170;
  int y2_0 = 165;

  int dt = 15; // Dead zone range (-15 .. +15) where the input will be ignored

  if (!map_if_in_range(x1,        0, x1_0-dt,   100,    0) &&
      !map_if_in_range(x1,  x1_0+dt, 340,  0,  -100))
  {
    x1 = 0; // In dead zone
  } 

  if (!map_if_in_range(y1,        0, y1_0-dt,  -100,   0) &&
      !map_if_in_range(y1,  y1_0+dt,     340,     0, 100))
  {
    y1 = 0; // In dead zone
  }

  if (!map_if_in_range(x2 ,       0, x2_0-dt,  -100,   0) &&
      !map_if_in_range(x2,  x2_0+dt,     340,     0, 100))
  {
    x2 = 0; // In dead zone
  } 

  if (!map_if_in_range(y2,        0, y2_0-dt,  100,    0) &&
      !map_if_in_range(y2,  y2_0+dt,     340,    0, -100))
  {
    y2 = 0; // In dead zone
  }

  joy_x1 = x1;
  joy_x2 = x2;
  joy_y1 = y1;
  joy_y2 = y2;
}
