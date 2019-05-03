#include <vector>
#include <iostream>
#include <math.h>
#include <algorithm>
#include "ifile.h"

#include "time_util.h"

#include "scene_particles.h"
#include "vec3.h"

#define  PI 3.14159265358979f
#define TWOPI 6.28318530718f
#define  DEG_RAD (PI / 180.0f)
#define rand_max 32767.0f + 1.0f

inline float mix(float x, float y, float a) {
    return x + a * (y - x);
}

scene_particles::~scene_particles() { glDeleteProgram(program); }
void scene_particles::awake() { glClearColor(0.0f, 0.0f, 0.0f, 1.0f); }
void scene_particles::sleep() { glClearColor(0.0f, 0.0f, 0.0f, 1.0f); }
void scene_particles::resize(int width, int height) {
    aspect = (float)width / (float)height;
	glViewport(0, 0, width, height);
}


void scene_particles::print_shader_compile_errors(GLuint shader) {
    GLint vertex_compiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &vertex_compiled);
	if (vertex_compiled != GL_TRUE) {
		GLint log_length;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);
        std::cout << "shader compile status: " << vertex_compiled << ", log length: " << log_length << std::endl;
		std::vector<char> log(log_length);
		glGetShaderInfoLog(shader, log_length, &log_length, log.data());
		for (auto c : log) std::cout << c;
	}
}

void scene_particles::print_program_link_errors() {
    GLint program_linked;
	glGetProgramiv(program, GL_LINK_STATUS, &program_linked);
    if (program_linked != GL_TRUE) {
		GLint log_length;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);
        std::cout << "program link status: " << program_linked << ", log length: " << log_length << std::endl;
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
    this->print_shader_compile_errors(vertex_shader);

    glAttachShader(program, vertex_shader);

    shader_file.read("shaders/particles.frag");
    std::string fragment_source = shader_file.get_contents();
    const GLchar* fragment_source_cstr = (const GLchar*)fragment_source.c_str();

    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragment_source_cstr, nullptr);
	glCompileShader(fragment_shader);
    this->print_shader_compile_errors(fragment_shader);

    glAttachShader(program, fragment_shader);


    const char * outputNames[] = { "Position", "Velocity", "StartTime" };
    glTransformFeedbackVaryings(program, 3, outputNames, GL_SEPARATE_ATTRIBS);


    glLinkProgram(program);
    this->print_program_link_errors();

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

    //#pragma region initial values

        // initial fill, TODO simplify

        // positions start at 0,0,0
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
            // theta = mix(0.0f, (float)PI / 6.0f, drand48());
            // phi = mix(0.0f, (float)TWOPI, drand48());

            // v.x = sin(theta) * cos(phi);
            // v.y = cos(theta);
            // v.z = sin(theta) * sin(phi);

            // velocity = mix(1.25f, 1.5f, drand48());
            // v.normalize();
            // v *= velocity;

            // data[3*i]   = v.x;
            // data[3*i+1] = v.y;
            // data[3*i+2] = v.z;
            data[3*i]   = (i + 1) % 3 == 0 ? 0.0f : 0;
            data[3*i+1] = (i + 1) % 3 == 1 ? 1.0f : 0;
            data[3*i+2] = (i + 1) % 3 == 2 ? 1.0f : 0;
        }
        glBindBuffer(GL_ARRAY_BUFFER, initial_velocity_buffer);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof_3f_buffer, data);

        glBindBuffer(GL_ARRAY_BUFFER, velocity_buffers[0]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof_3f_buffer, data);

        delete [] data;
        data = new GLfloat[particle_count];
        float time = 0.0f;
        float rate = 0.0001f;
        for( int i = 0; i < particle_count; i++ ) {
            data[i] = time;
            time += rate;
        }
        glBindBuffer(GL_ARRAY_BUFFER, start_time_buffers[0]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof_1f_buffer, data);
        delete [] data;

        glBindBuffer(GL_ARRAY_BUFFER,0);

    //#pragma endregion
    glGenVertexArrays(2, particle_vaos);
    for (int i = 0; i < 2; i++) {
        glBindVertexArray(particle_vaos[i]);
        glBindBuffer(GL_ARRAY_BUFFER, position_buffers[i]);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, velocity_buffers[i]);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, start_time_buffers[i]);
        glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, initial_velocity_buffer);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(3);
    }
    glBindVertexArray(0);

    glGenTransformFeedbacks(2, particle_tfos);
    for (int i = 0; i < 2; i++) {
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, particle_tfos[i]);
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, position_buffers[i]);
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, velocity_buffers[i]);
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, start_time_buffers[i]);
    }
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
}


void scene_particles::setup_camera() {
    camera.yaw = 0.0f;
    camera.pitch = 0.0f;
    camera.roll = 0.0f;

    camera.position = cgmath::vec3(0.0f, 0.0f, 1.0f);
    camera.up = cgmath::vec3(0.0f, 1.0f, 0.0f);
    camera.target = cgmath::vec3(
        cos(DEG_RAD * (camera.pitch)) * cos(DEG_RAD * (camera.yaw)),
        sin(DEG_RAD * (camera.pitch)),
        cos(DEG_RAD * (camera.pitch)) * sin(DEG_RAD * (camera.yaw))
    );

    camera.dirty = false;
}

