#include "constantMedium.h"

bool constantMedium::hit(const ray& r, float tMin, float tMax, hitRecord& rec) const { 
    // Print occasional samples when debugging. To enable, set enableDebug true.
    const bool enableDebug = false;
    const bool debugging = enableDebug && randFloat() < 0.00001f;

    hitRecord rec1, rec2;

    if (!boundary->hit(r, -infinity, infinity, rec1))
        return false;

    if (!boundary->hit(r, rec1.t+0.0001f, infinity, rec2))
        return false;

    if (debugging) std::cerr << "\nt_min=" << rec1.t << ", t_max=" << rec2.t << '\n';

    if (rec1.t < tMin) rec1.t = tMin;
    if (rec2.t > tMax) rec2.t = tMax;

    if (rec1.t >= rec2.t)
        return false;

    if (rec1.t < 0)
        rec1.t = 0;

    const auto ray_length = r.direction().length();
    const auto distance_inside_boundary = (rec2.t - rec1.t) * ray_length;
    const auto hit_distance = negInvDensity * log(randFloat());

    if (hit_distance > distance_inside_boundary)
        return false;

    rec.t = rec1.t + hit_distance / ray_length;
    rec.p = r.at(rec.t);

    if (debugging) {
        std::cerr << "hit_distance = " <<  hit_distance << '\n'
                  << "rec.t = " <<  rec.t << '\n'
                  << "rec.p = " <<  rec.p << '\n';
    }

    rec.normal = vec3(1,0,0);  // arbitrary
    rec.frontFace = true;     // also arbitrary
    rec.matPtr = phaseFunction;

    return true;; }

bool constantMedium::boundingBox(float time0, float time1, aabb& output_box) const {
    return boundary->boundingBox(time0, time1, output_box);
}
