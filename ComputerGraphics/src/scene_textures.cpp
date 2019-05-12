#include <vector>
#include <iostream>
#include <math.h>
#include <IL/il.h>

#include "scene_textures.h"
#include "ifile.h"
#include "time_util.h"

#include "mat4.h"


#define  PI_F 3.14159265358979f
#define  DEG_RAD (PI_F / 180.0f)

scene_textures::~scene_textures() {
	glDeleteProgram(shader_program);
}

void scene_textures::awake() {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_PROGRAM_POINT_SIZE);
}

void scene_textures::sleep() {
	glClearColor(1.0f, 1.0f, 0.5f, 1.0f);
	glDisable(GL_PROGRAM_POINT_SIZE);
}

void scene_textures::setup_shaders() {
	ifile shader_file;

	shader_file.read("shaders/textures.vert");
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


	shader_file.read("shaders/textures.frag");
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
	glBindAttribLocation(shader_program, 1, "TextureCoordinates");

	glLinkProgram(shader_program);
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
}

void scene_textures::setup_textures() {
	ILuint imageID, imageID1;

	ilGenImages(1, &imageID);
	ilBindImage(imageID);
	{
		ilLoadImage("images/metal.jpg");
		glGenTextures(1, &metal_texture);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, metal_texture);
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				ilGetInteger(IL_IMAGE_FORMAT),
				ilGetInteger(IL_IMAGE_WIDTH),
				ilGetInteger(IL_IMAGE_HEIGHT),
				0,
				ilGetInteger(IL_IMAGE_FORMAT),
				ilGetInteger(IL_IMAGE_TYPE),
				ilGetData()
			);

		}
		glBindTexture(GL_TEXTURE_2D, 0);
		// glDeleteTextures(1, &metal_texture);

	}
	ilBindImage(0);
	ilDeleteImages(1, &imageID);

	ilGenImages(1, &imageID1);
	ilBindImage(imageID1);
	{
		ilLoadImage("images/lucina.jpg");

		glGenTextures(1, &lucina_texture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, lucina_texture);
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				ilGetInteger(IL_IMAGE_FORMAT),
				ilGetInteger(IL_IMAGE_WIDTH),
				ilGetInteger(IL_IMAGE_HEIGHT),
				0,
				ilGetInteger(IL_IMAGE_FORMAT),
				ilGetInteger(IL_IMAGE_TYPE),
				ilGetData()
			);

		}
		glBindTexture(GL_TEXTURE_2D, 0);
		// glDeleteTextures(1, &lucina_texture);
	}
	ilBindImage(0);
	ilDeleteImages(1, &imageID1);
}

void scene_textures::init() {
	this->setup_shaders();
	this->setup_textures();
	{
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

		texture_cordinates.push_back({ 0.0f,1.0f });
		texture_cordinates.push_back({ 1.0f,1.0f });
		texture_cordinates.push_back({ 1.0f,0.0f });
		texture_cordinates.push_back({ 0.0f,0.0f });

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

		texture_cordinates.push_back({ 0.0f,1.0f });
		texture_cordinates.push_back({ 1.0f,1.0f });
		texture_cordinates.push_back({ 1.0f,0.0f });
		texture_cordinates.push_back({ 0.0f,0.0f });

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

		texture_cordinates.push_back({ 0.0f,1.0f });
		texture_cordinates.push_back({ 1.0f,1.0f });
		texture_cordinates.push_back({ 1.0f,0.0f });
		texture_cordinates.push_back({ 0.0f,0.0f });

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

		texture_cordinates.push_back({ 0.0f,1.0f });
		texture_cordinates.push_back({ 1.0f,1.0f });
		texture_cordinates.push_back({ 1.0f,0.0f });
		texture_cordinates.push_back({ 0.0f,0.0f });

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

		texture_cordinates.push_back({ 0.0f,1.0f });
		texture_cordinates.push_back({ 1.0f,1.0f });
		texture_cordinates.push_back({ 1.0f,0.0f });
		texture_cordinates.push_back({ 0.0f,0.0f });

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

		texture_cordinates.push_back({ 0.0f,1.0f });
		texture_cordinates.push_back({ 1.0f,1.0f });
		texture_cordinates.push_back({ 1.0f,0.0f });
		texture_cordinates.push_back({ 0.0f,0.0f });

		idxes.push_back(s + 3);
		idxes.push_back(s + 1);
		idxes.push_back(s);

		idxes.push_back(s + 3);
		idxes.push_back(s + 2);
		idxes.push_back(s + 1);

		aspect = 1.0f;
	}

	glGenVertexArrays(1, &p_vao);
	glBindVertexArray(p_vao);
	{
		// positions
		glGenBuffers(1, &p_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, p_vbo);
		{
			glBufferData(GL_ARRAY_BUFFER, sizeof(cgmath::vec3) * positions.size(), positions.data(), GL_STATIC_DRAW);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// texture_coordinates
		glGenBuffers(1, &c_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, c_vbo);
		{
			glBufferData(GL_ARRAY_BUFFER, sizeof(cgmath::vec2) * texture_cordinates.size(), texture_cordinates.data(), GL_DYNAMIC_DRAW);

			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
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

void scene_textures::mainLoop() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(shader_program);
	{
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

		auto mvp = (proj_m * view_m * model_m);

		glUniform1i(glGetUniformLocation(shader_program, "metal"), 0);
		glUniform1i(glGetUniformLocation(shader_program, "lucina"), 1);

		glUniformMatrix4fv(glGetUniformLocation(shader_program, "mvp"), 1, GL_FALSE, &mvp[0][0]);
		glBindVertexArray(p_vao);
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, metal_texture);

			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, lucina_texture);

				glDrawElements(GL_TRIANGLES, idxes.size(), GL_UNSIGNED_INT, nullptr);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, 0);

			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		glBindVertexArray(0);
	}
	glUseProgram(0);
}

void scene_textures::resize(int width, int height) {
	aspect = (float)width / (float)height;
	glViewport(0, 0, width, height);
}