void scene_particles::setup_matrices() {
    model = cgmath::mat4(1.0f);
    view = cgmath::mat4::look_at(camera.position, camera.target, camera.up);
    projection = cgmath::mat4(1.0f);

    // float
	// 		far = 1000.0f,
	// 		near = 0.01f,
	// 		fov = 60 * DEG_RAD;

    // projection = cgmath::mat4({
    //     {1.0f / (aspect * tan(fov / 2.0f)), 0.0f, 0.0f, 0.0f},
    //     {0.0f, 1.0f / tan(fov / 2.0f), 0.0f, 0.0f},
    //     {0.0f, 0.0f, -((far + near) / (far - near)), -1.0f},
    //     {0.0f, 0.0f, -((2.0f * far * near) / (far - near)), 0.0f},
    // });
}


void scene_particles::init() {
    particle_count = 100000 * 1; // 8;
    active_vao_idx = 1;


    glPointSize(5.0f);
    glClearColor(0.0f,0.0f,0.0f,1.0f);

    this->setup_program_with_shaders();
	this->setup_uniforms();
    this->setup_buffers();

    this->setup_camera();

    this->setup_matrices();
    this->set_mvp_uniform();

    glUniform1f(time_to_live_uniform_location, 8.0f);
    glUniform3f(acceleration_uniform_location, 0.0f, 0.0f, 0.0f);

    if ( glGetError() != GL_NO_ERROR ) std::cout << "ERROR!!!!" << std::endl;
}

void scene_particles::keysDown(int key) {
    float disp_speed = 0.5;
    float rot_speed = 45.0f;
    // std::cout << key << "   " << GLFW_KEY_UP << std::endl;
    if ( key == GLFW_KEY_W ) {
        camera.position.z -= disp_speed;
        camera.dirty = true;
    }
    if ( key == GLFW_KEY_S ) {
        camera.position.z += disp_speed;
        camera.dirty = true;
    }


    if ( key == GLFW_KEY_UP ) {
        camera.position.y += disp_speed;
        camera.dirty = true;
    }
    if ( key == GLFW_KEY_DOWN ) {
        camera.position.y -= disp_speed;
        camera.dirty = true;
    }


    if ( key == GLFW_KEY_LEFT ) {
        camera.position.x -= disp_speed;
        camera.dirty = true;
    }
    if ( key == GLFW_KEY_RIGHT ) {
        camera.position.x += disp_speed;
        camera.dirty = true;
    }


    if ( key == GLFW_KEY_I ) {
        camera.pitch = std::min(camera.pitch + rot_speed, 89.0f);
        camera.dirty = true;
    }
    if ( key == GLFW_KEY_K ) {
        camera.pitch = std::max(camera.pitch - rot_speed, -89.0f);
        camera.dirty = true;
    }

    if ( key == GLFW_KEY_J ) {
        camera.yaw += rot_speed;
        camera.dirty = true;
    }
    if ( key == GLFW_KEY_L ) {
        camera.yaw -= rot_speed;
        camera.dirty = true;
    }

    // if ( key == GLFW_KEY_J ) camera.rotation.y += rot_speed;
    // if ( key == GLFW_KEY_L ) camera.rotation.y -= rot_speed;
}

void scene_particles::set_mvp_uniform() {
    std::cout << "pitch: " << camera.pitch << ", yaw: " << camera.yaw << std::endl;

    camera.target[0] = cos(DEG_RAD * (camera.pitch)) * cos(DEG_RAD * (camera.yaw));
    camera.target[1] = sin(DEG_RAD * (camera.pitch));
    camera.target[2] = cos(DEG_RAD * (camera.pitch)) * sin(DEG_RAD * (camera.yaw)) ;

    // view = cgmath::mat4::look_at(camera.position, camera.target, camera.up);
    view = cgmath::mat4(1.0f);


    mvp = projection * view * model;
    glUniformMatrix4fv(mvp_uniform_location, 1, GL_FALSE, &mvp[0][0]);

    camera.dirty = false;
}

void scene_particles::set_delta_time_uniform() {
    glUniform1f(now_uniform_location, time_util::elapsed_time().count() );
    glUniform1f( delta_time_uniform_location, time_util::delta_time().count() ) ;
}

void scene_particles::mainLoop() {
    this->set_delta_time_uniform();

    // simulation
    glUniformSubroutinesuiv(GL_VERTEX_SHADER, 1, &update_subroutine_idx);

    glEnable(GL_RASTERIZER_DISCARD);

    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, particle_tfos[active_vao_idx]);
    glBeginTransformFeedback(GL_POINTS);
        glBindVertexArray(particle_vaos[1 - active_vao_idx]);
        glDrawArrays(GL_POINTS, 0, particle_count);
    glEndTransformFeedback();
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);

    glDisable(GL_RASTERIZER_DISCARD);

    // rendering
    glUniformSubroutinesuiv(GL_VERTEX_SHADER, 1, &render_subroutine_idx);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if ( camera.dirty )this->set_mvp_uniform();
    glBindVertexArray(particle_vaos[active_vao_idx]);
    glDrawArrays(GL_POINTS, 0, particle_count);

    active_vao_idx = 1 - active_vao_idx;
}
