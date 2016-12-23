#version 330

uniform int uGridSide;

uniform sampler2D uPositions;
uniform sampler2D uVelocities;

out vec4 FragColor;

#define EPSILON 0.00001
#define MAX_SPEED 3.0
#define MAX_FORCE 0.06

vec2 limit(vec2 v, float lim) {
  float len = length(v);
  return min(len, lim) * normalize(v);
}

vec2 flockAccel(in vec2 selfPos, in vec2 selfVel) {
  float separationNeighborDist = 10.0;
  vec2 sepSteer = vec2(0);
  int separationNeighbors = 0;

  float alignNeighborDist = 30.0;
  vec2 alignSteer = vec2(0);
  int alignNeighbors = 0;

  float cohesionNeighborDist = 50.0;
  vec2 cohesionPosition = vec2(0);
  int cohesionNeighbors = 0;

  for (int x = 0; x < uGridSide; x += 1) {
    for (int y = 0; y < uGridSide; y += 1) {
      vec2 texUV = vec2(x, y) / uGridSide;
      vec2 otherPos = texture(uPositions, texUV).xy;
      float dist = length(otherPos - selfPos);

      float isSelf = dist < EPSILON ? 0.0 : 1.0;

      if (dist < isSelf * separationNeighborDist) {
        vec2 fromOther = normalize(selfPos - otherPos) / dist;
        sepSteer += fromOther;
        separationNeighbors += 1;
      }

      if (dist < isSelf * alignNeighborDist) {
        vec2 otherVel = texture(uVelocities, texUV).xy;
        alignSteer += otherVel;
        alignNeighbors += 1;
      }

      if (dist < isSelf * cohesionNeighborDist) {
        cohesionPosition += otherPos;
        cohesionNeighbors += 1;
      }
    }
  }

  if (separationNeighbors > 0) {
    sepSteer /= float(separationNeighbors);
    if (length(sepSteer) > 0) {
      sepSteer = (normalize(sepSteer) * MAX_SPEED) - selfVel;
      sepSteer = limit(sepSteer, MAX_FORCE);
    }
  }

  if (alignNeighbors > 0) {
    alignSteer /= float(alignNeighbors);
    alignSteer = (normalize(alignSteer) * MAX_SPEED) - selfVel;
    alignSteer = limit(alignSteer, MAX_FORCE);
  }

  vec2 cohesionSteer = vec2(0);
  if (cohesionNeighbors > 0) {
    cohesionPosition /= float(cohesionNeighbors);
    cohesionSteer = (normalize(cohesionPosition - selfPos) * MAX_SPEED) - selfVel;
    cohesionSteer = limit(cohesionSteer, MAX_FORCE);
  }

  sepSteer *= 3.5;
  alignSteer *= 1.0;
  cohesionSteer *= 1.0;

  return sepSteer + alignSteer + cohesionSteer;
}

void main() {
  vec2 texIndex = gl_FragCoord.xy / vec2(uGridSide, uGridSide);
  vec2 pos = texture(uPositions, texIndex).xy;
  vec2 vel = texture(uVelocities, texIndex).xy;

  vec2 acc = flockAccel(pos, vel);
  vel = limit(vel + acc, MAX_SPEED);

  FragColor = vec4(vel, 0, 1);
}
