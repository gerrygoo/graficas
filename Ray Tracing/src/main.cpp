#include<vector>
#include <thread>
#include <stdlib.h>

#include "image_writer.h"

#include "sphere.h"
#include "hitable_list.h"
#include "float.h"
#include "camera.h"
#include "material.h"

#define verbose true

#define nx 1920
#define ny 1080
#define ns 100
#define spheries 600
#define cores 8

float nearly256 = 255.99;
unsigned int chunk_pixel_size = nx * ny / cores;

unsigned char pixels[nx * ny * 3 + 100];
std::thread threads[cores];

camera cam(vec3(13, 2, 3), vec3(0, 0, 0), vec3(0, 1, 0), 30, float(nx) / float(ny), 0.05, 10);
hitable* g_world;

hitable *random_scene() {

	int n = spheries;

	hitable **list = new hitable*[n + 5];
	list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(vec3(0.5, 0.5, 0.5)));
	list[1] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
	list[2] = new sphere(vec3(-4, 1, 0), 1.0, new metal(vec3(1, 1, 1), 0.0));
	list[3] = new sphere(vec3(4, 1, 0), 1.0, new dielectric(1.5));
	list[4] = new sphere(vec3(7, 1, 0), 1.0, new dielectric(1.5));
	

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

void color_pixel(int c, int i, int j) {
	vec3 col(0, 0, 0);
	for (int s = 0; s < ns; s++) {

		float u = float(i + drand48()) / float(nx);
		float v = float(j + drand48()) / float(ny);
		ray r = cam.get_ray(u, v);

		col += color(r, g_world, 0);
	}
	col /= float(ns);

	pixels[c] = int(nearly256 * sqrt(col[0]));
	pixels[c + 1] = int(nearly256 * sqrt(col[1]));
	pixels[c + 2] = int(nearly256 * sqrt(col[2]));
}

void color_chunk(int chunk_no) {
	unsigned int initial_pixel = chunk_no * chunk_pixel_size;
	unsigned int current_pixel = initial_pixel;

	unsigned int progress = 0, decile = 0;

	for (int ii = 0; ii < chunk_pixel_size; ii++) {
		color_pixel( current_pixel++ * 3, current_pixel % nx, ny - 1 - current_pixel / nx );

		if (++progress > chunk_pixel_size / 10) {
			if (verbose) std::cout << chunk_no << "-" << ++decile << "0%\n";
			progress = 0;
		}
	}
}

int main() {

	g_world = random_scene();

	for (int i = 0; i < cores; i++ ) threads[i] = std::thread(color_chunk, i);
	for (auto &t : threads) t.join();

	image_writer::save_jpg("out.png", nx, ny, 3, pixels);

	return 0;
}