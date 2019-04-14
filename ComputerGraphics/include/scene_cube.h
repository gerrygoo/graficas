# pragma once

#include "scene.h"
#include "vec3.h"
#include "vec2.h"


class scene_cube : public scene {
public:
	~scene_cube();
	void setup_shaders();

	void init();
	void awake();
	void sleep();
	void reset() { }
	void mainLoop();

	void resize(int width, int height);

    void keysDown(int key) {}
    void keysUp(int key) {}
    void passiveMotion(float x, float y) {}

private:
	GLuint shader_program;

	GLuint p_vao, p_vbo, c_vbo, i_vbo;

	float aspect;

	std::vector<cgmath::vec3> positions;
	std::vector<cgmath::vec3> colors;
	std::vector<GLuint> idxes;
};