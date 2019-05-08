#version 410

subroutine void type_of_render_fn();

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexVelocity;
layout (location = 2) in float VertexStartTime;
layout (location = 3) in vec3 VertexInitialVelocity;


out vec3 Position;   // tf varying
out vec3 Velocity;   // tf varying
out float StartTime; // tf varying

out vec3 interpolated_color;

uniform float time_to_live;
uniform vec3 acceleration;
uniform vec3 emisor_position;
uniform float emisro_width;
uniform float emisor_height;


uniform float now;
uniform float delta_time;
uniform mat4 mvp;
subroutine uniform type_of_render_fn render_fn;

float rand(float x) {
	return fract(sin(x)*100000.0);
}

subroutine (type_of_render_fn) void render() {
    interpolated_color = normalize(vec3(
        50 + VertexPosition.x,
        50 + VertexPosition.y,
        100 + VertexPosition.z
    ));
    gl_Position = mvp * vec4(VertexPosition, 1.0);
}

subroutine (type_of_render_fn) void update() {
    Position = VertexPosition;
    Velocity = VertexVelocity;
    StartTime = VertexStartTime;

    if( now >= StartTime ) {

        float age = now - StartTime;

        if( age < time_to_live ) {
            // alive, simulate
            Position += Velocity * delta_time;
            Velocity += acceleration * delta_time;
        } else {
            // past ttl, reset
            // Position = vec3(0.0f);
            Position = vec3(
                mix(emisor_position.x - (emisro_width / 2.0f), emisor_position.x + (emisro_width / 2.0f), rand( VertexPosition.x + delta_time )),
                mix(emisor_position.y - (emisor_height / 2.0f), emisor_position.y + (emisor_height / 2.0f), rand( VertexPosition.y + delta_time )),
                mix(emisor_position.x - (emisro_width / 2.0f), emisor_position.x + (emisro_width / 2.0f), rand( VertexPosition.z + delta_time ))
            );
            Velocity = VertexInitialVelocity;
            StartTime = now;
        }

    }
}

void main() {
    render_fn();
}