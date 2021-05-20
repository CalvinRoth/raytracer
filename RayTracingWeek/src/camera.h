#ifndef CAMERA_H
#define CAMERA_H

#include "ray.h"

class camera {
	public:
		camera(vec3 lookfrom, vec3 lookat, vec3 vup, double vfov, double aspect, double aperture, double focus_dist, double t0 = 0, double t1 = 0) {
			double theta, half_h, half_w;
			double view_height, view_width;
			lens_radius = aperture / 2;
			theta = vfov * M_PI / 180;
			half_h = tan(theta/2);
			view_height = 2.0*half_h;
			view_width = aspect*view_height;

			origin = lookfrom;
			w = unit_vector(lookfrom - lookat);
			u = unit_vector(cross(vup, w));
			v = cross(w, u);
			horizontal = focus_dist*view_width*u;
			vertical =   focus_dist*view_height*v;
			lower_left_corner = origin - horizontal/2 - vertical/2 - focus_dist*w;
			time0 = t0;
			time1 = t1;
		}
		camera() {}

		ray get_ray(double s, double t) const {
			vec3 rd = lens_radius*random_in_unit_disk();
			vec3 offset = u*rd.x()+ v*rd.y();
			return ray(origin + offset, 
					   lower_left_corner + s*horizontal + t*vertical - origin - offset,
					   Randomdouble(time0, time1));
		}
		vec3 u,v, w;
		vec3 origin;
		vec3 lower_left_corner;
		vec3 horizontal;
		vec3 vertical;
		double lens_radius;
		double time0;
		double time1;
};

#endif