#ifndef SPHERE_H
#define SPHERE_H

#include "hitable.h"

class sphere : public hitable {
public:
	sphere() {}
	sphere(vec3 cen, double r) : center(cen), radius(r) {};
	sphere(vec3 cen, double r, shared_ptr<material> m) : center(cen), radius(r), mat_ptr(m) {};
	virtual bool hit(const ray& r, double tmin, double tmax, hit_record& rec) const;
	virtual bool bounding_box(double t0, double t1, aaab& output_box) const;
	vec3 center;
	double radius;
	shared_ptr<material> mat_ptr;
};

bool sphere::hit(const ray& r, double tmin, double tmax, hit_record& rec) const {
	vec3 oc = r.origin() - center;
	double a = dot(r.direction(), r.direction());
	double b = dot(oc, r.direction());
	double c = dot(oc, oc) - radius*radius;
	double discrim = b*b - a*c;
	if(discrim > 0) {
		double temp = (-b - sqrt(discrim))/a;
		if(temp < tmax && temp > tmin){
			rec.t = temp;
			rec.mat_ptr = mat_ptr;
			rec.p = r.point_at_parameter(rec.t);
			vec3 onormal = (rec.p - center) / radius;
			rec.set_face_normal(r, onormal);
			return true;
		}
		temp = (-b + sqrt(discrim))/a;
		if(temp < tmax && temp > tmin){
			rec.t = temp;
			rec.mat_ptr = mat_ptr;
			rec.p = r.point_at_parameter(rec.t);
			vec3 onormal = (rec.p - center) / radius;
			rec.set_face_normal(r, onormal);
			return true;
		}
	}
	return false;
}

bool sphere::bounding_box(double t0, double t1, aaab& output_box) const {
	output_box = aaab(
		center - vec3(radius, radius, radius),
		center + vec3(radius, radius, radius)
	);
	return true;
}
#endif