#pragma once

#include "aabb.h"
#include "ray.h"
#include "vec3.h"

class material;

struct hitRecord {
    point3 p;
    vec3 normal;
    shared_ptr<material> matPtr;
    float t;
    float u;
    float v;

    bool frontFace;

    void setFaceNormal(const ray& r, const vec3& outwardNormal) {
        frontFace = dot(r.direction(), outwardNormal) < 0;
        normal = frontFace ? outwardNormal :-outwardNormal;
    }
};

class hittable {
public:
    virtual bool hit(const ray& r, float tMin, float tMax, hitRecord& rec) const = 0;
    virtual bool boundingBox(float time0, float time1, aabb& outputBox) const = 0;
};

class translate : public hittable {
public:
    translate(shared_ptr<hittable> p, const vec3& displacement)
        : ptr(p), offset(displacement) {}

    bool hit(const ray& r, float tMin, float tMax, hitRecord& rec) const override;
    bool boundingBox(float time0, float time1, aabb& outputBox) const override;

    shared_ptr<hittable> ptr;
    vec3 offset;
};

inline bool translate::hit(const ray& r, float tMin, float tMax, hitRecord& rec) const {
    ray movedR(r.origin() - offset, r.direction(), r.time());
    if (!ptr->hit(movedR, tMin, tMax, rec))
        return false;

    rec.p += offset;
    rec.setFaceNormal(movedR, rec.normal);

    return true;
}

inline bool translate::boundingBox(float time0, float time1, aabb& outputBox) const {
    if (!ptr->boundingBox(time0, time1, outputBox))
        return false;
    outputBox = aabb(outputBox.min() + offset, outputBox.max() + offset);
    return true;
}

class rotateY : public hittable {
public:
    rotateY(shared_ptr<hittable> p, float angle);

    bool hit(const ray& r, float tMin, float tMax, hitRecord& rec) const override;

    bool boundingBox(float time0, float time1, aabb& outputBox) const override {
        outputBox = bbox;
        return hasbox;
    }

    shared_ptr<hittable> ptr;
    float sinTheta;
    float cosTheta;
    bool hasbox;
    aabb bbox;
};
