#include<vector>
#include <thread>
#include <stdlib.h>

#include "image_writer.h"

#include "sphere.h"
#include "hitable_list.h"
#include "float.h"
#include "camera.h"
#include "material.h"

float nearly256 = 255.99;

int 
nx = 200,
ny = 100,
ns = 20;

unsigned char pixels[200 * 100 * 3];
camera cam(vec3(13, 2, 3), vec3(0, 0, 0), vec3(0, 1, 0), 30, float(200) / float(100), 0.1, 10);

hitable *random_scene() {

	int n = 100;

	hitable **list = new hitable*[n + 5];
	list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(vec3(0.5, 0.5, 0.5)));
	list[1] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
	list[2] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian(vec3(0.4, 0.2, 0.1)));
	list[3] = new sphere(vec3(4, 1, 0), 1.0, new dielectric(1.5));
	list[4] = new sphere(vec3(8, 1, 0), 1.0, new dielectric(1.5));
	

	int i = 5;
	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {

			float choose_mat = drand48();
			vec3 center(a + 0.9*drand48(), 0.2, b + 0.9*drand48());
			if ( (center - vec3(4, 0.2, 0)).length() > 0.9 ) {
				
				if (choose_mat < 0.8) {
					list[i++] = new sphere(center, 0.2, new lambertian(vec3(drand48()*drand48(), drand48()*drand48(), drand48()*drand48())));
				}
				else if (choose_mat < 0.95) {
					list[i++] = new sphere(center, 0.2, new metal(vec3(0.5*(1 + drand48()), 0.5*(1 + drand48()), 0.5*(1 + drand48())), 0.5*drand48()));
				}
				else {
					list[i++] = new sphere(center, 0.2, new dielectric(1.5));
				}
			}
			if (i > n - 6) break;
		}

		if (i > n - 6) break;
	}

	return new hitable_list(list, i);

}
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

void color_pixel(int c, camera& cam, hitable* world, int i, int j) {
	vec3 col(0, 0, 0);
	for (int s = 0; s < ns; s++) {

		float u = float(i + drand48()) / float(nx);
		float v = float(j + drand48()) / float(ny);
		ray r = cam.get_ray(u, v);

		col += color(r, world, 0);
	}
	col /= float(ns);

	pixels[c] = int(nearly256 * sqrt(col[0]));
	pixels[c + 1] = int(nearly256 * sqrt(col[1]));
	pixels[c + 2] = int(nearly256 * sqrt(col[2]));
}

int main() {
	
	std::vector<std::thread> threads;

	hitable* world = random_scene();

	int c = 0;
	for (int j = ny - 1; j >= 0; j--) {
		for (int i = 0; i < nx; i++) {
			//color_pixel(c, cam, world, i, j);
			/*threads.push_back(std::thread(
				[&]() {
				color_pixel(pixels, c, cam, world, i, j);
				}
			));*/


			threads.push_back( std::thread(color_pixel, c, std::ref(cam), world, i, j) );
			
			
			c += 3;
		}
	}

	for (auto &t : threads) t.join();

	image_writer::save_jpg("out.png", nx, ny, 3, pixels);

	return 0;
}