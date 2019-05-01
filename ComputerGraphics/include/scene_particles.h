# pragma once

#include "scene.h"

class scene_particles : public scene {
public:
	~scene_particles();
	void init();
	void mainLoop();
	void awake() { }
	void sleep() { }
	void reset() { }
    void keysDown(int key) {}
    void keysUp(int key) {}
    void passiveMotion(float x, float y) {}
	void resize(int width, int height);


	void setup_program_with_shaders();
	void print_shader_compile_errors(GLuint);
	void print_program_link_errors();

	void setup_uniforms();
	void setup_buffers();

private:
	float aspect;

	GLuint program;

	unsigned int particle_count;

	GLuint now_uniform_location;
	GLuint delta_time_uniform_location;
	GLuint acceleration_uniform_location;
	GLuint time_to_live_uniform_location;
	GLuint mvp_uniform_location;
	GLuint render_subroutine_idx;
	GLuint update_subroutine_idx;


	GLuint particle_vaos[2];
	GLuint particle_tfos[2];

	GLuint initial_velocity_buffer;

	GLuint position_buffers[2];
	GLuint velocity_buffers[2];
	GLuint start_time_buffers[2];

};