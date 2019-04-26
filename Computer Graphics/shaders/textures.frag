#version 450

uniform sampler2D metal;
uniform sampler2D lucina;

out vec4 FragColor;
in vec2 coord;

void main(){
	FragColor = mix(texture2D(lucina, coord), texture2D(metal, coord), 0.5f) ;
}