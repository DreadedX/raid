#version 330 core
#pragma shader_stage(fragment)
in vec2 tex_coords;
out vec4 color;

uniform sampler2D text;
uniform properties {
	vec3 text_color;
};

void main() {
	vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, tex_coords).r);
	color = sampled;
}
