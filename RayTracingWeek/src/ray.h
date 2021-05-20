#ifndef RAY_H
#define RAY_H

#include "color.h"

class ray{
	public:
		ray () {}
		ray(const vec3& a, const vec3& b, double time = 0.0) { A = a; B = b; tm = time;}
		vec3 origin() const {return A;}
		vec3 direction() const { return B; }
		double time() const {return tm;}
		vec3 point_at_parameter(double t) const {return A + t*B;}

		vec3 A;
		vec3 B;
		double  tm;
};
#endif