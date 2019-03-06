#include <vector>
#include "scene_conchoid.h"

#define scale 1.3f
#define k 0.3f / scale
#define b 0.1f / scale


cgmath::vec2 conchoid_u(float t) {
	return cgmath::vec2(t + (k*t) / sqrt(b * b + t * t), b + (b*k) / sqrt(b * b + t * t) );
}

cgmath::vec2 conchoid_d(float t) {
	return cgmath::vec2(t + (-k*t) / sqrt(b * b + t * t), b + (b*-k) / sqrt(b * b + t * t));
}


void scene_conchoid::init() {
	primitiveType = GL_LINE_STRIP;

	float start = -1.5f, end = 1.5f, step = 0.01f;

	for (float t = start; t < end; t += step) v.push_back(conchoid_d(t)); 

	firsts.push_back(0);
	firsts.push_back(v.size());

	counts.push_back(v.size());
	counts.push_back(v.size());

	for (float t = start; t < end; t += step) v.push_back(conchoid_u(t));

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

		glGenBuffers(1, &positionsVBO);
		glBindBuffer(GL_ARRAY_BUFFER, positionsVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof cgmath::vec2 * v.size(), v.data(), GL_DYNAMIC_DRAW);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

void scene_conchoid::awake() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glPointSize(5.0f);
}

void scene_conchoid::sleep() { glPointSize(1.0f); }

void scene_conchoid::mainLoop() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(vao);
	glMultiDrawArrays(primitiveType, firsts.data(), counts.data(), 2);
	glBindVertexArray(0);
}


void scene_conchoid::normalKeysDown(unsigned char key) {
	if (key == '1') primitiveType = GL_POINTS;
	if (key == '2') primitiveType = GL_LINES;
	if (key == '3') primitiveType = GL_LINE_STRIP;
	if (key == '4') primitiveType = GL_LINE_LOOP;
	if (key == '5') primitiveType = GL_TRIANGLES;
	if (key == '6') primitiveType = GL_TRIANGLE_STRIP;
	if (key == '7') primitiveType = GL_TRIANGLE_FAN;
}