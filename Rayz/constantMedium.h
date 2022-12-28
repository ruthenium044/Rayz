#pragma once
#include "hittable.h"
#include "material.h"
#include "texture.h"

class constantMedium : public hittable {
public:

    constantMedium(shared_ptr<hittable> b, float d, shared_ptr<texture> a)
        : boundary(b),
          negInvDensity(-1/d),
          phaseFunction(make_shared<isotropic>(a))
    {}

    constantMedium(shared_ptr<hittable> b, float d, color c)
        : boundary(b),
          negInvDensity(-1/d),
          phaseFunction(make_shared<isotropic>(c))
    {}

    bool hit(const ray& r, float tMin, float tMax, hitRecord& rec) const override;
    bool boundingBox(float time0, float time1, aabb& outputBox) const override;
    
    shared_ptr<hittable> boundary;
    shared_ptr<material> phaseFunction;
    float negInvDensity;
};
