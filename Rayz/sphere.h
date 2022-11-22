#pragma once

#include "hittable.h"
#include "vec3.h"

class sphere : public hittable {
public:
    sphere() {}
    sphere(point3 cen, float r, shared_ptr<material> m) : center(cen), radius(r), matPtr(m) {}

    virtual bool hit(const ray& r, float t_min, float t_max, hitRecord& rec) const override;

public:
    point3 center;
    float radius{};
    shared_ptr<material> matPtr;
};

inline bool sphere::hit(const ray& r, float t_min, float t_max, hitRecord& rec) const {
    vec3 oc = r.origin() - center;
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius*radius;

    auto discriminant = half_b*half_b - a*c;
    if (discriminant < 0) return false;
    auto sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    auto root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root) {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root)
            return false;
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    vec3 outwardNormal = (rec.p - center) / radius;
    rec.set_face_normal(r, outwardNormal);
    rec.matPtr = matPtr;
    
    return true;
}
