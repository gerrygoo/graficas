# pragma once

#include "scene.h"
#include "vec2.h"
#include "vec3.h"

class scene_circle : public scene {
public:
	~scene_circle();
	void init();
	void setup_shaders();
	void awake();
	void sleep();
	void reset() { }
	void mainLoop();
	void resize(int width, int height) { }
	void normalKeysDown(unsigned char key) { }
	void normalKeysUp(unsigned char key) { }
	void specialKeys(int key) { };
	void passiveMotion(int x, int y) { };

private:
	GLuint shader_program;

	GLuint p_vao, p_vbo, c_vbo;

	std::vector<cgmath::vec2> positions;
	std::vector<cgmath::vec3> colors;
};