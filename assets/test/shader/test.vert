#version 330 core
#pragma shader_stage(vertex)
layout (location = 0) in vec4 vertex;

out vec2 tex_coords;

uniform transformations {
	mat4 projection;
	mat4 model;
};

void main() {
	tex_coords = vertex.zw;
	gl_Position = projection * model * vec4(vertex.xy, 0.0, 1.0);
}
