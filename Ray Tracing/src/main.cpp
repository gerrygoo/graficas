#include<vector>
#include <stdlib.h>

#include "image_writer.h"

#include "sphere.h"
#include "hitable_list.h"
#include "float.h"
#include "camera.h"
#include "material.h"

float nearly256 = 255.99;

vec3 color(const ray& r, hitable *world, int depth) {
	hit_record rec;

	if (world->hit(r, 0.001, FLT_MAX, rec)) {
		ray scattered;
		vec3 attenuation;

		if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered) ) {
			return attenuation * color(scattered, world, depth + 1);
		} else {
			return vec3(0, 0, 0);
		}
	} else {
		vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5 * (unit_direction.y() + 1.0);
		return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
	}
}

int main() {
	int nx, ny, ns;
	bool cheap = true;

	if (cheap) {
		nx = 200;
		ny = 100;
		ns = 30;
	} else {
		nx = 200 * 4;
		ny = 100 * 4;
		ns = 20 * 5;
	}
	
	
	std::vector<unsigned char> pixels;

	camera cam(90, float(nx)/float(ny));
	hitable* list[2];
	
	float R = cos(M_PI / 4);
	list[0] = new sphere(vec3(-R, 0, -1), R, new lambertian(vec3(0, 0, 1)));
	list[1] = new sphere(vec3(R, 0, -1), R, new lambertian(vec3(1, 0, 0)));

	hitable* world = new hitable_list(list, 2);

	for (int j = ny - 1; j >= 0; j--) {
		for (int i = 0; i < nx; i++) {
			// ------
			vec3 col(0, 0, 0);
			for (int s = 0; s < ns; s++) {
			
				float u = float(i + drand48()) / float(nx);
				float v = float(j + drand48()) / float(ny);
				ray r = cam.get_ray(u, v);

				//vec3 p = r.point_at_parameter(2.0);
				col += color(r, world, 0);
			}
			col /= float(ns);

			pixels.push_back(int(nearly256 * sqrt(col[0])));
			pixels.push_back(int(nearly256 * sqrt(col[1])));
			pixels.push_back(int(nearly256 * sqrt(col[2])));
			// ------
		}
	}

	image_writer::save_jpg("out.png", nx, ny, 3, pixels.data());

	return 0;
}