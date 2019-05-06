# pragma once

#include <math.h>
#include <iostream>

#include "scene.h"
#include "mat4.h"


#define  PI 3.14159265358979f
#define  DEG_RAD (PI / 180.0f)

struct Camera {
	cgmath::vec3
		position, target, right,
		direction, up;

	cgmath::mat4 rotation_m, position_m;

	float yaw, pitch, roll;
	bool dirty;

	Camera():
		yaw(),
		pitch(),
		roll(),
		position(),
		target(),
		right(),
		direction(),
		up{0.0f, 1.0f, 0.0f},
		rotation_m(1.0f),
		position_m(1.0f)
		{ }

	cgmath::mat4 look() {

		direction.x = cos(DEG_RAD * (pitch)) * cos(DEG_RAD * (yaw));
        direction.y = sin(DEG_RAD * (pitch));
        direction.z = cos(DEG_RAD * (pitch)) * sin(DEG_RAD * (yaw));
		direction.normalize();

		right = cgmath::vec3::cross(up, direction);
		up = 	cgmath::vec3::cross(direction, right);

		rotation_m[0].x = right.x;
        rotation_m[1].x = right.y;
        rotation_m[2].x = right.z;

        rotation_m[0].y = up.x;
        rotation_m[1].y = up.y;
        rotation_m[2].y = up.z;

        rotation_m[0].z = direction.x;
        rotation_m[1].z = direction.y;
        rotation_m[2].z = direction.z;

		position_m[3].z = -position.x;
		position_m[3].y = -position.y;
		position_m[3].x = -position.z;

		std::cout << "camera position_m: " << std::endl << position_m << std::endl;
		std::cout << "camera rotation_m: " << std::endl << rotation_m << std::endl;

		return rotation_m * position_m;
	}
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
    void keysUp(int key);
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