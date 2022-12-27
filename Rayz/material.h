#pragma once
#include "ray.h"
#include "texture.h"

class material {
public:
    virtual ~material() = default;

    virtual color emitted(float u, float v, const point3& p) const {
        return color(0,0,0);
    }
    virtual bool scatter(const ray& rIn, const struct hitRecord& rec, color& attenuation, ray& scattered) const = 0;
};

class lambertian : public material {
public:
    lambertian(const color& a) : albedo(make_shared<solidColor>(a)) {}
    lambertian(shared_ptr<texture> a) : albedo(a) {}
    bool scatter(const ray& rIn, const hitRecord& rec, color& attenuation, ray& scattered) const override;
    
    shared_ptr<texture> albedo;
};

class metal : public material {
public:
    metal(const color& a, float f) : albedo(a), fuzz(f < 1 ? f : 1) {}
    bool scatter(const ray& rIn, const hitRecord& rec, color& attenuation, ray& scattered) const override;
    
    color albedo;
    float fuzz;
};

class dielectric : public material {
public:
    dielectric(float refractionIndex) : ir(refractionIndex) {}
    bool scatter(const ray& rIn, const hitRecord& rec, color& attenuation, ray& scattered) const override;

    float ir; // Index of Refraction

    private:
    static float reflectance(float cosine, float refIdx) {
        // Use Schlick's approximation for reflectance.
        auto r0 = (1.0f - refIdx) / (1.0f + refIdx);
        r0 = r0 * r0;
        return r0 + (1.0f - r0)*pow((1.0f - cosine),5.0f);
    }
};

class diffuseLight : public material  {
public:
    diffuseLight(shared_ptr<texture> a) : emit(a) {}
    diffuseLight(color c) : emit(make_shared<solidColor>(c)) {}

    bool scatter(const ray& r_in, const hitRecord& rec, color& attenuation, ray& scattered) const override {
        return false;
    }

    color emitted(float u, float v, const point3& p) const override {
        return emit->value(u, v, p);
    }
    
    shared_ptr<texture> emit;
};