#version 330 core
#pragma shader_stage(vertex)
layout (location = 0) in vec4 vertex;
out vec2 tex_coords;

uniform transformations {
	mat4 projection;
};

void main() {
	gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
	// gl_Position = vec4(vertex.xy, 0.0, 1.0);
	tex_coords = vertex.zw;
}
