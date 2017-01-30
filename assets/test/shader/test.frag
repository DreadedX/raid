#version 330 core
#pragma shader_stage(fragment)
in vec2 tex_coords;
out vec4 color;

uniform sampler2D image;

void main() {
	color = texture(image, tex_coords);
	// color = vec4(1.0, 0.0, 0.0, 1.0);
}
