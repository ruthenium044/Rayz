#pragma once
#include "hittable.h"

#include "vec3.h"

class movingSphere : public hittable {
public:
    movingSphere() {}
    movingSphere(point3 cen0, point3 cen1, float tm0, float tm1, float r, std::shared_ptr<material> m)
            : center0(cen0), center1(cen1), time0(tm0), time1(tm1), radius(r), matPtr(m) {}

    virtual bool hit(const ray& r, float tMin, float tMax, hitRecord& rec) const override;

    point3 center(float t) const;

public:
    point3 center0;
    point3 center1;
    float time0;
    float time1;
    float radius;
    std::shared_ptr<material> matPtr;
};

inline point3 movingSphere::center(float time) const {
    return center0 + ((time - time0) / (time1 - time0)) * (center1 - center0);
}

inline bool movingSphere::hit(const ray& r, float tMin, float tMax, hitRecord& rec) const {
    vec3 oc = r.origin() - center(r.time());
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius*radius;

    auto discriminant = half_b*half_b - a*c;
    if (discriminant < 0) return false;
    auto sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    auto root = (-half_b - sqrtd) / a;
    if (root < tMin || tMax < root) {
        root = (-half_b + sqrtd) / a;
        if (root < tMin || tMax < root)
            return false;
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    auto outward_normal = (rec.p - center(r.time())) / radius;
    rec.set_face_normal(r, outward_normal);
    rec.matPtr = matPtr;
    
    return true;
}
