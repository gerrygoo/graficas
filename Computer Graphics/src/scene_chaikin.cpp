#include "scene_chaikin.h"


void scene_chaikin::initVAO() {
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	{

		glGenBuffers(1, &positionsVBO);
		glBindBuffer(GL_ARRAY_BUFFER, positionsVBO); 
		{
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
		} 
		glBindBuffer(GL_ARRAY_BUFFER, 0);

	}
	glBindVertexArray(0);
}

void scene_chaikin::defineVAO() {
	glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, positionsVBO);
		{
			glBufferData(GL_ARRAY_BUFFER, sizeof cgmath::vec2 * v.size(), v.data(), GL_DYNAMIC_DRAW);
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		
	glBindVertexArray(0);
}

void scene_chaikin::chaikinize(bool in_a_loop = false) {
	float threefourths = 3.0f / 4.0f;
	float onefourth = 1.0f / 4.0f;
	std::vector<cgmath::vec2> chaikinized;

	// for each geometry
	for (int range = 0; range < firsts.size(); range++) {
		int count = counts[range];
		int first = firsts[range];

		int n_first = chaikinized.size() ? chaikinized.size() : 0;
		int n_count = 0;

		if (!in_a_loop) {
			chaikinized.push_back(v[first]);
			n_count++;
		}

		// for each vertex in the geometry minus last one
		for (int j = first; j < first + count - 1; j++) {
			cgmath::vec2 p1 = v[j];
			cgmath::vec2 p0 = v[j + 1];

			chaikinized.push_back(onefourth * p0 + threefourths * p1);
			chaikinized.push_back(threefourths * p0 + onefourth * p1);

			n_count += 2;
		}
		if (!in_a_loop) {
			chaikinized.push_back(v[first + count - 1]);
			n_count++;
		}

		counts[range] = n_count;
		firsts[range] = n_first;
	}

	v = chaikinized;
}

void scene_chaikin::initDots() {
	glGenVertexArrays(1, &dotsVAO);
	glBindVertexArray(dotsVAO);
	{
		glGenBuffers(1, &dotsVBO);
		glBindBuffer(GL_ARRAY_BUFFER, dotsVBO);
		{
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
			glBufferData(GL_ARRAY_BUFFER, sizeof cgmath::vec2 * orig.size(), orig.data(), GL_DYNAMIC_DRAW);
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	glBindVertexArray(0);
}

void scene_chaikin::densifyDots(int factor) {
	float slice = 1.0f / (float)factor;

	std::vector<cgmath::vec2> dense;
	for (int i = 0; i < orig.size() - 1; i++) {
		auto p0 = orig[i];
		auto p1 = orig[i + 1];

		dense.push_back(p0);

		for (int j = 1; j < factor; j++) {
			float slices_l = j * slice;
			float slices_r = 1.0f - slices_l;
			dense.push_back(slices_r * p0 + slices_l * p1);
		}
	}
	dense.push_back(orig[orig.size() - 1]);

	orig = dense;
}

void scene_chaikin::init() {
	primitiveType = GL_POINTS;
	dotted = false;

	int p_size;

	orig.push_back(cgmath::vec2(-0.7f, -0.5f));
	orig.push_back(cgmath::vec2(-0.7f, 0.5f));
	orig.push_back(cgmath::vec2(-0.3f, 0.5f));
	orig.push_back(cgmath::vec2(0.0f, -0.5f));
	orig.push_back(cgmath::vec2(0.3f, 0.5f));

	counts.push_back(orig.size());
	firsts.push_back(0);
	p_size = orig.size();

	orig.push_back(cgmath::vec2(0.5f, 0.5f));
	orig.push_back(cgmath::vec2(0.7f, 0.0f));
	orig.push_back(cgmath::vec2(0.9f, 0.5f));

	counts.push_back(orig.size() - p_size);
	firsts.push_back(p_size);
	p_size = orig.size();

	v = orig;
	
	this->densifyDots(10);
	this->initDots();
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

	glBindVertexArray(vao); 
	{
		glMultiDrawArrays(primitiveType, firsts.data(), counts.data(), counts.size());
	}
	glBindVertexArray(0);
	if (dotted) {
		glBindVertexArray(dotsVAO);
		{
			glDrawArrays(GL_LINES, 0, orig.size());
		}
		glBindVertexArray(0);
	}
}

void scene_chaikin::normalKeysDown(unsigned char key) {
	if (key == '1') primitiveType = GL_POINTS;
	if (key == '2') primitiveType = GL_LINES;
	if (key == '3') primitiveType = GL_LINE_STRIP;
	if (key == '4') primitiveType = GL_LINE_LOOP;
	if (key == '5') primitiveType = GL_TRIANGLES;
	if (key == '6') primitiveType = GL_TRIANGLE_STRIP;
	if (key == '7') primitiveType = GL_TRIANGLE_FAN;
	if (key == 's') {
		this->chaikinize();
		this->defineVAO();
	}
	if (key == 'd') {
		dotted = !dotted;
	}
}