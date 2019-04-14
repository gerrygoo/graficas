#version 410

in vec2 VertexPosition;
in vec3 VertexColor;

out vec3 InterpolatedColor;

void main() {
	gl_Position = vec4(VertexPosition, 0.0f, 1.0f);
	gl_PointSize = 5.0;
	InterpolatedColor = VertexColor;
}
