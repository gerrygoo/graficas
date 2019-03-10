#include <vector>
#include <iostream>


#include "scene_fragment.h"
#include "ifile.h"
#include "time_util.h"
#include "vec2.h"


scene_fragment::~scene_fragment() {
	glDeleteProgram(shader_program);
}

void scene_fragment::init() {

	std::vector<cgmath::vec2> p;
	p.push_back(cgmath::vec2(-1.0f, -1.0f));
	p.push_back(cgmath::vec2(1.0f, -1.0f));
	p.push_back(cgmath::vec2(1.0f, 1.0f));
	p.push_back(cgmath::vec2(-1.0f, 1.0f));

	std::vector<unsigned int> idxes{ 0, 1, 3, 1, 2, 3 };

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	{
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		{
			glBufferData(GL_ARRAY_BUFFER, sizeof cgmath::vec2 * p.size(), p.data(), GL_STATIC_DRAW);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);


		glGenBuffers(1, &idxvbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		{
			glBufferData(GL_ARRAY_BUFFER, sizeof cgmath::vec2 * idxes.size(), idxes.data(), GL_STATIC_DRAW);
		}
	}
	glBindVertexArray(0);

	ifile shader_file;

	shader_file.read("shaders/coco.vert");
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


	shader_file.read("shaders/coco.frag");
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
}

void scene_fragment::awake() {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_PROGRAM_POINT_SIZE);
}

void scene_fragment::sleep() {
	glClearColor(1.0f, 1.0f, 0.5f, 1.0f);
	glDisable(GL_PROGRAM_POINT_SIZE);
}

void scene_fragment::mainLoop() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(shader_program);
	{
		GLuint time_location = glGetUniformLocation(shader_program, "time");
		glUniform1f(time_location, time_util::elapsed_time().count());
		glDrawArrays(GL_LINE_STRIP, 0, 100);
	}
	glUseProgram(0);
}

void scene_fragment::resize(int width, int height)
{
}
