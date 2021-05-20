ray_trace : src/main.cc build/ray.o build/bvh.o build/sphere.o build/camera.o build/material.o build/moving_sphere.o
	g++ -fopenmp -lpthread src/main.cc -o ray_trace

build/camera.o : src/camera.h build/ray.o
	g++ -c src/camera.h -o build/camera.o

build/aaab.o : src/aaab.h build/ray.o
	g++ -c src/aaab.h -o build/aaab.o

build/bvh.o : src/bvh.h build/hitablelist.o
	g++ -c src/bvh.h -o build/bvh.o

build/moving_sphere.o : src/moving_sphere.h build/hitable.o
	g++ -c src/moving_sphere.h -o build/moving_sphere.o

build/sphere.o : src/sphere.h build/hitable.o 
	g++ -c src/sphere.h -o build/sphere.o

build/hitablelist.o : src/hitablelist.h build/hitable.o
	g++ -c src/hitablelist.h -o build/hitablelist.o

build/hitable.o : src/hitable.h build/aaab.o
	g++ -c src/hitable.h -o build/hitable.o
	
build/material.o : src/material.h build/hitable.o
	g++ -c src/material.h -o build/material.o

build/ray.o : src/ray.h build/color.o
	g++ -c src/ray.h -o build/ray.o

build/color.o : src/color.h build/vec3.o
	g++ -c src/color.h -o build/color.o

build/vec3.o : src/vec3.h
	g++ -c src/vec3.h -o build/vec3.o

clean:
	rm build/*.o ray_trace image/*.ppm

clean_images:
	rm image/*.ppm