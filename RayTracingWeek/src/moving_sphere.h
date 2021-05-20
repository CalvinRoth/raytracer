#ifndef MOVINGSPHERE_H
#define MOVINGSPHERE_H
#include "hitable.h"

class moving_sphere : public hitable {
public:
	point3 center0, center1;
	double time0, time1;
	double radius;
	shared_ptr<material> mat_ptr;
	moving_sphere();
	moving_sphere(point3 cen0, point3 cen1, double t0, double t1, double r, shared_ptr<material> m):
		center0(cen0), center1(cen1), time0(t0), time1(t1), radius(r), mat_ptr(m) {};

	virtual bool hit(const ray& r, double tmin, double tmax, hit_record& rec) const;
	virtual bool bounding_box(double t0, double t1, aaab& output_box) const;
	vec3 center(double time) const;

};

point3 moving_sphere::center(double time) const{
	return center0 + ((time - time0) / (time1 - time0))*(center1 - center0);
}


bool moving_sphere::hit(const ray& r, double tmin, double tmax, hit_record& rec) const {
	vec3 oc = r.origin() - center(r.time());
	double a = dot(r.direction(), r.direction());
	double b = dot(oc, r.direction());
	double c = dot(oc, oc) - radius*radius;
	double discrim = b*b - a*c;
	if(discrim > 0) {
		rec.mat_ptr = mat_ptr;
		double temp = (-b - sqrt(discrim))/a;
		if(temp < tmax && temp > tmin){
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			vec3 onormal = (rec.p - center(r.time())) / radius;
			rec.set_face_normal(r, onormal);
			return true;
		}
		temp = (-b + sqrt(discrim))/a;
		if(temp < tmax && temp > tmin){
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			vec3 onormal = (rec.p - center(r.time())) / radius;
			rec.set_face_normal(r, onormal);
			return true;
		}
	}
	return false;
}

bool moving_sphere::bounding_box(double t0, double t1, aaab& output_box) const {
	aaab box0 (
		center(t0) - vec3(radius, radius, radius),
		center(t0) + vec3(radius, radius,radius)
	);
	aaab box1(		
		center(t1) - vec3(radius, radius, radius),
		center(t1) + vec3(radius, radius,radius)
	);
	output_box = surrounding_box(box0, box1);
	return true;
}
#endif