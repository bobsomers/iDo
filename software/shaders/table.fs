#version 330

uniform float intensity;
out vec4 color;

void main() {
  color = vec4(vec3(intensity), 1.0);
}
