#pragma once
#include "hittable.h"
#include "ray.h"
#include "texture.h"

struct hitRecord;


class material {
public:
    virtual bool scatter(const ray& rIn, const hitRecord& rec, color& attenuation, ray& scattered) const = 0;
};

class lambertian : public material {
public:
    lambertian(const color& a) : albedo(make_shared<solidColor>(a)) {}
    lambertian(shared_ptr<texture> a) : albedo(a) {}


    virtual bool scatter(const ray& rIn, const hitRecord& rec, color& attenuation, ray& scattered) const override {
        auto scatterDirection = rec.normal + random_unit_vector();
        
        if (scatterDirection.near_zero())
            scatterDirection = rec.normal;
        
        scattered = ray(rec.p, scatterDirection, rIn.time());
        attenuation = albedo->value(rec.u, rec.v, rec.p);
        return true;
    }

    shared_ptr<texture> albedo;
};

class metal : public material {
public:
    metal(const color& a, float f) : albedo(a), fuzz(f < 1 ? f : 1) {}

    virtual bool scatter(const ray& rIn, const hitRecord& rec, color& attenuation, ray& scattered) const override {
        vec3 reflected = reflect(unit_vector(rIn.direction()), rec.normal);
        scattered = ray(rec.p, reflected + fuzz*random_in_unit_sphere(), rIn.time());
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    }

public:
    color albedo;
    float fuzz;
};

class dielectric : public material {
public:
    dielectric(float refractionIndex) : ir(refractionIndex) {}

    virtual bool scatter(const ray& rIn, const hitRecord& rec, color& attenuation, ray& scattered) const override {
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

public:
    float ir; // Index of Refraction

    private:
    static float reflectance(float cosine, float refIdx) {
        // Use Schlick's approximation for reflectance.
        auto r0 = (1.0f - refIdx) / (1.0f + refIdx);
        r0 = r0 * r0;
        return r0 + (1.0f - r0)*pow((1.0f - cosine),5.0f);
    }
};