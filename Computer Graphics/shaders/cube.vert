#version 450

uniform mat4 mvp;

in vec3 VertexPosition;
in vec3 VertexColor;

out vec4 InterpolatedColor;

void main() {
	gl_Position = mvp * vec4(VertexPosition, 1.0f);
	gl_PointSize = 20.0;

	InterpolatedColor = vec4(VertexColor, 1.0f);
}
