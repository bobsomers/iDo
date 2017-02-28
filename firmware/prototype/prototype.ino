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

  color.red = 255;
  color.green = 255;
  color.blue = 255;
}

void loop() {
  
  paint(color);
  delay(100);
}
