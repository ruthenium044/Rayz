﻿#pragma once
#include "vec3.h"

class perlin {
public:
    perlin() {
        ranvec = new vec3[pointCount];
        for (int i = 0; i < pointCount; ++i) {
            ranvec[i] = unit_vector(vec3::random(-1,1));
        }

        permX = perlin_generate_perm();
        permY = perlin_generate_perm();
        permZ = perlin_generate_perm();
    }

    ~perlin() {
        delete[] ranvec;
        delete[] ranfloat;
        delete[] permX;
        delete[] permY;
        delete[] permZ;
    }

    float noise(const point3& p) const {
        auto u = p.x() - floor(p.x());
        auto v = p.y() - floor(p.y());
        auto w = p.z() - floor(p.z());
        
        auto i = static_cast<int>(floor(p.x()));
        auto j = static_cast<int>(floor(p.y()));
        auto k = static_cast<int>(floor(p.z()));
        vec3 c[2][2][2];

        for (int di=0; di < 2; di++)
            for (int dj=0; dj < 2; dj++)
                for (int dk=0; dk < 2; dk++)
                    c[di][dj][dk] = ranvec[
                        permX[(i+di) & 255] ^
                        permY[(j+dj) & 255] ^
                        permZ[(k+dk) & 255]
                    ];

        return trilinearInterp(c, u, v, w);
    }

    float turb(const point3& p, int depth=7) const {
        auto accum = 0.0f;
        auto temp = p;
        auto weight = 1.0f;

        for (int i = 0; i < depth; i++) {
            accum += weight * noise(temp);
            weight *= 0.5f;
            temp *= 2;
        }

        return fabs(accum);
    }

private:
    static const int pointCount = 256;
    vec3* ranvec;
    float* ranfloat;
    int* permX;
    int* permY;
    int* permZ;

    static int* perlin_generate_perm() {
        auto p = new int[pointCount];

        for (int i = 0; i < pointCount; i++)
            p[i] = i;

        permute(p, pointCount);

        return p;
    }

    static void permute(int* p, int n) {
        for (int i = n-1; i > 0; i--) {
            int target = randInt(0, i);
            int tmp = p[i];
            p[i] = p[target];
            p[target] = tmp;
        }
    }

    static float trilinearInterp(vec3 c[2][2][2], float u, float v, float w) {
        auto uu = u*u*(3-2*u);
        auto vv = v*v*(3-2*v);
        auto ww = w*w*(3-2*w);
        auto accum = 0.0f;

        for (int i=0; i < 2; i++)
            for (int j=0; j < 2; j++)
                for (int k=0; k < 2; k++) {
                    vec3 weightV(u-i, v-j, w-k);
                    accum += (i*uu + (1-i)*(1-uu))
                           * (j*vv + (1-j)*(1-vv))
                           * (k*ww + (1-k)*(1-ww))
                           * dot(c[i][j][k], weightV);
                }
        return accum;
    }
};
