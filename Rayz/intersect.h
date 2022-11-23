#pragma once
#include "ray.h"
#include "vec3.h"

class aabb {
public:
    aabb() {}
    aabb(const point3& a, const point3& b) {
        minimum = a; maximum = b;
    }

    point3 min() const { return minimum; }
    point3 max() const { return maximum; }
    
    bool hit(const ray& r, float tMin, float tMax) const;

    point3 minimum;
    point3 maximum;
};

inline bool aabb::hit(const ray& r, float tMin, float tMax) const {
    for (int a = 0; a < 3; a++) {
        auto invD = 1.0f / r.direction()[a];
        auto t0 = (min()[a] - r.origin()[a]) * invD;
        auto t1 = (max()[a] - r.origin()[a]) * invD;
        if (invD < 0.0f)
            std::swap(t0, t1);
        tMin = t0 > tMin ? t0 : tMin;
        tMax = t1 < tMax ? t1 : tMax;
        if (tMax <= tMin)
            return false;
    }
    return true;
}

inline aabb surroundingBox(aabb box0, aabb box1) {
    point3 small(fmin(box0.min().x(), box1.min().x()),
                 fmin(box0.min().y(), box1.min().y()),
                 fmin(box0.min().z(), box1.min().z()));

    point3 big(fmax(box0.max().x(), box1.max().x()),
               fmax(box0.max().y(), box1.max().y()),
               fmax(box0.max().z(), box1.max().z()));

    return aabb(small,big);
}

//inline bool aabb::hit(const ray& r, float tMin, float tMax) const {
//    for (int i = 0; i < 3; ++i) {
//        auto t0 = fmin((minimum[i] - r.origin()[i]) / r.direction()[i],
//        (maximum[i] - r.origin()[i]) / r.direction()[i]);
//        auto t1 = fmax((minimum[i] - r.origin()[i]) / r.direction()[i],
//        (maximum[i] - r.origin()[i]) / r.direction()[i]);
//        tMin = fmax(t0, tMax);
//        tMax = fmin(t1, tMin);
//        if (tMax <= tMin) {
//            return false;
//        }
//    }
//    return true;
//}