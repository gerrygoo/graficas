#include <vector>
#include <iostream>


#include "scene_circle_grid.h"
#include "ifile.h"
#include "time_util.h"


scene_circle_grid::~scene_circle_grid() {
	glDeleteProgram(shader_program);
}

void scene_circle_grid::init() {
	ifile shader_file;

	shader_file.read("shaders/circle_grid.vert");
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


	shader_file.read("shaders/solid_color.frag");
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
	glLinkProgram(shader_program);

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	t = 62;
}

void scene_circle_grid::awake() {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_PROGRAM_POINT_SIZE);
}

void scene_circle_grid::sleep() {
	glClearColor(1.0f, 1.0f, 0.5f, 1.0f);
	glDisable(GL_PROGRAM_POINT_SIZE);
}

void scene_circle_grid::mainLoop() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(shader_program);
	{
		GLuint time_location = glGetUniformLocation(shader_program, "time");
		glUniform1f(time_location, time_util::elapsed_time().count());

		for (int i = 0; i < 100; i++) glDrawArrays(GL_TRIANGLE_STRIP, (t * i), t);

	}
	glUseProgram(0);
}

void scene_circle_grid::resize(int width, int height) { }

void scene_circle_grid::normalKeysDown(unsigned char key) {
	if (key == '1') t++;
	if (key == '2') t--;
}