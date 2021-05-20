#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

using std::sqrt;

double Randomdouble(){
	return  ((double) rand()) / ((double) RAND_MAX+1);
}

double Randomdouble(double min, double max){
	return  min + (max-min)*Randomdouble();
}

class vec3
{
public:
	vec3() : e{0,0,0} {}
	vec3(double e0, double e1, double e2) : e{e0,e1,e2} {}

	double x() const { return e[0]; }
	double y() const { return e[1];}
	double z() const { return e[2];}

	vec3 operator-() const {return vec3(-e[0],-e[1], -e[2]);}
	double operator[](int i) const { return e[i];}
	double& operator[](int i) { return e[i];}

	vec3& operator+= (const vec3 &v){
		e[0] += v[0];
		e[1] += v[1];
		e[2] += v[2];
		return *this;
	}

	vec3& operator*= (const double s){
		e[0] *= s;
		e[1] *= s;
		e[2] *= s;
		return *this;
	}

	vec3& operator/= (const double s){
		return *this *= 1/s;
	}

	double length() const {
		return sqrt(length_squared());
	}

	double length_squared() const {
		return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
	}

	inline static vec3 random() {
		vec3(Randomdouble(), Randomdouble(), Randomdouble());
	}

	inline static vec3 random(double min, double max){
		return vec3(Randomdouble(min,max), Randomdouble(min,max), Randomdouble(min,max));
	}

	double e[3];
};
 
using point3 = vec3;
using color = vec3;


inline std::ostream& operator<<(std::ostream &out, const vec3 &v){
	return out << v[0] << " " << v[1] << " " << v[2];
}

inline vec3 operator+(const vec3 &u, const vec3 &v){
	return vec3(u[0] + v[0], u[1] + v[1], u[2] + v[2]);
}

inline vec3 operator-(const vec3 &u, const vec3 &v){
	return vec3(u[0] - v[0], u[1] - v[1], u[2] - v[2]);
}

inline vec3 operator*(const vec3 &u, const vec3 &v){
	return vec3(u[0] * v[0], u[1] * v[1], u[2] * v[2]);
}

inline vec3 operator*(double t, const vec3 &u){
	return vec3(u[0] * t, u[1] * t, u[2] * t);
}

inline vec3 operator*(const vec3 &u, double t){
	return vec3(u[0] * t, u[1] * t, u[2] * t);
}

inline vec3 operator/(const vec3 &u, double t){
	return vec3(u[0] / t, u[1] / t, u[2] / t);
}

inline double dot(const vec3& u, const vec3& v){
	return u[0]*v[0] + u[1]*v[1] + u[2]*v[2];
}

inline vec3 cross(const vec3& u, const vec3 &v){
	return vec3(u[1]*v[2] - u[2]*v[1], u[2]*v[0] - u[0]*v[2], u[0]*v[1] - u[1]*v[0]);
}

inline vec3 unit_vector(vec3 v){
	return v / v.length();
}

vec3 reflect(const vec3& v, const vec3& n){
	return v - 2*dot(v,n)*n;
}

bool refract(const vec3& v, const vec3& n, double ratio, vec3& refracted){
	double cos_theta = dot(-v, n);
	if(cos_theta > 1) cos_theta = 1.0;
	if(cos_theta < -1) cos_theta = -1.0;
	vec3 r_para = ratio * (v + cos_theta*n);
	vec3 r_perp = -sqrt(1.0 - r_para.length_squared())*n;
	refracted = r_para + r_perp;
	return true;
}

double schlick(double cosine, double r_idx){
 	double r0 = (1 - r_idx) / (1 + r_idx);
 	r0 = r0 * r0;
 	return r0 + (1-r0)*pow((1-cosine), 5);
 }

vec3 random_in_unit_sphere(){
	vec3 p;
	double x = rand();
  	double r1, r2, r3;
	do {
		vec3 p = vec3::random(-1,1);
	} while (p.length_squared() >= 1.0);
	return p;
}

vec3 random_in_unit_disk(){
	while(true){
		auto p = vec3(Randomdouble(-1,1), Randomdouble(-1,1), 0);
		if(p.length_squared() >= 1) continue;
		return p;
	}
}

vec3 random_unit_vector(){
	auto a = Randomdouble(0, 2*M_PI);
	auto z = Randomdouble(-1,1);
	auto r = sqrt(1 - z*z);
	return vec3(r*cos(a), r*sin(a), z);
}

double clamp(double val, double fmin, double fmax){
	if(val < fmin) return fmin;
	if(val < fmax) return val;
	return fmax;
}

#endif