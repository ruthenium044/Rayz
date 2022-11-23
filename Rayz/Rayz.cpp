//refs
//https://raytracing.github.io/books/RayTracingInOneWeekend.html

#include "utils.h"

#include "camera.h"
#include "color.h"
#include "hittableList.h"
#include "sphere.h"
#include "material.h"
#include "movingSphere.h"

#include <iostream>
#include <sstream>


hittableList randomScene() {
    // initialize random seed:
    srand (time(nullptr));
    
    hittableList world;

    auto groundMaterial = make_shared<lambertian>(color(0.2f, 0.2f, 0.2f));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, groundMaterial));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto chooseMat = randFloat();
            point3 center(a + 0.9f * randFloat(), 0.2f, b + 0.9f * randFloat());

            if ((center - point3(4, 0.2f, 0)).length() > 0.9f) {
                shared_ptr<material> sphereMaterial;

                if (chooseMat < 0.8f) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphereMaterial = make_shared<lambertian>(albedo);
                    auto center2 = center + vec3(0, randFloat(0,.5), 0);
                    world.add(make_shared<movingSphere>(center, center2, 0.0, 1.0, 0.2, sphereMaterial));
                
                } else if (chooseMat < 0.95f) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = randFloat(0, 0.5);
                    sphereMaterial = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphereMaterial));
                } else {
                    // glass
                    sphereMaterial = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphereMaterial));
                }
            }
        }
    }
    auto material1 = make_shared<dielectric>(1.5);
    auto material2 = make_shared<lambertian>(color(0.4f, 0.7f, 0.1f));
    auto material3 = make_shared<metal>(color(0.7f, 0.6f, 0.5f), 0.0);

    world.add(make_shared<sphere>(point3(4, 1.0f, 0.5f), 1.0, material1));
    world.add(make_shared<sphere>(point3(2, 1.0f, -1.5f), 1.0, material2));
    world.add(make_shared<sphere>(point3(0, 1.0f, 2.5f), 1.0, material3));
    return world;
}

color rayColor(const ray& r, const hittable& world, int depth) {
    hitRecord rec;
    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0)
        return color(0,0,0);
    
    if (world.hit(r, 0.001f, infinity, rec)) {
        ray scattered;
        color attenuation;
        if (rec.matPtr->scatter(r, rec, attenuation, scattered))
            return attenuation * rayColor(scattered, world, depth-1);
        return color(0,0,0);
    }
    vec3 unit_direction = unit_vector(r.direction());
    float t = 0.5f * (unit_direction.y() + 1.0f);
    return (1.0f - t) * color(1.0f, 1.0f, 1.0f) + t * color(1.0f, 1.0f, 0.6f);
}

int main()
{
    // Image
    const auto aspectRatio = 16.0f / 9.0f;
    const int imageWidth = 400;
    const int imageHeight = static_cast<int>(imageWidth / aspectRatio);
    const int samplesPerPixel = 16;
    const int maxDepth = 50;
    
    // World
    auto world = randomScene();
    
    // Camera
    point3 lookfrom(13,2,3);
    point3 lookat(0,0,0);
    vec3 vup(0,1,0);
    auto distToFocus = 10.0f;
    auto aperture = 0.1f;

    camera cam(lookfrom, lookat, vup, 20, aspectRatio, aperture, distToFocus, 0.0f, 1.0f);
    
    // Render
    std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";
    std::string lines;

    float widthFac = 1.0f / static_cast<float>(imageWidth - 1);
    float heightFac = 1.0f / static_cast<float>(imageHeight - 1);
    
    std::stringstream stringstream;
    for (int j = imageHeight - 1 ; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < imageWidth; ++i) {
            color pixel_color(0, 0, 0);
            
            float fi = static_cast<float>(i);
            float fj = static_cast<float>(j);
            
            for (int s = 0; s < samplesPerPixel; s++) {
                float u0 = (fi + randFloat()) * widthFac;
                float v0 = (fj + randFloat()) * heightFac;
                ray r0 = cam.get_ray(u0, v0);
                pixel_color += rayColor(r0, world, maxDepth);
                
            }
            write_color(stringstream, pixel_color, samplesPerPixel);
        }
    }
    std::cout << stringstream.str();
    
    std::cerr << "\nDone.\n";
    
    //C:\Users\ruta.sapokaite\Documents\Github\Rayz\Rayz\x64\Release\Rayz.exe > image.ppm
    //C:\Data\Github\Rayz\x64\Release\Rayz.exe > image.ppm
}
