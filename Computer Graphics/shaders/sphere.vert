#version 450

#define two_pi radians(360.0) 
#define rot_x mat3(1, 0, 0, 0, cos(time), -sin(time), 0, sin(time), cos(time))
#define rot_z mat3(cos(time), -sin(time), 0, sin(time), cos(time), 0, 0, 0, 1)

uniform float time;

out vec4 InterpolatedColor;

void main() {
  float	
    per_circle = 100.0,
    r = 0.8,
    
    x = mod(gl_VertexID, per_circle),
    y = floor(gl_VertexID / per_circle),
    
    ph = x / per_circle * two_pi,
    th = y / per_circle * two_pi;
  
  vec3 pos = vec3(
  	r * sin(ph) * cos(th),
    r * sin(ph) * sin(th),
    r * cos(ph)
  );
  
  gl_Position = vec4(
    rot_x * rot_z * pos,
    1
  );
  
  gl_PointSize = 5.0;
  InterpolatedColor = vec4(0, 0.6, 0.8, 1.0);
}