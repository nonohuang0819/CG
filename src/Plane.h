#ifndef PLANE_H
#define PLANE_H

#include "vec3.h"
#include "ray.h"

struct Plane {
    vec3 point;
    vec3 normal;
};

bool hit_plane(const Plane& plane, const ray& r, float& t, vec3& N) {
    float denom = dot(plane.normal, r.direction());
    if (abs(denom) > 0.0001) {
        t = dot(plane.point - r.origin(), plane.normal) / denom;
        if (t >= 0) {
            N = plane.normal;
            return true;
        }
    }
    return false;
}

#endif // PLANE_H