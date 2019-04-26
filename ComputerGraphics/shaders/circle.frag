#version 410

in vec3 InterpolatedColor;

in vec4 gl_FragCoord;

out vec4 FragColor;
void main(){
	if ( length( gl_FragCoord.xy/vec2(800, 800) - vec2(0.5, 0.5) ) < 0.25 ) discard;
	FragColor = vec4(InterpolatedColor, 1.0);
}