#pragma once

#include "ray.h"
#include "utils.h"
#include "vec3.h"

class camera {
public:
    camera(point3 lookfrom, point3 lookat, vec3 vup, float vfov,  float aspectRatio, float aperture, float focusDist,
        float time0, float time1) {
        
        auto theta = degToRad(vfov);
        auto h = tan(theta / 2.0f);
        auto viewportHeight = 2.0f * h;
        auto viewportWidth = aspectRatio * viewportHeight;

        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);

        origin = lookfrom;
        horizontal = focusDist * viewportWidth * u;
        vertical = focusDist * viewportHeight * v;
        lowerLeftCorner = origin - horizontal/2 - vertical/2 - focusDist*w;

        lensRadius = aperture / 2.0f;
        this->time0 = time0;
        this->time1 = time1;
    }

    ray get_ray(float s, float t) const {
        vec3 rd = lensRadius * random_in_unit_disk();
        vec3 offset = u * rd.x() + v * rd.y();

        return ray(origin + offset, lowerLeftCorner + s * horizontal + t * vertical - origin - offset,
            randFloat(time0, time1));
    }

private:
    point3 origin;
    point3 lowerLeftCorner;
    vec3 horizontal;
    vec3 vertical;
    vec3 u, v, w;
    float lensRadius;
    float time0;
    float time1;
};
