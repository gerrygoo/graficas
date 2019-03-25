#include <vector>
#include <iostream>
#include <math.h>

#include "scene_circle.h"
#include "ifile.h"
#include "time_util.h"


#define  PI_F 3.14159265358979f
#define  DEG_RAD (PI_F / 180.0f)

scene_circle::~scene_circle() {
	glDeleteProgram(shader_program);
}

void scene_circle::awake() {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_PROGRAM_POINT_SIZE);
}

void scene_circle::sleep() {
	glClearColor(1.0f, 1.0f, 0.5f, 1.0f);
	glDisable(GL_PROGRAM_POINT_SIZE);
}

void scene_circle::setup_shaders() {
	ifile shader_file;

	shader_file.read("shaders/circle.vert");
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


	shader_file.read("shaders/circle.frag");
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

void scene_circle::init() {
	this->setup_shaders();

	positions.push_back(cgmath::vec2({ 0.0f, 0.0f }));

	colors.push_back(cgmath::vec3({ 1.0f, 1.0f, 1.0f }));
	
	for (int i = 0; i <= 361; i++) {
		float rads = i * DEG_RAD;
		positions.push_back(cgmath::vec2({
			1.0f * cos(rads),
			1.0f * sin(rads)
		}));
		colors.push_back(cgmath::vec3({ 0.0f, sin(rads) * 0.9f, cos(rads) }));
	}

	glGenVertexArrays(1, &p_vao);
	glBindVertexArray(p_vao);
	{

		glGenBuffers(1, &p_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, p_vbo);
		{
			glBufferData(GL_ARRAY_BUFFER, sizeof cgmath::vec2 * positions.size(), positions.data(), GL_DYNAMIC_DRAW);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glGenBuffers(1, &c_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, c_vbo);
		{
			glBufferData(GL_ARRAY_BUFFER, sizeof cgmath::vec3 * colors.size(), colors.data(), GL_DYNAMIC_DRAW);

			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
		}
		glBindBuffer(GL_ARRAY_BUFFER, 1);

	}
	glBindVertexArray(0);
}

void scene_circle::mainLoop() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(shader_program);
	{
		glBindVertexArray(p_vao);
		{
			glDrawArrays(GL_TRIANGLE_FAN, 0, positions.size());
		}
		glBindVertexArray(0);
	}
	glUseProgram(0);
}
