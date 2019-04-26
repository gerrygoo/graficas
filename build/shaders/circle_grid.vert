#version 410
 
 uniform float time;

 out vec4 InterpolatedColor;

void main(){
  // circle
  float x = floor(gl_VertexID / 2.0);
  float y = mod(gl_VertexID + 1.0, 2.0);
  
  float tri_per_circle = 30.0;
  float ang = x/(tri_per_circle) * radians(360.0);
  
  float r = 2.0 - y;
  float rx = r * cos(ang);
  float ry = r * sin(ang);
  
  // grid
  float width = 10.0;
  float grid_idx = floor( (gl_VertexID) / ( (tri_per_circle + 1.0) * 2.0) );
  
  float x_g = 5.0 * mod(grid_idx, width);
  float y_g = 5.0 * floor(grid_idx / width);
 
  float u_g = x_g + cos(time + x_g * 0.2) * 0.4;
  float v_g = y_g + cos(time + y_g * 0.3) * 0.8;
  
  
  gl_Position = vec4(
    (vec2(rx + u_g, ry + v_g) * 0.04) - vec2(0.9, 0.9),
    0,
    1
  );
  
  gl_PointSize = 5.0;
  InterpolatedColor = vec4(0, 0.6, 0.8, 1.0);
}
