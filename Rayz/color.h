﻿#pragma once

inline void writeColor(std::ostream &out, color pixelColor) {
    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(255.999f * pixelColor.x()) << ' '
        << static_cast<int>(255.999f * pixelColor.y()) << ' '
        << static_cast<int>(255.999f * pixelColor.z()) << '\n';
}

inline void writeColor(std::ostream &out, color pixelColor, int samplesPerPixel) {
    auto r = pixelColor.x();
    auto g = pixelColor.y();
    auto b = pixelColor.z();

    // Divide the color by the number of samples and gamma-correct for gamma=2.0.
    float scale = 1.0f / static_cast<float>(samplesPerPixel);
    r = sqrt(scale * r);
    g = sqrt(scale * g);
    b = sqrt(scale * b);
    
    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(256 * clamp(r, 0.0f, 0.999f)) << ' '
        << static_cast<int>(256 * clamp(g, 0.0f, 0.999f)) << ' '
        << static_cast<int>(256 * clamp(b, 0.0f, 0.999f)) << '\n';
}
struct RGB{
    unsigned char r;
    unsigned char g;
    unsigned char b;
};

inline void writeColor(RGB &out, color pixelColor, int samplesPerPixel) {
    auto r = pixelColor.x();
    auto g = pixelColor.y();
    auto b = pixelColor.z();

    // Divide the color by the number of samples and gamma-correct for gamma=2.0.
    float scale = 1.0f / static_cast<float>(samplesPerPixel);
    r = sqrt(scale * r);
    g = sqrt(scale * g);
    b = sqrt(scale * b);
    
    out.r = static_cast<int>(256 * clamp(r, 0.0f, 0.999f));
    out.g = static_cast<int>(256 * clamp(g, 0.0f, 0.999f));
    out.b = static_cast<int>(256 * clamp(b, 0.0f, 0.999f));
}