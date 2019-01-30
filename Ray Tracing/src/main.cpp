#include<vector>
#include <stdlib.h>

#include "image_writer.h"

#include "sphere.h"
#include "hitable_list.h"
#include "float.h"
#include "camera.h"

float nearly256 = 255.99;
float rand_max = 32767.0f + 1.0f;

//int main() {
//	int nx = 400 * 2, 
//		ny = 500 * 2;
//
//	float cheat = 255.99f;
//	std::vector<unsigned char> pixels;
//
//	for (int j = 0; j < ny; j++) {
//		for (int i = 0; i < nx; i++) {
//			vec3 color(float(i) / float(nx), float(j) / float(ny), 0.2f);
//
//
//			pixels.push_back(int(nearly256 * color[0]));
//			pixels.push_back(int(nearly256 * color[1]));
//			pixels.push_back(int(nearly256 * color[2]));
//		}
//	}
//
//	image_writer::save_jpg("out.png", nx, ny, 3, pixels.data());
//	return 0;
//}


//float hit_sphere(const vec3& sphere_center, float radius, const ray& r) {
//	vec3 sphere_center_to_origin = r.origin() - sphere_center; // origin to sphere
//
//	float a = dot(r.direction(), r.direction());
//	float b = 2.0 * dot(sphere_center_to_origin, r.direction());
//	float c = dot(sphere_center_to_origin, sphere_center_to_origin) - radius * radius;
//
//	float discriminant = b * b - 4 * a * c;
//
//	return discriminant < 0 ? -1.0 : (-b - sqrt(discriminant) / (2.0 * a));
//}
//
//vec3 color(const ray& r) {
//	float t = hit_sphere(vec3(0, 0, -1), 0.5, r);
//
//	if (t > 0.0) {
//		vec3 normal = unit_vector(r.point_at_parameter(t) - vec3(0, 0, -1));
//		return 0.5 * vec3(normal.x() + 1, normal.y() + 1, normal.z() + 1);
//	}
//
//	// background gradient
//	vec3 unit_direction = unit_vector(r.direction());
//	t = 0.5 * (unit_direction.y() + 1.0);
//	return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
//}
//
//
//int main() {
//	int nx = 200, ny = 100;
//
//	std::vector<unsigned char> pixels;
//
//	// screen & origin in space
//	vec3 lower_left_corner(-2.0, -1.0, -1.0);
//	vec3 horizontal(4.0, 0.0, 0.0);
//	vec3 vertical(0.0, 2.0, 0.0);
//
//	vec3 origin(0.0, 0.0, 0.0);
//	
//	for (int j = ny - 1; j >= 0; j--) {
//		for (int i = 0; i < nx; i++) {
//			float u = float(i) / float(nx);
//			float v = float(j) / float(ny);
//
//			ray r(origin, lower_left_corner + u * horizontal + v * vertical);
//
//			vec3 col = color(r);
//
//			pixels.push_back(int(nearly256 * col[0]));
//			pixels.push_back(int(nearly256 * col[1]));
//			pixels.push_back(int(nearly256 * col[2]));
//
//		}
//	}
//
//	image_writer::save_jpg("out.png", nx, ny, 3, pixels.data());
//	
//	return 0;
//}

vec3 color(const ray& r, hitable *world) {
	hit_record rec;

	if (world->hit(r, 0.0, FLT_MAX, rec)) {
		return 0.5 * vec3(rec.normal.x() + 1, rec.normal.y() + 1 , rec.normal.z() + 1);
	} else {
		vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5 * (unit_direction.y() + 1.0);
		return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
	}
}

int main() {
	int nx = 200 * 2, ny = 100 * 2;
	int ns = 100;
	std::vector<unsigned char> pixels;

	hitable* list[2];

	list[0] = new sphere(vec3(0, 0, -1), 0.5);
	list[1] = new sphere(vec3(0, -100.5, -1), 100);

	hitable* world = new hitable_list(list, 2);
	camera cam;

	for (int j = ny - 1; j >= 0; j--) {
		for (int i = 0; i < nx; i++) {
			vec3 col(0, 0, 0);
			for (int s = 0; s < ns; s++) {
				float u = (float(i) + float(rand()) / rand_max) / float(nx);
				float v = (float(j) + float(rand()) / rand_max) / float(ny);
				ray r = cam.get_ray(u, v);
				//vec3 p = r.point_at_parameter(2.0);
				col += color(r, world);
			}
			col /= float(ns);

			pixels.push_back(int(nearly256 * col[0]));
			pixels.push_back(int(nearly256 * col[1]));
			pixels.push_back(int(nearly256 * col[2]));
		}
	}

	image_writer::save_jpg("out.png", nx, ny, 3, pixels.data());

	return 0;
}