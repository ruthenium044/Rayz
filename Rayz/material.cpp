#include "material.h"
#include "hittable.h"

bool lambertian::scatter(const ray& rIn, const hitRecord& rec, color& attenuation, ray& scattered) const  {
    auto scatterDirection = rec.normal + random_unit_vector();
        
    if (scatterDirection.near_zero())
        scatterDirection = rec.normal;
        
    scattered = ray(rec.p, scatterDirection, rIn.time());
    attenuation = albedo->value(rec.u, rec.v, rec.p);
    return true;
}

bool metal::scatter(const ray& rIn, const hitRecord& rec, color& attenuation, ray& scattered) const {
    const vec3 reflected = reflect(unit_vector(rIn.direction()), rec.normal);
    scattered = ray(rec.p, reflected + fuzz*random_in_unit_sphere(), rIn.time());
    attenuation = albedo;
    return (dot(scattered.direction(), rec.normal) > 0);
}

bool dielectric::scatter(const ray& rIn, const hitRecord& rec, color& attenuation, ray& scattered) const {
    attenuation = color(1.0, 1.0, 1.0);
    float refractionRatio = rec.frontFace ? (1.0f/ir) : ir;

    vec3 unitDirection = unit_vector(rIn.direction());
    float cosTheta = fmin(dot(-unitDirection, rec.normal), 1.0f);
    float sinTheta = sqrt(1.0f - cosTheta*cosTheta);

    bool cannotRefract = refractionRatio * sinTheta > 1.0f;
    vec3 direction;

    if (cannotRefract || reflectance(cosTheta, refractionRatio) > randFloat())
        direction = reflect(unitDirection, rec.normal);
    else
        direction = refract(unitDirection, rec.normal, refractionRatio);

    scattered = ray(rec.p, direction, rIn.time());
    return true;
}