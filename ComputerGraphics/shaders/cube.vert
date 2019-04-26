#version 410

uniform mat4 mvp;

in vec3 VertexPosition;
in vec3 VertexColor;

out vec4 InterpolatedColor;

void main() {
	gl_Position = mvp * vec4(VertexPosition, 1.0f);
	InterpolatedColor = vec4(VertexColor, 1.0f);
}
