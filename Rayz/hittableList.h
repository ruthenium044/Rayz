#pragma once

#include "hittable.h"

#include <memory>
#include <vector>

using namespace std;

class hittableList : public hittable {
    public:
        hittableList() = default;
        hittableList(shared_ptr<hittable> object) { add(object); }

        void clear() { objects.clear(); }
        void add(shared_ptr<hittable> object) { objects.push_back(object); }

        virtual bool hit( const ray& r, float t_min, float t_max, hit_record& rec) const override;

    public:
        std::vector<shared_ptr<hittable>> objects;
};

inline bool hittableList::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
    hit_record temp_rec;
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