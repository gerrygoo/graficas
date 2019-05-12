# pragma once

#include <math.h>
#include <algorithm>
#include <iostream>

#include "scene.h"
#include "mat4.h"


#define  PI 3.14159265358979f
#define  DEG_RAD (PI / 180.0f)

inline float clamp(float n, float a, float b) {
	return fmin(b, fmax(n, a));
}

struct Camera {
	cgmath::vec3
		position, right,
		direction, up;

	cgmath::mat4 rotation_m, position_m;

	float yaw, pitch, roll;
	cgmath::vec3 disp_speed, turn_speed;

	Camera():
		yaw(0.0f),
		pitch(),
		roll(),
		position(),
		right(),
		direction(),
		up(),
		rotation_m(1.0f),
		position_m(1.0f),
		turn_speed(0.0f),
		disp_speed(0.0f)
		{ }

	bool must_move() {
		return (
			disp_speed.x ||
			disp_speed.y ||
			disp_speed.z ||
			turn_speed.x ||
			turn_speed.y ||
			turn_speed.z
		);
	}

	void zero_out_speeds() {
		disp_speed.x = 0.0f;
		disp_speed.y = 0.0f;
		disp_speed.z = 0.0f;
		turn_speed.x = 0.0f;
		turn_speed.y = 0.0f;
		turn_speed.z = 0.0f;
	}

	void move(float delta_time) {
		pitch = clamp(pitch + delta_time * 10 * turn_speed.x, -89.0, 89.0);
		yaw += delta_time * 10 * turn_speed.y;
		// position += delta_time * disp_speed;
		position += delta_time * (
			disp_speed.x * right +
			disp_speed.y * cgmath::vec3(0.0f, 1.0f, 0.0f) +
			disp_speed.z * direction
		);
	}

	cgmath::mat4 look() {
		direction.z = cos(DEG_RAD * (pitch)) * cos(DEG_RAD * (yaw));
        direction.y = sin(DEG_RAD * (pitch));
        direction.x = cos(DEG_RAD * (pitch)) * sin(DEG_RAD * (yaw));
		direction.normalize();

		right = cgmath::vec3::normalize(cgmath::vec3::cross({0.0f, 1.0f, 0.0f}, direction));
		up =	cgmath::vec3::normalize(cgmath::vec3::cross(direction, right));

		rotation_m[0].x = right.x;
        rotation_m[1].x = right.y;
        rotation_m[2].x = right.z;

        rotation_m[0].y = up.x;
        rotation_m[1].y = up.y;
        rotation_m[2].y = up.z;

        rotation_m[0].z = direction.x;
        rotation_m[1].z = direction.y;
        rotation_m[2].z = direction.z;

		position_m[3].x = -position.x;
		position_m[3].y = -position.y;
		position_m[3].z = -position.z;

		// std::cout << std::endl << "direction: " << direction << std::endl;
		// std::cout << "right: " << right << std::endl << std::endl;
		// std::cout << "up: " << up << std::endl;
		// std::cout << "pitch: " << pitch << ", yaw: " << yaw << ", position: " << position << std::endl;
		// std::cout << "camera position_m: " << std::endl << position_m << std::endl;
		// std::cout << "camera rotation_m: " << std::endl << rotation_m << std::endl;

		return rotation_m * position_m;
	}
};

struct Emisor {
	cgmath::vec3 position;
	float width, height;
	Emisor(): position(0.0f), width(5.0f), height(5.0f) { }
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
	void setup_textures();
	void setup_buffers();
	void setup_matrices();
	void setup_camera();

	void set_mvp_uniform();
	void set_delta_time_uniform();

private:
	bool debug;

	float aspect;

	GLuint program;


	GLint now_uniform_location;
	GLint delta_time_uniform_location;
	GLint acceleration_uniform_location;
	GLint time_to_live_uniform_location;
	GLuint render_subroutine_idx;
	GLuint update_subroutine_idx;
	GLint emisor_position_uniform_location;
	GLint emisro_width_uniform_location;
	GLint emisor_height_uniform_location;

	GLint view_matrix_uniform_location;
	GLint projection_matrix_uniform_location;

	GLint drop_texture_uniform_location;
	GLint positions_texture_uniform_location;

	GLint camera_position_uniform_location;


	cgmath::mat4 model, view, projection, mvp;
	Camera camera;

	GLuint particle_vaos[2];
	GLuint particle_tfos[2];

	GLuint initial_velocity_buffer;
	GLuint position_buffers[2];
	GLuint velocity_buffers[2];
	GLuint start_time_buffers[2];
	GLuint shape_vertex_buffer;

	GLuint drop_texture;
	GLuint positions_texture;

	int active_vao_idx;

	float disp_speed, turn_speed;
	float far, near, fov;

	unsigned int particle_count;
	Emisor emisor;
	cgmath::vec3 acceleration;
};