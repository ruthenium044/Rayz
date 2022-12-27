#pragma once

#include "rectXY.h"

class box : public hittable  {
public:
    box() {}
    box(const point3& p0, const point3& p1, shared_ptr<material> ptr);

    virtual bool hit(const ray& r, float t_min, float t_max, hitRecord& rec) const override;

    virtual bool boundingBox(float time0, float time1, aabb& outputBox) const override {
        outputBox = aabb(boxMin, box_max);
        return true;
    }

    point3 boxMin;
    point3 box_max;
    hittableList sides;
};

inline box::box(const point3& p0, const point3& p1, shared_ptr<material> ptr) {
    boxMin = p0;
    box_max = p1;

    sides.add(make_shared<rectXY>(p0.x(), p1.x(), p0.y(), p1.y(), p1.z(), ptr));
    sides.add(make_shared<rectXY>(p0.x(), p1.x(), p0.y(), p1.y(), p0.z(), ptr));

    sides.add(make_shared<rectXZ>(p0.x(), p1.x(), p0.z(), p1.z(), p1.y(), ptr));
    sides.add(make_shared<rectXZ>(p0.x(), p1.x(), p0.z(), p1.z(), p0.y(), ptr));

    sides.add(make_shared<rectYZ>(p0.y(), p1.y(), p0.z(), p1.z(), p1.x(), ptr));
    sides.add(make_shared<rectYZ>(p0.y(), p1.y(), p0.z(), p1.z(), p0.x(), ptr));
}

inline bool box::hit(const ray& r, float t_min, float t_max, hitRecord& rec) const {
    return sides.hit(r, t_min, t_max, rec);
}
