#version 330

in vec4 ciPosition;
in vec2 birdVelocity;
in vec4 ciColor;

out VertexData {
  vec4 color;
  vec2 velocity;
} vs_out;

void main() {
  gl_Position = ciPosition;
  vs_out.color = ciColor;
  vs_out.velocity = birdVelocity;
}
