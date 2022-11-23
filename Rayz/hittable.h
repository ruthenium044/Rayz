#pragma once

#include "ray.h"

class aabb;
class material;

struct hitRecord {
    point3 p;
    vec3 normal;
    shared_ptr<material> matPtr;
    float t;

    bool frontFace;

    inline void set_face_normal(const ray& r, const vec3& outward_normal) {
        frontFace = dot(r.direction(), outward_normal) < 0;
        normal = frontFace ? outward_normal :-outward_normal;
    }
};

class hittable {
public:
    virtual bool hit(const ray& r, float tMin, float tMax, hitRecord& rec) const = 0;
    virtual bool boundingBox(float time0, float time1, aabb& outputBox) const = 0;
};