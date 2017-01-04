#version 330 core
#pragma shader_stage(vertex)
#include "test.inc"

layout(location = 0) in vec3 vertex_position;
void main() {
	gl_Position.w = 1.0;
	gl_Position.xyz = vertex_position;
}
