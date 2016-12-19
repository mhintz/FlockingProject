#version 330

layout(points) in;
// layout(line_strip, max_vertices = 2) out;
layout(triangle_strip, max_vertices = 3) out;

in VertexData {
  vec4 color;
  vec2 velocity;
} gs_in[];

uniform mat4 ciModelViewProjection;
uniform float uBirdSize;

out vec4 fColor;

void main() {
  vec4 vel = uBirdSize * vec4(normalize(gs_in[0].velocity), 0, 0);

  gl_Position = ciModelViewProjection * (gl_in[0].gl_Position + vel);
  fColor = gs_in[0].color;
  EmitVertex();

  vec4 backVec = -vel;
  vec4 backPerp = 0.5 * vec4(-backVec.y, backVec.x, 0, 0);

  gl_Position = ciModelViewProjection * (gl_in[0].gl_Position + backVec - backPerp);
  fColor = gs_in[0].color;
  EmitVertex();

  gl_Position = ciModelViewProjection * (gl_in[0].gl_Position + backVec + backPerp);
  fColor = gs_in[0].color;
  EmitVertex();

  EndPrimitive();
}
