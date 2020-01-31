#include <iostream>
#include <fstream>
#include <random>
#include "Ray.h"
#include "HitableList.h"
#include "Sphere.h"
#include "float.h"
#include "Camera.h"
#include "Material.h"
using namespace std;



Hitable *random_scene() {
	int n = 500;
	Hitable **list = new Hitable*[n + 1];
	list[0] = new Sphere(Vec3(0, -1000, 0), 1000, new Lambertian(Vec3(0.5, 0.5, 0.5)));

	int i = 1;
	for (int a = -11; a < 11; a++) {
		for (int b = -5; b < 5; b++) {
			float choose_mat = (double)rand() / double(RAND_MAX);

			Vec3 center(a + 0.9*((double)rand() / double(RAND_MAX)), 0.2, b + 0.9*((double)rand() / double(RAND_MAX)));
			if ((center - Vec3(4, 0.2, 0)).length() > 0.9) {	
				if (choose_mat < 0.8) {	
					list[i++] = new Sphere(center, 0.2, new Lambertian(Vec3(((double)rand() / double(RAND_MAX))*((double)rand() / double(RAND_MAX)), ((double)rand() / double(RAND_MAX))*((double)rand() / double(RAND_MAX)), ((double)rand() / double(RAND_MAX))*((double)rand() / double(RAND_MAX)))));
				}
				else if (choose_mat < 0.95) {
					list[i++] = new Sphere(center, 0.2, new Metal(Vec3(0.5*(1 + (double)rand() / double(RAND_MAX)), 0.5*(1 + (double)rand() / double(RAND_MAX)), 0.5*(1 + (double)rand() / double(RAND_MAX))), (double)rand() / double(RAND_MAX)));
				}
				else {		// glass
					list[i++] = new Sphere(center, 0.2, new Dielectric(1.5));
				}
			}
		}
	}

	list[i++] = new Sphere(Vec3(0, 1, 0), 1, new Dielectric(1.5));
	list[i++] = new Sphere(Vec3(-4, 1, 0), 1, new Lambertian(Vec3(0.4, 0.2, 0.1)));
	list[i++] = new Sphere(Vec3(4, 1, 0), 1, new Metal(Vec3(0.7,0.6,0.5),0.0));

	return new HitableList(list, i);
}

float hit_sphere(const Vec3& center, float radious, const Ray& r) {
	Vec3 oc = r.origin() - center;
	float a = dot(r.direction(), r.direction());
	float b = 2.0 * dot(oc, r.direction());
	float c = dot(oc, oc) - radious * radious;
	float discriminant = b * b - 4 * a*c;
	if (discriminant < 0) {
		return -1.0;
	}
	else {
		return (-b - sqrt(discriminant)) / (2.0*a);
	}
}

Vec3 color(const Ray& r, Hitable *world, int depth) {
	hit_record rec;
	if (world->hit(r, 0.0001, FLT_MAX, rec)) {
		Ray scattered;
		Vec3 attenuation;

		if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
			return attenuation * color(scattered, world, depth + 1);
		}
		else {
			return Vec3(0, 0, 0);
		}
	}
	else {
		Vec3 unit_direction = unit_vector(r.direction());		
		float t = 0.5 * (unit_direction.y() + 1.0);	
		return (1.0 - t)*Vec3(0.1, 0.7, 1.0) + t * Vec3(1, 1, 1);
	}
}

int main() {

	const int nx = 400;
	const int ny = 200;
	const int ns = 50;	
	
	ofstream myfile;
	myfile.open("view.ppm");
	myfile << "P3\n" << nx << " " << ny << "\n255\n";
	
	Hitable *world = random_scene();

	Camera cam(Vec3(9, 2, 3), Vec3(-1, 0, 0), Vec3(0, 1, 0), 30, float(nx) / float(ny));

	for (int j = ny - 1; j >= 0; j--) {
		for (int i = 0; i < nx; i++) {

			Vec3 col(0, 0, 0);
			for (int s = 0; s < ns; s++) {

				float u = float(i + rand() / double(RAND_MAX)) / float(nx);
				float v = float(j + rand() / double(RAND_MAX)) / float(ny);
				Ray r = cam.get_ray(u, v);
				Vec3 p = r.point_at_parameter(2.0);

				col += color(r, world, 0);
			}
			col /= float(ns);
			col = Vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
			int ir = int(255.99* col[0]);
			int ig = int(255.99* col[1]);
			int ib = int(255.99* col[2]);

			myfile << ir << " " << ig << " " << ib << "\n";
		}
	}
	myfile.close();

	system("pause");
	return 0;
}