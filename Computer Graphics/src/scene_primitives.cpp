#include <vector>
#include "scene_primitives.h"
#include "vec2.h"
#include "..\include\scene_vertex.h"

scene_vertex::~scene_vertex()
{

}

void scene_primitives::init() {

	std::vector<cgmath::vec2> v;

	v.push_back(cgmath::vec2(0, 0));
	v.push_back(cgmath::vec2(-1, 0));
	
	v.push_back(cgmath::vec2(0, -0.4));
	v.push_back(cgmath::vec2(1, 0));

	

	// make identifier for VAO
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &positionsVBO);
	glBindBuffer(GL_ARRAY_BUFFER, positionsVBO);

	// create buffer memory, specify data and send to gpu
	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof cgmath::vec2 * v.size(),
		v.data(),
		GL_DYNAMIC_DRAW
	);

	// enable attr with 'id' 0
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(
		0,			// config attr with 'id' 0
		2,			// two things in vec2[i]
		GL_FLOAT,	// of type float ( or GLfloat )
		GL_FALSE,	// normalize data?
		0,			// step? (called stride)
		nullptr		// the buffer data that we already sent using glBufferData
	);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	primitiveType = GL_POINTS;
}

void scene_primitives::awake() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glPointSize(20.0f);
}

void scene_primitives::sleep() {
	glPointSize(1.0f);
}

void scene_primitives::mainLoop() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(vao); // bind vao
	glDrawArrays(
		primitiveType,	// primitive
		0,			// starting at
		4			// how many of them
	);
	glBindVertexArray(0); // unbind vao
}

void scene_primitives::normalKeysDown(unsigned char key) {
	if (key == '1') {
		primitiveType = GL_POINTS;
	}

	if (key == '2') {
		primitiveType = GL_LINES;
	}
}

void scene_primitives::normalKeysUp(unsigned char key) {

}