#pragma once




#include "Hitable.h"

class Sphere : public Hitable {
public:
	Sphere();
	Sphere(Vec3 cen, float r, Material *mat) : center(cen), radius(r), mat_ptr(mat) {};
	virtual bool hit(const Ray& r, float tmin, float tmax, hit_record& rec) const;
	Vec3 center;
	float radius;
	Material *mat_ptr;
};

bool Sphere::hit(const Ray& r, float tmin, float tmax, hit_record& rec) const {
	Vec3 oc = r.origin() - center;	
	float a = dot(r.direction(), r.direction());
	float b = dot(r.direction(), oc);
	float c = dot(oc, oc) - radius * radius;
	float discriminant = b * b - a * c;
	if (discriminant > 0) {
		float temp = (-b - sqrt(b*b - a * c)) / a;
		if (temp < tmax && temp > tmin) {
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - center) / radius;	
			rec.mat_ptr = mat_ptr;
			return true;
		}
		temp = (-b - sqrt(b*b - a * c)) / a;
		if (temp < tmax && temp > tmin) {
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - center) / radius;		
			rec.mat_ptr = mat_ptr;
			return true;
		}
	}
	return false;
}
