#ifndef HITABLE_H
#define HITABLE_H

#include "aaab.h"
class material; 

struct hit_record {
	double t;
	vec3 p;
	vec3 normal;
	bool front_face;
	shared_ptr<material> mat_ptr;
	inline void set_face_normal(const ray& r, const vec3& outward_normal){
		front_face = dot(r.direction(), outward_normal) < 0;
		normal = front_face ? outward_normal : -outward_normal;
	}

};

class hitable {
	public:
		virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
		virtual bool bounding_box(double t0, double t1, aaab& output_box) const = 0;
};


#endif