#version 450

out vec4 FragColor;
in vec4 InterpolatedColor;

void main(){
	FragColor = InterpolatedColor;
}