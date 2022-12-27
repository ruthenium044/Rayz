#pragma once
#include <algorithm>
#include "hittableList.h"

class hittableList;

inline bool box_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b, int axis) {
    aabb box_a;
    aabb box_b;

    if (!a->boundingBox(0,0, box_a) || !b->boundingBox(0,0, box_b))
        std::cerr << "No bounding box in bvh_node constructor.\n";

    return box_a.min().e[axis] < box_b.min().e[axis];
}

inline bool boxCompareX (const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
    return box_compare(a, b, 0);
}

inline bool boxCompareY (const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
    return box_compare(a, b, 1);
}

inline bool boxCompareZ (const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
    return box_compare(a, b, 2);
}

class bvhNode : public hittable {
public:
    bvhNode();
    bvhNode(const hittableList& list, float time0, float time1) : bvhNode(list.objects, 0, list.objects.size(), time0, time1) {}
    bvhNode(const std::vector<std::shared_ptr<hittable>>& srcObjects, size_t start, size_t end, float time0, float time1);

    virtual bool hit(const ray& r, float tMin, float tMax, hitRecord& rec) const override;
    virtual bool boundingBox(float time0, float time1, aabb& outputBox) const override;

public:
    std::shared_ptr<hittable> left;
    std::shared_ptr<hittable> right;
    aabb box;
};

inline bvhNode::bvhNode(const std::vector<std::shared_ptr<hittable>>& srcObjects, size_t start, size_t end, float time0, float time1) {
    auto objects = srcObjects; // Create a modifiable array of the source scene objects

    int axis = randInt(0,2);
    auto comparator = (axis == 0) ? boxCompareX : (axis == 1) ? boxCompareY : boxCompareZ;

    size_t objectSpan = end - start;

    if (objectSpan == 1) {
        left = right = objects[start];
    } else if (objectSpan == 2) {
        if (comparator(objects[start], objects[start+1])) {
            left = objects[start];
            right = objects[start+1];
        } else {
            left = objects[start+1];
            right = objects[start];
        }
    } else {
        std::sort(objects.begin() + start, objects.begin() + end, comparator);

        auto mid = start + objectSpan/2;
        left = make_shared<bvhNode>(objects, start, mid, time0, time1);
        right = make_shared<bvhNode>(objects, mid, end, time0, time1);
    }

    aabb box_left, box_right;

    if (  !left->boundingBox(time0, time1, box_left)
       || !right->boundingBox(time0, time1, box_right)
    )
        std::cerr << "No bounding box in bvh_node constructor.\n";

    box = surroundingBox(box_left, box_right);
}

inline bool bvhNode::hit(const ray& r, float tMin, float tMax, hitRecord& rec) const {
    if (!box.hit(r, tMin, tMax)) {
        return false;
    }
    bool hitLeft = left->hit(r, tMin, tMax, rec);
    bool hitRight = right->hit(r, tMin, hitLeft ? rec.t : tMax, rec);
    return hitLeft || hitRight;
}

inline bool bvhNode::boundingBox(float time0, float time1, aabb& outputBox) const {
    outputBox = box;
    return true;
}
