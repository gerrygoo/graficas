#version 420

subroutine void type_of_render_fn();

layout (location = 0) in vec3 particle_position;
layout (location = 1) in vec3 particle_velocity;
layout (location = 2) in float particle_start_time;
layout (location = 3) in vec3 particle_inital_velocity;

layout (location = 4) in vec3 ShapePosition;


out vec3 particle_simulated_position;   // tf varying
out vec3 particle_simulated_velocity;   // tf varying

out vec3 interpolated_color;
out vec2 tex_coord;

uniform vec3 acceleration;
uniform vec3 emisor_position;
uniform float emisro_width;
uniform float emisor_height;

uniform image3D positions_texture;

uniform float now;
uniform float delta_time;

uniform mat4 view_matrix;
uniform mat4 projection_matrix;
uniform vec3 camera_position;

subroutine uniform type_of_render_fn render_fn;

float rand(float x) {
	return fract(sin(x)*100000.0);
}

subroutine (type_of_render_fn) void render() {
    interpolated_color = normalize(vec3( 50 + particle_position.x, 50 + particle_position.y, 100 + particle_position.z));

	switch (gl_VertexID) {
		case 0:
			tex_coord = vec2(0.0f, 1.0f);
			break;
		case 1:
			tex_coord = vec2(0.0f, 0.0f);
			break;
		case 2:
			tex_coord = vec2(1.0f, 1.0f);
			break;
		case 3:
			tex_coord = vec2(1.0f, 1.0f);
			break;
		case 4:
			tex_coord = vec2(0.0f, 0.0f);
			break;
		case 5:
			tex_coord = vec2(1.0f, 0.0f);
			break;
	}

    float scale = 1.0f;
    vec3 position = particle_position;
    mat4 model_matrix = mat4(
        vec4(scale, 0.0f, 0.0f, 0.0f),
        vec4(0.0f, scale, 0.0f, 0.0f),
        vec4(0.0f, 0.0f, scale, 0.0f),
        vec4( position.x, position.y, position.z, 1.0f)
    );
    mat4 vm = ( view_matrix * model_matrix );


    vm[0][0] = 1.0f;
    vm[0][1] = 0.0f;
    vm[0][2] = 0.0f;

    vm[1][0] = 0.0f;
    vm[1][1] = 1.0f;
    vm[1][2] = 0.0f;

    vm[2][0] = 0.0f;
    vm[2][1] = 0.0f;
    vm[2][2] = 1.0f;

    gl_Position = ( projection_matrix * vm ) * vec4(ShapePosition, 1.0);
}

subroutine (type_of_render_fn) void update() {
    particle_simulated_position = particle_position;
    particle_simulated_velocity = particle_velocity;

    particle_simulated_position += particle_simulated_velocity * delta_time;

    vec3 to_camera_acceleration = 20.0f * normalize(camera_position - particle_position);

    particle_simulated_velocity += to_camera_acceleration * delta_time;
}

void main() {
    render_fn();
}