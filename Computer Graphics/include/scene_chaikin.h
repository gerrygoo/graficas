#pragma once
#include <vector>
#include <list>

#include "scene.h"
#include "vec2.h"

class scene_chaikin : public scene {
public:
	void init();
	void initVAO();
	void defineVAO();
	void chaikinize(bool);

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
	GLuint positionsVBO;
	GLuint vao;
	GLenum primitiveType;

	std::list<cgmath::vec2> v;
	std::vector<int> firsts;
	std::vector<int> counts;
};