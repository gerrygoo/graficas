#pragma once
#include "ray.h"

class camera {
	public:
		camera(float vfov, float aspect) {
			float theta = vfov * M_PI / 180;
			float half_height = tan(theta/2);
			float half_width = aspect * half_height;
			lower_left_corner = vec3(-half_width, -half_height, -1.0);
		}
		ray get_ray(float u, float v) {
			return ray(origin, lower_left_corner + u * horizontal + v * vertical - origin);
		}

		vec3 origin;
		vec3 lower_left_corner;
		vec3 horizontal;
		vec3 vertical;
};