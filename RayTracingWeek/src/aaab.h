#ifndef AABB_H
#define AABB_H
//==============================================================================================
// Originally written in 2016 by Peter Shirley <ptrshrl@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related and
// neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.
//
// You should have received a copy (see file COPYING.txt) of the CC0 Public Domain Dedication
// along with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
//==============================================================================================

#include "ray.h"

class aaab {
    public:
        aaab() {}
        aaab(const point3& a, const point3& b) { _min = a; _max = b; }

        point3 min() const {return _min; }
        point3 max() const {return _max; }

        bool hit(const ray& r, double tmin, double tmax) const {
            for (int a = 0; a < 3; a++) {
                auto t0 = fmin((_min[a] - r.origin()[a]) / r.direction()[a],
                               (_max[a] - r.origin()[a]) / r.direction()[a]);
                auto t1 = fmax((_min[a] - r.origin()[a]) / r.direction()[a],
                               (_max[a] - r.origin()[a]) / r.direction()[a]);
                tmin = fmax(t0, tmin);
                tmax = fmin(t1, tmax);
                if (tmax <= tmin)
                    return false;
            }
            return true;
        }

    public:
        point3 _min;
        point3 _max;
};

aaab surrounding_box(aaab box0, aaab box1) {
    vec3 small(fmin(box0.min().x(), box1.min().x()),
               fmin(box0.min().y(), box1.min().y()),
               fmin(box0.min().z(), box1.min().z()));

    vec3 big  (fmax(box0.max().x(), box1.max().x()),
               fmax(box0.max().y(), box1.max().y()),
               fmax(box0.max().z(), box1.max().z()));

    return aaab(small,big);
}

#endif
