#include <vector>
#include <iostream>
#include <math.h>

#include "scene_cube.h"
#include "ifile.h"
#include "time_util.h"

#include "mat4.h"

#define  PI_F 3.14159265358979f
#define  DEG_RAD (PI_F / 180.0f)

scene_cube::~scene_cube() {
	glDeleteProgram(shader_program);
}

void scene_cube::awake() {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_PROGRAM_POINT_SIZE);
}

void scene_cube::sleep() {
	glClearColor(1.0f, 1.0f, 0.5f, 1.0f);
	glDisable(GL_PROGRAM_POINT_SIZE);
}

void scene_cube::setup_shaders() {
	ifile shader_file;

	shader_file.read("shaders/cube.vert");
	std::string vertex_source = shader_file.get_contents();
	const GLchar* vertex_source_c = (const GLchar*)vertex_source.c_str();

	GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_source_c, nullptr);
	glCompileShader(vertex_shader);

	GLint vertex_compiled;
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &vertex_compiled);
	if (vertex_compiled != GL_TRUE) {
		GLint log_length;
		glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &log_length);

		std::vector<char> log(log_length);

		glGetShaderInfoLog(vertex_shader, log_length, &log_length, log.data());

		for (auto c : log) std::cout << c;
	}


	shader_file.read("shaders/cube.frag");
	std::string fragment_source = shader_file.get_contents();
	const GLchar* fragment_source_c = (const GLchar*)fragment_source.c_str();

	GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragment_source_c, nullptr);
	glCompileShader(fragment_shader);

	GLint fragment_compiled;
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &fragment_compiled);
	if (fragment_compiled != GL_TRUE) {
		GLint log_length;
		glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &log_length);

		std::vector<char> log(log_length);

		glGetShaderInfoLog(fragment_shader, log_length, &log_length, log.data());

		for (auto c : log) std::cout << c;
	}


	shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);

	glBindAttribLocation(shader_program, 0, "VertexPosition");
	glBindAttribLocation(shader_program, 1, "VertexColor");

	glLinkProgram(shader_program);
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
}

