#pragma once
#include "color.h"
#include "perlin.h"


class texture {
public:
    virtual color value(float u, float v, const point3& p) const = 0;
};

class solidColor : public texture {
public:
    solidColor() {}
    solidColor(color c) : colorValue(c) {}

    solidColor(float red, float green, float blue)
      : solidColor(color(red,green,blue)) {}

    virtual color value(float u, float v, const vec3& p) const override {
        return colorValue;
    }

private:
    color colorValue;
};

class checkerTexture : public texture {
public:
    checkerTexture() {}

    checkerTexture(shared_ptr<texture> _even, shared_ptr<texture> _odd)
        : odd(_odd), even(_even) {}

    checkerTexture(color c1, color c2)
        : odd(make_shared<solidColor>(c2)) , even(make_shared<solidColor>(c1)) {}

    virtual color value(float u, float v, const point3& p) const override {
        auto sines = sin(10*p.x())*sin(10*p.y())*sin(10*p.z());
        if (sines < 0)
            return odd->value(u, v, p);
        else
            return even->value(u, v, p);
    }

    shared_ptr<texture> odd;
    shared_ptr<texture> even;
};

class noiseTexture : public texture {
public:
    noiseTexture();
    noiseTexture(float sc) : scale(sc) {}
    
    virtual color value(float u, float v, const point3& p) const override {
        return color(1,1,1) * 0.5f * (1 + sin(scale*p.z() + 10*noise.turb(p)));
    }

    perlin noise;
    float scale{};
};