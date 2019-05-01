#version 410

uniform sampler2D metal;
uniform sampler2D lucina;

out vec4 FragColor;
in vec2 coord;

void main(){
	FragColor = mix(texture(lucina, coord), texture(metal, coord), 0.5f) ;
}