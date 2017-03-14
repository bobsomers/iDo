#version 330

uniform vec3 clr;
out vec4 color;

void main() {
  color = vec4(clr, 1.0);
}
