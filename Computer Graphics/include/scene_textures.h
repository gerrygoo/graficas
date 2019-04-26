# pragma once

#include "scene.h"
#include "vec3.h"
#include "vec2.h"


class scene_textures : public scene {
public:
	~scene_textures();
	void init();
	void setup_shaders();
	void awake();
	void sleep();
	void reset() { }
	void mainLoop();

	void setup_textures();

	void resize(int width, int height);
	void normalKeysDown(unsigned char key) { }
	void normalKeysUp(unsigned char key) { }
	void specialKeys(int key) { };
	void passiveMotion(int x, int y) { };

private:
	GLuint shader_program;

	GLuint p_vao, p_vbo, c_vbo, i_vbo, lucina_texture, metal_texture;

	float aspect;

	std::vector<cgmath::vec3> positions;
	std::vector<cgmath::vec2> texture_cordinates;
	std::vector<GLuint> idxes;
};