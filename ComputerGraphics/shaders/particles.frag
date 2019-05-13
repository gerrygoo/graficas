#version 410

in vec3 interpolated_color;
in vec2 tex_coord;

uniform sampler2D drop_texture;

layout ( location = 0 ) out vec4 FragColor;

void main(){
	// FragColor = vec4(0.2f, 0.5f, 0.9f, 1.0f);
	// FragColor = mix(texture(lucina, coord), texture(metal, coord), 0.5f);
	// FragColor = vec4(interpolated_color, 1.0f);
	// FragColor =  vec4(texture(drop_texture, tex_coord).xyz, 0.5f);
	FragColor = normalize(mix(texture(drop_texture, tex_coord), vec4(interpolated_color, 1.0f), 0.8f));
	// vec3 texture_color = interpolated_color;
	// vec3 ambient = interpolated_color * 0.1;
	// vec3 phong = (ambient) *  texture_color;
	// FragColor = vec4(phong, 1.0f);

}