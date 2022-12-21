#pragma once

#include "hittable.h"

#include <memory>
#include <vector>

#include "intersect.h"

using namespace std;

class hittableList : public hittable {
    public:
        hittableList() = default;
        hittableList(shared_ptr<hittable> object) { add(object); }

        void clear() { objects.clear(); }
        void add(shared_ptr<hittable> object) { objects.push_back(object); }

        virtual bool hit( const ray& r, float t_min, float t_max, hitRecord& rec) const override;
        virtual bool boundingBox(float time0, float time1, aabb& outputBox) const override;

        std::vector<shared_ptr<hittable>> objects;
};

inline bool hittableList::hit(const ray& r, float t_min, float t_max, hitRecord& rec) const {
    hitRecord temp_rec;
    bool hit_anything = false;
    auto closest_so_far = t_max;

    for (const auto& object : objects) {
        if (object->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}

bool hittableList::boundingBox(float time0, float time1, aabb& outputBox) const {
    if (objects.empty()) return false;

    aabb temp_box;
    bool first_box = true;

    for (const auto& object : objects) {
        if (!object->boundingBox(time0, time1, temp_box)) return false;
        outputBox = first_box ? temp_box : surroundingBox(outputBox, temp_box);
        first_box = false;
    }

    return true;
}