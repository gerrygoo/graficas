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

	glUseProgram(shader_program);
	{
		glUniform2f(glGetUniformLocation(shader_program, "iResolution"), 400.0f, 400.0f);
	}
	glUseProgram(0);

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
}

void scene_cube::init() {
	this->setup_shaders();

	auto blue = cgmath::vec3(66.0f, 134.0f, 244.0f)/255.0f;
	auto orange = cgmath::vec3(244.0f, 152.0f, 65.0f) / 255.0f;
	auto white = cgmath::vec3(255.0f, 250.0f, 230.0f) / 255.0f;
	auto green = cgmath::vec3(98.0f, 196.0f, 53.0f) / 255.0f;
	auto yellow = cgmath::vec3(255.0f, 243.0f, 89.0f) / 255.0f;
	auto red = cgmath::vec3(249.0f, 89.0f, 89.0f) / 255.0f;

	// +z axis outwards, towards 'me' 
	
	// F - 0
	positions.push_back(cgmath::vec3({ -3.0f, 3.0f, 3.0f }));
	positions.push_back(cgmath::vec3({ 3.0f, 3.0f, 3.0f }));
	positions.push_back(cgmath::vec3({ 3.0f, -3.0f, 3.0f }));
	positions.push_back(cgmath::vec3({ -3.0f, -3.0f, 3.0f }));

	for (int i = 0; i < 4; i++) colors.push_back(blue);

	idxes.push_back(0);
	idxes.push_back(3);
	idxes.push_back(1);

	idxes.push_back(1);
	idxes.push_back(3);
	idxes.push_back(2);

	// L - 4
	positions.push_back(cgmath::vec3({-3.0f, 3.0f, -3.0f }));
	positions.push_back(cgmath::vec3({ -3.0f, 3.0f, 3.0f }));
	positions.push_back(cgmath::vec3({ -3.0f, -3.0f, 3.0f }));
	positions.push_back(cgmath::vec3({ -3.0f, -3.0f, -3.0f }));

	for (int i = 0; i < 4; i++) colors.push_back(white);

	idxes.push_back(4);
	idxes.push_back(7);
	idxes.push_back(5);

	idxes.push_back(5);
	idxes.push_back(7);
	idxes.push_back(6);

	// R - 8
	positions.push_back(cgmath::vec3({ 3.0f, 3.0f, 3.0f }));
	positions.push_back(cgmath::vec3({ 3.0f, 3.0f, -3.0f }));
	positions.push_back(cgmath::vec3({ 3.0f, -3.0f, -3.0f }));
	positions.push_back(cgmath::vec3({ 3.0f, -3.0f, 3.0f }));

	for (int i = 0; i < 4; i++) colors.push_back(orange);

	idxes.push_back(8);
	idxes.push_back(11);
	idxes.push_back(9);

	idxes.push_back(9);
	idxes.push_back(11);
	idxes.push_back(10);


	// D - 12
	positions.push_back(cgmath::vec3({ -3.0f, -3.0f, 3.0f }));
	positions.push_back(cgmath::vec3({ 3.0f, -3.0f, 3.0f }));
	positions.push_back(cgmath::vec3({ 3.0f, -3.0f, -3.0f }));
	positions.push_back(cgmath::vec3({ -3.0f, -3.0f, -3.0f }));

	for (int i = 0; i < 4; i++) colors.push_back(green);

	idxes.push_back(12);
	idxes.push_back(15);
	idxes.push_back(13);

	idxes.push_back(13);
	idxes.push_back(15);
	idxes.push_back(14);

	// U - 16
	positions.push_back(cgmath::vec3({ -3.0f, 3.0f, -3.0f }));
	positions.push_back(cgmath::vec3({ 3.0f, 3.0f, -3.0f }));
	positions.push_back(cgmath::vec3({ 3.0f, 3.0f, 3.0f }));
	positions.push_back(cgmath::vec3({ -3.0f, 3.0f, 3.0f }));
	
	for (int i = 0; i < 4; i++) colors.push_back(red);

	idxes.push_back(16);
	idxes.push_back(19);
	idxes.push_back(17);

	idxes.push_back(17);
	idxes.push_back(19);
	idxes.push_back(18);

	// B - 20
	positions.push_back(cgmath::vec3({ 3.0f, 3.0f, -3.0f }));
	positions.push_back(cgmath::vec3({ -3.0f, 3.0f, -3.0f }));
	positions.push_back(cgmath::vec3({ -3.0f, -3.0f, -3.0f }));
	positions.push_back(cgmath::vec3({ 3.0f, -3.0f, -3.0f }));


	for (int i = 0; i < 4; i++) colors.push_back(yellow);

	std::cout << positions.size() << "   " << colors.size();

	idxes.push_back(20);
	idxes.push_back(23);
	idxes.push_back(21);

	idxes.push_back(21);
	idxes.push_back(23);
	idxes.push_back(22);

	glGenVertexArrays(1, &p_vao);
	glBindVertexArray(p_vao);
	{

		glGenBuffers(1, &p_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, p_vbo);
		{
			glBufferData(GL_ARRAY_BUFFER, sizeof cgmath::vec2 * positions.size(), positions.data(), GL_STATIC_DRAW);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glGenBuffers(1, &c_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, c_vbo);
		{
			glBufferData(GL_ARRAY_BUFFER, sizeof cgmath::vec3 * colors.size(), colors.data(), GL_DYNAMIC_DRAW);

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

}

void scene_cube::mainLoop() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(shader_program);
	{
		// 'static' uniforms
		auto camera_model = cgmath::mat4(1.0f);
		camera_model[3][2] = 10.0f;

		auto view_m = cgmath::mat4::inverse(camera_model);

		float t = time_util::elapsed_time().count();
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
			aspect = 1.0f,
			far = 1000.0f,
			near = 1.0f,
			fov = 60 * DEG_RAD;

		auto proj_m = cgmath::mat4({
			{1.0f / (aspect * tan(fov / 2.0f)), 0.0f, 0.0f, 0.0f},
			{0.0f, 1 / tan(fov / 2.0f), 0.0f, 0.0f},
			{0.0f, 0.0f, -(far + near) / (far - near), -1.0f},
			{0.0f, 0.0f, -(2.0f * far * near) / (far - near), 0.0f},
			});

		auto mvp = (
			proj_m
			* view_m
			* model_m
		);

		glUniform1f(glGetUniformLocation(shader_program, "iTime"), time_util::elapsed_time().count());
		glUniformMatrix4fv(glGetUniformLocation(shader_program, "mvp"), 1, GL_FALSE, &mvp[0][0]);
		glBindVertexArray(p_vao);
		{
			glDrawElements(GL_TRIANGLES, idxes.size(), GL_UNSIGNED_INT, nullptr);
		}
		glBindVertexArray(0);
	}
	glUseProgram(0);
}

void scene_cube::resize(int width, int height) {
	glViewport(0, 0, width, height);
	glUseProgram(shader_program);
	glUniform2f(glGetUniformLocation(shader_program, "iResolution"), width, height);
	glUseProgram(0);
}
