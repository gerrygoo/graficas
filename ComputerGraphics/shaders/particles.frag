#version 410

in vec3 interpolated_color;

layout ( location = 0 ) out vec4 FragColor;

void main(){
	FragColor = vec4(0.2f, 0.5f, 0.9f, 1.0f);
	// FragColor = vec4(interpolated_color, 1.0f);
}