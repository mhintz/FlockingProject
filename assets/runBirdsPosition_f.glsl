#version 330

uniform int uGridSide;
uniform int uScreenWidth;
uniform int uScreenHeight;

uniform sampler2D uPositions;
uniform sampler2D uVelocities;

uniform float uBirdSize;

out vec4 FragColor;

void main() {
  vec2 texIndex = gl_FragCoord.xy / vec2(uGridSide, uGridSide);
  vec2 pos = texture(uPositions, texIndex).xy;
  vec2 vel = texture(uVelocities, texIndex).xy;

  vec2 newPos = pos + vel;

  // Apply bounds
  if (newPos.x < -uBirdSize) { newPos.x = uScreenWidth + uBirdSize; }
  if (newPos.y < -uBirdSize) { newPos.y = uScreenHeight + uBirdSize; }
  if (newPos.x > uScreenWidth + uBirdSize) { newPos.x = -uBirdSize; }
  if (newPos.y > uScreenHeight + uBirdSize) { newPos.y = -uBirdSize; }

  FragColor = vec4(newPos, 0, 1);
}
