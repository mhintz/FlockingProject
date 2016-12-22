#version 330

uniform int uGridSide;
uniform int uScreenWidth;
uniform int uScreenHeight;

uniform sampler2D uPositions;
uniform sampler2D uVelocities;

out vec4 FragColor;

void main() {
  vec2 texIndex = gl_FragCoord.xy / vec2(uGridSide, uGridSide);
  vec2 pos = texture(uPositions, texIndex).xy;
  vec2 vel = texture(uVelocities, texIndex).xy;

  FragColor = vec4(vel, 1, 1);
}
