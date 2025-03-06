#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "vec3.h"
#include "ray.h"

struct Triangle {
    vec3 v0, v1, v2;
};

bool hit_triangle(const Triangle& tri, const ray& r, float& t, vec3& N) {
    const float EPSILON = 0.0000001;
    vec3 edge1 = tri.v1 - tri.v0;
    vec3 edge2 = tri.v2 - tri.v0;
    vec3 h = cross(r.direction(), edge2);
    float a = dot(edge1, h);
    if (a > -EPSILON && a < EPSILON)
        return false;    // This ray is parallel to this triangle.
    float f = 1.0 / a;
    vec3 s = r.origin() - tri.v0;
    float u = f * dot(s, h);
    if (u < 0.0 || u > 1.0)
        return false;
    vec3 q = cross(s, edge1);
    float v = f * dot(r.direction(), q);
    if (v < 0.0 || u + v > 1.0)
        return false;
    t = f * dot(edge2, q);
    if (t > EPSILON) {
        N = unit_vector(cross(edge1, edge2));
        return true;
    } else {
        return false;
    }
}

#endif // TRIANGLE_H