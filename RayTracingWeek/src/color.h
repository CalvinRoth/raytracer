#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"

#include <iostream>

void write_color(std::ostream &out,  color c, int samples){
	double scale = 1.0/samples;
	double r = c.x();
	double g = c.y();
	double b = c.z();
	if ( r != r) r = 0.0;
	if ( g != g) g = 0.0;
	if ( b != b) b = 0.0;
	r = sqrt(scale * r);
	g = sqrt(scale * g);
	b = sqrt(scale * b);
	out << static_cast<int>(256*clamp(r, 0.0, 0.99999)) << " "
		<< static_cast<int>(256*clamp(g, 0.0, 0.99999))  << " "
		<< static_cast<int>(256*clamp(b, 0.0, 0.99999))  << '\n';
}

void write_colors(std::ostream &out, int a, int b, color** c, int samples){
	for(int j = a -1; j >= 0; --j){
		//std::cerr << "\r Scanlines ramining: " << j << " " << std::flush;
		for(int i = 0; i < b; ++i){
			write_color(out, c[j][i], samples);
		}
	}
}
#endif