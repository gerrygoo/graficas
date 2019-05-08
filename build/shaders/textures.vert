#version 450

uniform mat4 mvp;

in vec3 VertexPosition;
in vec2 TextureCoordinates;

out vec2 coord;

void main() {
	gl_Position = mvp * vec4(VertexPosition, 1.0f);
	coord = TextureCoordinates;
}
