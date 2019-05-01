#include <vector>
#include <iostream>
#include <math.h>

#include "ifile.h"

#include "scene_particles.h"
#include "vec3.h"

#define PI 3.14159265f
#define TWOPI 6.28318530718f
#define rand_max 32767.0f + 1.0f

inline float drand48() {
    return float(rand()) / rand_max;
}

inline float mix(float x, float y, float a) {
    return x + a * (y - x);
}

scene_particles::~scene_particles() { }

void scene_particles::resize(int width, int height) {
    aspect = (float)width / (float)height;
	glViewport(0, 0, width, height);
}

void print_shader_compile_errors(GLuint shader) {
    GLint vertex_compiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &vertex_compiled);
	if (vertex_compiled != GL_TRUE) {
		GLint log_length;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);
		std::vector<char> log(log_length);
		glGetShaderInfoLog(shader, log_length, &log_length, log.data());
		for (auto c : log) std::cout << c;
	}
}

void print_program_link_errors(GLuint program) {
    GLint program_linked;
	glGetShaderiv(program, GL_COMPILE_STATUS, &program_linked);
    if (program_linked != GL_TRUE) {
		GLint log_length;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);
		std::vector<char> log(log_length);
		glGetProgramInfoLog(program, log_length, &log_length, log.data());
		for (auto c : log) std::cout << c;
	}
}

void scene_particles::setup_program_with_shaders() {
    program = glCreateProgram();

    ifile shader_file;

    shader_file.read("shaders/particles.vert");
    std::string vertex_source = shader_file.get_contents();
    const GLchar* vertex_source_cstr = (const GLchar*)vertex_source.c_str();

    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_source_cstr, nullptr);
	glCompileShader(vertex_shader);
    print_shader_compile_errors(vertex_shader);

    glAttachShader(program, vertex_shader);


    shader_file.read("shaders/particles.frag");
    std::string fragment_source = shader_file.get_contents();
    const GLchar* fragment_source_cstr = (const GLchar*)fragment_source.c_str();

    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragment_source_cstr, nullptr);
	glCompileShader(fragment_shader);
    print_shader_compile_errors(fragment_shader);

    glAttachShader(program, fragment_shader);


    const char * outputNames[] = { "Position", "Velocity", "StartTime" };
    glTransformFeedbackVaryings(program, 3, outputNames, GL_SEPARATE_ATTRIBS);


    glLinkProgram(program);

    glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

    glUseProgram(program);
}

void scene_particles::setup_uniforms() {
    now_uniform_location = glGetUniformLocation(program, "now");
    delta_time_uniform_location = glGetUniformLocation(program, "delta_time");
    acceleration_uniform_location = glGetUniformLocation(program, "acceleration");
    time_to_live_uniform_location = glGetUniformLocation(program, "time_to_live");
    mvp_uniform_location = glGetUniformLocation(program, "mvp");

    render_subroutine_idx = glGetSubroutineIndex(program, GL_VERTEX_SHADER, "render");
    update_subroutine_idx = glGetSubroutineIndex(program, GL_VERTEX_SHADER, "update");
}

void scene_particles::setup_buffers() {
    glGenBuffers(2, position_buffers);
    glGenBuffers(2, velocity_buffers);
    glGenBuffers(2, start_time_buffers);
    glGenBuffers(1, &initial_velocity_buffer);

    const unsigned int sizeof_3f_buffer = sizeof(float) * 3 * particle_count;
    const unsigned int sizeof_1f_buffer = sizeof(float) * particle_count;

    // alloc
    glBindBuffer(GL_ARRAY_BUFFER, initial_velocity_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof_3f_buffer, NULL, GL_DYNAMIC_COPY);
    for (int i = 0; i < 2; i++) {
        glBindBuffer(GL_ARRAY_BUFFER, position_buffers[i]);
        glBufferData(GL_ARRAY_BUFFER, sizeof_3f_buffer, NULL, GL_DYNAMIC_COPY);

        glBindBuffer(GL_ARRAY_BUFFER, velocity_buffers[i]);
        glBufferData(GL_ARRAY_BUFFER, sizeof_3f_buffer, NULL, GL_DYNAMIC_COPY);

        glBindBuffer(GL_ARRAY_BUFFER, start_time_buffers[i]);
        glBufferData(GL_ARRAY_BUFFER, sizeof_1f_buffer, NULL, GL_DYNAMIC_COPY);
    }


    // initial fill, TODO simplify

    // positions start at 0
    // TODO RANDOMIZE GIVEN SPAWN AREA UNIFORM ATTRS
    GLfloat *data = new GLfloat[particle_count * 3];
    for( int i = 0; i < particle_count * 3; i++ ) data[i] = 0.0f;

    glBindBuffer(GL_ARRAY_BUFFER, position_buffers[0]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof_3f_buffer, data);


    // random angles for initial velocities
    // TODO RANDOMIZE GIVEN SPAWN AREA UNIFORM ATTRS
    cgmath::vec3 v(0.0f);
    float velocity, theta, phi;
    for( int i = 0; i < particle_count; i++ ) {
        theta = mix(0.0f, (float)PI / 6.0f, drand48());
        phi = mix(0.0f, (float)TWOPI, drand48());

        v.x = sin(theta) * cos(phi);
        v.y = cos(theta);
        v.z = sin(theta) * sin(phi);

        velocity = mix(1.25f, 1.5f, drand48());
        v.normalize();
        v *= velocity;

        data[3*i]   = v.x;
        data[3*i+1] = v.y;
        data[3*i+2] = v.z;
    }
    glBindBuffer(GL_ARRAY_BUFFER, velocity_buffers[0]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof_3f_buffer, data);

    glBindBuffer(GL_ARRAY_BUFFER, initial_velocity_buffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof_3f_buffer, data);




}

void scene_particles::init() {
    particle_count = 1000;

    glPointSize(10.0f);

    this->setup_program_with_shaders();

	this->setup_uniforms();
    this->setup_buffers();

}

void scene_particles::mainLoop() {

}
