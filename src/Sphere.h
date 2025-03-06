#ifndef SPHERE_H
#define SPHERE_H

#include "vec3.h"
#include "ray.h"

struct Sphere {
    vec3 center;
    float radius;
};

bool hit_sphere(const Sphere& sphere, const ray& r, float& t, vec3& N) {
    vec3 oc = r.origin() - sphere.center;
    float a = dot(r.direction(), r.direction());
    float b = 2.0 * dot(oc, r.direction());
    float c = dot(oc, oc) - sphere.radius * sphere.radius;
    float discriminant = b * b - 4 * a * c;
    if (discriminant < 0) {
        return false;
    } else {
        t = (-b - sqrt(discriminant)) / (2.0 * a);
        N = unit_vector(r.point_at_parameter(t) - sphere.center);
        return true;
    }
}

#endif // SPHERE_H