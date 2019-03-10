#pragma once
#include <vector>
#include "scene.h"
#include "vec2.h"

class scene_chaikin : public scene {
public:
	void init();

	void initVAO();
	void defineVAO();

	void initDots();
	void densifyDots(int);

	void chaikinize();

	void add_segment(std::vector<cgmath::vec2>&, bool);

	void awake();
	void sleep() { }
	void reset() { }
	void mainLoop();
	void resize(int width, int height) { }
	void normalKeysDown(unsigned char key);
	void normalKeysUp(unsigned char key) { }
	void specialKeys(int key) { };
	void passiveMotion(int x, int y) { };

private:
	GLuint dotsVBO;
	GLuint dotsVAO;

	GLuint positionsVBO;
	GLuint vao;

	GLenum primitiveType;

	bool curved;
	bool dotted;

	std::vector<bool> inaloop;

	std::vector<cgmath::vec2> orig;
	std::vector<int> o_firsts;
	std::vector<int> o_counts;

	std::vector<cgmath::vec2> v;
	std::vector<int> firsts;
	std::vector<int> counts;
};