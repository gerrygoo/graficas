#version 410

out vec4 FragColor;
in vec4 InterpolatedColor;

void main(){
	FragColor = InterpolatedColor;
}