void scene_cube::init() {
	this->setup_shaders();

	auto blue = cgmath::vec3(66.0f, 134.0f, 244.0f)/255.0f;
	auto orange = cgmath::vec3(244.0f, 152.0f, 65.0f) / 255.0f;

	auto white = cgmath::vec3(255.0f, 250.0f, 230.0f) / 255.0f;
	// auto white = cgmath::vec3(244, 66, 238) / 255.0f;
	auto green = cgmath::vec3(98.0f, 196.0f, 53.0f) / 255.0f;
	auto yellow = cgmath::vec3(255.0f, 243.0f, 89.0f) / 255.0f;
	auto red = cgmath::vec3(249.0f, 89.0f, 89.0f) / 255.0f;

	// +z axis outwards, towards 'me'
	float p3 = 3.0f;
	float m3 = -3.0f;

	auto utl = cgmath::vec3(m3, p3, m3);
	auto utr = cgmath::vec3(p3, p3, m3);
	auto ubr = cgmath::vec3(p3, p3, p3);
	auto ubl = cgmath::vec3(m3, p3, p3);

	auto dtl = cgmath::vec3(m3, m3, m3);
	auto dtr = cgmath::vec3(p3, m3, m3);
	auto dbr = cgmath::vec3(p3, m3, p3);
	auto dbl = cgmath::vec3(m3, m3, p3);

	int s;

	// F - 0
	s = positions.size();
	positions.push_back(ubl);
	positions.push_back(ubr);
	positions.push_back(dbr);
	positions.push_back(dbl);

	for (int i = 0; i < 4; i++) colors.push_back(red);

	idxes.push_back(s + 3);
	idxes.push_back(s + 1);
	idxes.push_back(s);

	idxes.push_back(s + 3);
	idxes.push_back(s + 2);
	idxes.push_back(s + 1);

	// L - 4
	s = positions.size();
	positions.push_back(utl);
	positions.push_back(ubl);
	positions.push_back(dbl);
	positions.push_back(dtl);

	for (int i = 0; i < 4; i++) colors.push_back(green);

	idxes.push_back(s + 3);
	idxes.push_back(s + 1);
	idxes.push_back(s);

	idxes.push_back(s + 3);
	idxes.push_back(s + 2);
	idxes.push_back(s + 1);

	// R - 8
	s = positions.size();
	positions.push_back(ubr);
	positions.push_back(utr);
	positions.push_back(dtr);
	positions.push_back(dbr);

	for (int i = 0; i < 4; i++) colors.push_back(blue);

	idxes.push_back(s + 3);
	idxes.push_back(s + 1);
	idxes.push_back(s);

	idxes.push_back(s + 3);
	idxes.push_back(s + 2);
	idxes.push_back(s + 1);

	// D - 12
	s = positions.size();
	positions.push_back(dbl);
	positions.push_back(dbr);
	positions.push_back(dtr);
	positions.push_back(dtl);

	for (int i = 0; i < 4; i++) colors.push_back(yellow);

	idxes.push_back(s + 3);
	idxes.push_back(s + 1);
	idxes.push_back(s);

	idxes.push_back(s + 3);
	idxes.push_back(s + 2);
	idxes.push_back(s + 1);

	// U - 16
	s = positions.size();
	positions.push_back(utl);
	positions.push_back(utr);
	positions.push_back(ubr);
	positions.push_back(ubl);

	for (int i = 0; i < 4; i++) colors.push_back(white);

	idxes.push_back(s + 3);
	idxes.push_back(s + 1);
	idxes.push_back(s);

	idxes.push_back(s + 3);
	idxes.push_back(s + 2);
	idxes.push_back(s + 1);

	// B - 20
	s = positions.size();
	positions.push_back(utr);
	positions.push_back(utl);
	positions.push_back(dtl);
	positions.push_back(dtr);

	for (int i = 0; i < 4; i++) colors.push_back(orange);

	idxes.push_back(s + 3);
	idxes.push_back(s + 1);
	idxes.push_back(s);

	idxes.push_back(s + 3);
	idxes.push_back(s + 2);
	idxes.push_back(s + 1);

	glGenVertexArrays(1, &p_vao);
	glBindVertexArray(p_vao);
	{

		glGenBuffers(1, &p_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, p_vbo);
		{
			glBufferData(GL_ARRAY_BUFFER, sizeof(cgmath::vec3) * positions.size(), positions.data(), GL_DYNAMIC_DRAW);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glGenBuffers(1, &c_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, c_vbo);
		{
			glBufferData(GL_ARRAY_BUFFER, sizeof(cgmath::vec3) * colors.size(), colors.data(), GL_DYNAMIC_DRAW);

			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glGenBuffers(1, &i_vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, i_vbo);
		{
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * idxes.size(), idxes.data(), GL_STATIC_DRAW);
		}

	}
	glBindVertexArray(0);

	aspect = 1.0f;
}

void scene_cube::mainLoop() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(shader_program);
	{
		// 'static' uniforms
		auto camera_model = cgmath::mat4(1.0f);
		camera_model[3][2] = 10.0f;
		auto view_m = cgmath::mat4::inverse(camera_model);

		float t = time_util::elapsed_time().count() / 1;
		auto model_m = (
			cgmath::mat4({
				{cos(30.0f * DEG_RAD * t), sin(30.0f * DEG_RAD * t), 0.0f, 0.0f},
				{-sin(30.0f * DEG_RAD * t), cos(30.0f * DEG_RAD * t), 0.0f, 0.0f},
				{0.0f, 0.0f, 1.0f, 0.0f},
				{0.0f, 0.0f, 0.0f, 1.0f},
			}) *
			cgmath::mat4({
				{cos(60.0f * DEG_RAD * t), 0.0f, -sin(60.0f * DEG_RAD * t), 0.0f},
				{0.0f, 1.0f, 0.0f, 0.0f},
				{sin(60.0f * DEG_RAD * t), 0.0f, cos(60.0f * DEG_RAD * t), 0.0f},
				{0.0f, 0.0f, 0.0f, 1.0f},
			}) *
			cgmath::mat4({
				{1.0f, 0.0f, 0.0f, 0.0f},
				{0.0f, cos(30 * DEG_RAD * t), sin(30.0f * DEG_RAD * t), 0.0f},
				{0.0f, -sin(30 * DEG_RAD * t), cos(30.0f * DEG_RAD * t), 0.0f},
				{0.0f, 0.0f, 0.0f, 1.0f},
			})
		);

		float
			far = 1000.0f,
			near = 1.0f,
			fov = 60 * DEG_RAD;

		auto proj_m = cgmath::mat4({
			{1.0f / (aspect * tan(fov / 2.0f)), 0.0f, 0.0f, 0.0f},
			{0.0f, 1.0f / tan(fov / 2.0f), 0.0f, 0.0f},
			{0.0f, 0.0f, -((far + near) / (far - near)), -1.0f},
			{0.0f, 0.0f, -((2.0f * far * near) / (far - near)), 1.0f},
		});

		auto mvp = ( proj_m * view_m * model_m );

		glUniformMatrix4fv(glGetUniformLocation(shader_program, "mvp"), 1, GL_FALSE, &mvp[0][0]);
		glBindVertexArray(p_vao);
		{
			glDrawElements(GL_TRIANGLES, idxes.size(), GL_UNSIGNED_INT, nullptr);
			// glDrawElements(GL_POINTS, idxes.size(), GL_UNSIGNED_INT, nullptr);
		}
		glBindVertexArray(0);
	}
	glUseProgram(0);
}

void scene_cube::resize(int width, int height) {
	aspect = (float)width / (float)height;
	glViewport(0, 0, width, height);
}
