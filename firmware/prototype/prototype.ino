const int LED_R = 9;
const int LED_G = 10;
const int LED_B = 11;

struct Color {
  byte red;
  byte green;
  byte blue;
};

void paint(Color c) {
  analogWrite(LED_R, c.red);
  analogWrite(LED_G, c.green);
  analogWrite(LED_B, c.blue);
}

Color color;
int dir = 1;
int which = 0;

void setup() {
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);

  color.red = 0;
  color.green = 0;
  color.blue = 0;
}

void loop() {
  switch (which) {
  case 0:
    color.red += dir;
    if (color.red == 255)  {
      dir = -1;
    } else if (color.red == 0) {
      dir = 1;
      which = 1;
    }
    break;
  case 1:
    color.green += dir;
    if (color.green == 255)  {
      dir = -1;
    } else if (color.green == 0) {
      dir = 1;
      which = 2;
    }
    break;

  case 2:
    color.blue += dir;
    if (color.blue == 255)  {
      dir = -1;
    } else if (color.blue == 0) {
      dir = 1;
      which = 0;
    }
    break;

  default:
    which = 0;
  }
  
  paint(color);
  delay(10);
}
