# pragma once

#include "scene.h"
#include "mat4.h"

struct Camera {
	cgmath::vec3 position, up, target;

	float yaw, pitch, roll;
	bool dirty;
};

class scene_particles : public scene {
public:
	~scene_particles();
	void init();
	void mainLoop();
	void awake();
	void sleep();
	void reset() { }
    void keysDown(int key);
    void keysUp(int key) {}
    void passiveMotion(float x, float y) {}
	void resize(int width, int height);


	void setup_program_with_shaders();
	void print_shader_compile_errors(GLuint);
	void print_program_link_errors();

	void setup_uniforms();
	void setup_buffers();
	void setup_matrices();
	void setup_camera();

	void set_mvp_uniform();

	void set_delta_time_uniform();
	void set_view_matrix();

private:
	float aspect;

	GLuint program;

	unsigned int particle_count;

	GLuint now_uniform_location;
	GLuint delta_time_uniform_location;
	GLuint acceleration_uniform_location;
	GLuint time_to_live_uniform_location;
	GLuint render_subroutine_idx;
	GLuint update_subroutine_idx;

	GLuint mvp_uniform_location;
	cgmath::mat4 model, view, projection, mvp;
	Camera camera;

	GLuint particle_vaos[2];
	GLuint particle_tfos[2];

	GLuint initial_velocity_buffer;

	GLuint position_buffers[2];
	GLuint velocity_buffers[2];
	GLuint start_time_buffers[2];

	int active_vao_idx;
};