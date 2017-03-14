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

void setup() {
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);

  color.red = 0;
  color.green = 0;
  color.blue = 0;
}

void loop() {
  color.red += dir;
  if (color.red == 255)  {
    dir = -1;
  } else if (color.red == 0) {
    dir = 1;
  }
  paint(color);
  delay(10);
}
