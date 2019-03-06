#include "scene_chaikin.h"

#define scale 1.3f
#define k 0.3f / scale
#define b 0.1f / scale

#define threefourths 3.0f / 4.0f
#define onefourth 1.0f / 4.0f


void scene_chaikin::initVAO() {
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

		glGenBuffers(1, &positionsVBO);

		glBindBuffer(GL_ARRAY_BUFFER, positionsVBO);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
}

void scene_chaikin::defineVAO() {
	std::vector<cgmath::vec2> vec(v.begin(), v.end());
	glBufferData(GL_ARRAY_BUFFER, sizeof cgmath::vec2 * v.size(), vec.data(), GL_DYNAMIC_DRAW);
}

void scene_chaikin::chaikinize(bool in_a_loop = false) {
	std::list<cgmath::vec2>::iterator i = v.begin();

	for (int range = 0; range < firsts.size(); range++) {
		auto first = i;
		int count = counts[range];

		for (int j = 0; j < count - 1; j++) {
			cgmath::vec2 p1 = *++i;
			cgmath::vec2 p0 = *--i;
			i = v.insert(i, threefourths * p0 + onefourth * p1);
			i = v.insert(i, onefourth * p0 + threefourths * p1);
			i++;
		}

		i = first;
		i++; i++; // leave first point there
		for (int j = 0; j < count - 2; j++) {
			v.erase(i);
			i++; i++;
		}
		i++; // leave iterator at the next 'geometry.begin()'
	}
}

void scene_chaikin::init() {
	primitiveType = GL_LINE_STRIP;

	float start = -1.5f, end = 1.5f, step = 0.1f;

	for (float t = start; t < end; t += step) v.push_back(cgmath::vec2(t + (k*t) / sqrt(b * b + t * t), b + (b*k) / sqrt(b * b + t * t)));
	firsts.push_back(0);
	firsts.push_back(v.size());

	counts.push_back(v.size());
	counts.push_back(v.size());
	for (float t = start; t < end; t += step) v.push_back(cgmath::vec2(t + (-k*t) / sqrt(b * b + t * t), b + (b*-k) / sqrt(b * b + t * t)));

	this->chaikinize();
	this->initVAO();
	this->defineVAO();
}

void scene_chaikin::awake() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glPointSize(5.0f);
}

void scene_chaikin::mainLoop() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMultiDrawArrays(primitiveType, firsts.data(), counts.data(), 2);
}

void scene_chaikin::normalKeysDown(unsigned char key) {
	if (key == '1') primitiveType = GL_POINTS;
	if (key == '2') primitiveType = GL_LINES;
	if (key == '3') primitiveType = GL_LINE_STRIP;
	if (key == '4') primitiveType = GL_LINE_LOOP;
	if (key == '5') primitiveType = GL_TRIANGLES;
	if (key == '6') primitiveType = GL_TRIANGLE_STRIP;
	if (key == '7') primitiveType = GL_TRIANGLE_FAN;
}