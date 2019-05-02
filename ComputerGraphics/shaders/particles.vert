#version 410

subroutine void type_of_render_fn();

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexVelocity;
layout (location = 2) in float VertexStartTime;
layout (location = 3) in vec3 VertexInitialVelocity;


out vec3 Position;   // tf varying
out vec3 Velocity;   // tf varying
out float StartTime; // tf varying

uniform vec3 acceleration;
uniform float time_to_live;

uniform float now;
uniform float delta_time;
uniform mat4 mvp;
subroutine uniform type_of_render_fn render_fn;

subroutine (type_of_render_fn) void render() {
    // gl_Position = mvp * vec4(VertexPosition, 1.0);
    // gl_Position = vec4(VertexPosition, 1.0);
    gl_Position = vec4(0.0, 0.0, -0.5, 1.0);
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
            Position = vec3(0.0); // TODO RANDOMIZE GIVEN SPAWN AREA UNIFORM ATTRS
            Velocity = VertexInitialVelocity;
            StartTime = now;
        }

    }
}

void main() {
    render_fn();
    // gl_Position = vec4(0.0, 0.0, 0.0, 1.0);
}