#ifndef MATERIAL_H
#define MATERIAL_H
#include "hitable.h"

class material {
 public:
     virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const = 0;
};

class lambertian : public material {
public:
	lambertian(const color& a): albedo(a) {}
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const {
		vec3 target = rec.normal + random_unit_vector();
		scattered = ray(rec.p, target, r_in.time());
		attenuation = albedo;
		return true;
	}
	color albedo;
};

class metal : public material {
 public:
     metal(const color& a, double f = 0) : albedo(a) {if (f < 1) fuzz = f; else fuzz = 1;}
     double fuzz;
     virtual bool scatter(
     	const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
     ) const {
     	vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
     	scattered = ray(rec.p, reflected+ fuzz*random_in_unit_sphere(), r_in.time());
     	attenuation = albedo;
     	return (dot (scattered.direction(), rec.normal) > 0);
     }
     color albedo;
};

class dielectric : public material {
    public:
        dielectric(double ri) : ref_idx(ri) {}

        virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
        ) const {
            attenuation = color(1.0, 1.0, 1.0);
            double etai_over_etat = (rec.front_face) ? (1.0 / ref_idx) : (ref_idx);

            vec3 unit_direction = unit_vector(r_in.direction());
            double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
            double sin_theta = sqrt(1.0 - cos_theta*cos_theta);
            if (etai_over_etat * sin_theta > 1.0 ) {
                vec3 reflected = reflect(unit_direction, rec.normal);
                scattered = ray(rec.p, reflected, r_in.time());
                return true;
            }

            double reflect_prob = schlick(cos_theta, etai_over_etat);
            if (Randomdouble() < reflect_prob)
            {
                vec3 reflected = reflect(unit_direction, rec.normal);
                scattered = ray(rec.p, reflected, r_in.time());
                return true;
            }

            vec3 refracted;
            refract(unit_direction, rec.normal, etai_over_etat,refracted);
            scattered = ray(rec.p, refracted, r_in.time());
            return true;
        }

    public:
        double ref_idx;
};
#endif