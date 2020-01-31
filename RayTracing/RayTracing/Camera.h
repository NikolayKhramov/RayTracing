#pragma once


#include "Ray.h"

# define M_PI           3.14159265358979323846

class Camera {
public:
	Camera(Vec3 lookfrom, Vec3 lookat, Vec3 vup, float vfov, float aspect) {
		Vec3 w, u, v;
		float theta = vfov * M_PI / 180;	
		float half_height = tan(theta / 2);
		float half_width = aspect * half_height;
		origin = lookfrom;
		w = unit_vector(lookfrom - lookat);
		u = unit_vector(cross(vup, w));
		v = cross(w, u);
		lower_left_corner = origin - u * half_width - v * half_height - w;
		horizontal = 2 * half_width*u;
		vertical = 2 * half_height*v;
	}
	Ray get_ray(float u, float v) { return Ray(origin, lower_left_corner + u * horizontal + v * vertical - origin); }

	Vec3 origin;
	Vec3 lower_left_corner;
	Vec3 horizontal;
	Vec3 vertical;
};
