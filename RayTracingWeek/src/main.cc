#include "sphere.h"
#include "moving_sphere.h"
#include "bvh.h"
#include "camera.h"
#include "float.h"
#include "material.h"
#include <pthread.h>
#include <random>
#include <omp.h>
#include <iostream>


color ray_color(const ray& r, const hitable& world, int depth){
	hit_record rec;
	if( depth <= 0 ){
		return vec3(0,0,0);
	}
	if(!world.hit(r, 0.001, DBL_MAX, rec)){
		vec3 unit_direction = unit_vector(r.direction());
		double t = 0.5 * (unit_direction.y() + 1.0);
		return (1.0 - t)*color(1.0,1.0,1.0) + t*color(0.5, 0.7, 1.0);
	}
	ray scattered;
	vec3 attenuation;	
	bool flag = rec.mat_ptr->scatter(r, rec, attenuation, scattered);
	if (flag){
		return attenuation*ray_color(scattered, world, depth-1);
	}
	return vec3(0,0,0);
}

hitable_list random_scene(){
	hitable_list world;
	world.add(make_shared<sphere>(vec3(0,-1000.0, 0), 1000, make_shared<lambertian>(vec3(0.5,0.5,0.5))));
	for(int a = -11; a < 11; a++){
		for(int b = -11; b < 11; b++) {
			double choose_mat = Randomdouble();
			vec3 center(a+0.9*Randomdouble(), 0.2, b+0.9*Randomdouble());
			if((center - vec3(4,0.2 ,0)).length() > 0.9){
				if(choose_mat < 0.33){
					vec3 center2 = center + vec3(0, Randomdouble(0, .5), 0);
					world.add(make_shared<sphere>(center, 0.2, make_shared<lambertian>(vec3(Randomdouble()*Randomdouble(),
																			Randomdouble()*Randomdouble(),
																			Randomdouble()*Randomdouble()))));
				} else if(choose_mat < 0.66) {
					world.add(make_shared<sphere>(center, 0.2, make_shared<metal>(vec3(0.5*(1+ Randomdouble()), 
					  												   0.5*(1+ Randomdouble()),
					  												   0.5*(1+ Randomdouble())))));
				} else {
					world.add(make_shared<sphere>(center, 0.2, make_shared<dielectric>(1.5)));
				}
			}
		}
	}
	world.add(make_shared<sphere>(vec3(0,1,0), 1.0, make_shared<dielectric>(1.5)));
	world.add(make_shared<sphere>(vec3(-4, 1,0), 1.0, make_shared<lambertian>(vec3(0.4, 0.2, 0.1))));
	world.add(make_shared<sphere>(vec3(4,1,0), 1.0, make_shared<metal>(vec3(0.7,0.6,0.5), 0.0)));
	//return hitable_list(make_shared<hitable_list>(world));
	return hitable_list(make_shared<bvh_node>(world, 0.0, 0.0));
}

// struct params {
// 	int i;
// 	int j;
// 	int sn;
// 	int height;
// 	int width;
// 	color* output;
// 	const hitable* world;
// 	camera cam;
// };

// void* loop(void* params){
// 	struct params* p = (struct params*) params;
// 	color pixel_color;
// 	double u,v;
// 	ray r;
// 	camera cam = p->cam;
// 	for(int s = 0; s < p->sn; s++){
// 		u = (p->i + Randomdouble()) / (p->width - 1);
// 		v = (p->j + Randomdouble()) / (p->height - 1);
// 		r = cam.get_ray(u,v);
// 		pixel_color += ray_color(r, *(p->world), 10);
// 	}
// 	p->output[0] = pixel_color;
// 	return NULL;
// }

int main(int argc, char const *argv[])
{	
	const double aspect_ratio = 16.0 / 9.0;
	const int iw = 1200;
	const int ih = static_cast<int> (iw/aspect_ratio);
	const int is = 50;

	std::cout << "P3\n" << iw << " " << ih << "\n255\n";

	auto world = random_scene();
	point3 lookfrom(13,2,3);
	point3 lookat(0,0,0);
	vec3 vup(0,1,0);
	double dist_to_focus = 10.0;
	double aperture = 0.1;
	color **output = new color*[ih];
	for(int i = 0; i < ih; i++){
		output[i] = new color[iw];
	}
	camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);
	color pixel_color;
	double u,v;
	ray r;
	int i,j, s;
	#pragma omp parallel for num_threads(1024), shared(output, world) private(u, v, r,i,j,s) collapse(2)
	//__global__
	for(j = ih -1; j >= 0; --j){
		for(i = 0; i < iw; ++i){

			color pixel_color;
			for(s = 0; s < is; ++s){
				u = (i + Randomdouble()) / (iw - 1);
				v = (j + Randomdouble()) / (ih - 1);
				r = cam.get_ray(u,v);
				pixel_color += ray_color(r, world, 100);
			}
			//write_color(std::cout, pixel_color, is);
			output[j][i] = pixel_color;
		}
	}
	write_colors(std::cout, ih, iw, (color **) output, is);
	std::cerr << "\nDone\n";
	return 0;

}