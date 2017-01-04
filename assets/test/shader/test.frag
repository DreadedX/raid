#version 330 core
#pragma shader_stage(fragment)
#include "test.inc"

out vec3 color;
void main() {
	color = vec3(0.1,0.1,0.5);
}
