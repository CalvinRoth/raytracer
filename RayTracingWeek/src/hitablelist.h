#ifndef HITABLELIST_H
#define HITABLELIST_H

#include "hitable.h"
#include <vector>
class hitable_list : public hitable {
public:
	hitable_list() {}
	hitable_list(shared_ptr<hitable> object) {objects.push_back(object); }
	virtual bool hit(const ray& r, double tmin, double tmax, hit_record& rec) const;
	virtual bool bounding_box(double t0, double t1, aaab& output_box) const ;
	void clear() {objects.clear();}
	void add(shared_ptr<hitable> object) { objects.push_back(object);}
	std::vector<shared_ptr<hitable>> objects;
};

bool hitable_list::hit(const ray& r, double tmin, double tmax, hit_record& rec) const {
	hit_record temp_rec;
	bool hit_anything = false;
	double closest_so_far = tmax;
	for(const auto object : objects){
		if(object->hit(r, tmin,  closest_so_far, temp_rec)) {
			hit_anything = true;
			closest_so_far = temp_rec.t;
			rec = temp_rec;
		}
	}
	return hit_anything;
}

bool hitable_list::bounding_box(double t0, double t1, aaab& output_box) const {
	if (objects.size() == 0) return false;
	aaab temp_box;
	bool first_box = true;
	for(const auto object : objects){
		if(!object->bounding_box(t0,t1, temp_box)) return false;
		output_box = first_box ? temp_box : surrounding_box(output_box, temp_box);
		first_box = false;
	}
	return true;
}
#endif