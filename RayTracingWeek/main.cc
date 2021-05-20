#include "sphere.h"
#include "hitablelist.h"
#include "camera.h"
#include "float.h"
#include "material.h"
#include <random>
#include <iostream>


color ray_color(const ray& r, hitable *world, int depth){
	hit_record rec;
	if(world->hit(r, 0.001, DBL_MAX, rec)){
		ray scattered;
		vec3 attenuation;	

		bool flag = rec.mat_ptr->scatter(r, rec, attenuation, scattered);
		if (depth < 50 && flag){
			return attenuation*ray_color(scattered, world, depth+1);
		}
		return vec3(0,0,0);
	}

	vec3 unit_direction = unit_vector(r.direction());
	double t = 0.5 * (unit_direction.y() + 1.0);
	return (1.0 - t)*color(1.0,1.0,1.0) + t*color(0.5, 0.7, 1.0);
}

hitable *random_scene(){
	int n = 500;
	hitable **list = new hitable*[n+1];
	list[0] = new sphere(vec3(0,-1000.0, 0), 1000, new lambertian(vec3(0.5,0.5,0.5)));
	int i = 1;
	for(int a = -11; a < 11; a++){
		for(int b = -11; b < 11; b++) {
			double choose_mat = Randomdouble();
			vec3 center(a+0.9*Randomdouble(), 0.2, b+0.9*Randomdouble());
			if((center - vec3(4,0.2 ,0)).length() > 0.9){
				if(choose_mat < 0.8){
					list[i++] = new sphere(center, 0.2, new lambertian(vec3(Randomdouble()*Randomdouble(),
																			Randomdouble()*Randomdouble(),
																			Randomdouble()*Randomdouble())));
				} else if(choose_mat < 0.95) {
					list[i++] = new sphere(center, 0.2, new metal(vec3(0.5*(1+ Randomdouble()), 
					  												   0.5*(1+ Randomdouble()),
					  												   0.5*(1+ Randomdouble()))));
				} else {
					list[i++] = new sphere(center, 0.2, new dielectric(1.5));
				}
			}
		}
	}
	list[i++] = new sphere(vec3(0,1,0), 1.0, new dielectric(1.5));
	list[i++] = new sphere(vec3(-4, 1,0), 1.0, new lambertian(vec3(0.4, 0.2, 0.1)));
	list[i++] = new sphere(vec3(4,1,0), 1.0, new metal(vec3(0.7,0.6,0.5), 0.0));
	return new hitable_list(list, i);
}

int main(int argc, char const *argv[])
{	
	const double aspect_ratio = 16.0 / 9.1;
	const int iw = 1200;
	const int ih = static_cast<int> (iw/aspect_ratio);
	const int is = 10;

	std::cout << "P3\n" << iw << " " << ih << "\n255\n";

	auto world = random_scene();
	point3 lookfrom(13,2,3);
	point3 lookat(0,0,0);
	vec3 vup(0,1,0);
	double dist_to_foucs = 10.0;
	double aperture = 0.1;
	color **output = new color*[ih];
	for(int i = 0; i < ih; i++){
		output[i] = new color[iw];
	}
	camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_foucs);
	//color pixel_color;
	double u,v;
	ray r;
	#pragma omp parallel for num_threads(16), shared(output, world) private(u, v, r) collapse(2)
	//__global__
	for(int j = ih -1; j >= 0; --j){
		//std::cerr << "\r Scanlines ramining: " << j << " " << std::flush;
		for(int i = 0; i < iw; ++i){
			color pixel_color;
			for(int s = 0; s < is; ++s){
				u = (i + Randomdouble()) / (iw - 1);
				v = (j + Randomdouble()) / (ih - 1);
				r = cam.get_ray(u,v);
				pixel_color += ray_color(r, world, 0);
			}
			//write_color(std::cout, pixel_color, is);
			output[j][i] = pixel_color;
		}
	}
	write_colors(std::cout, ih, iw, (color **) output, is);
	std::cerr << "\nDone\n";
	return 0;

}