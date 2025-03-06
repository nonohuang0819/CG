#ifndef CUBE_H
#define CUBE_H

#include "vec3.h"
#include "ray.h"
#include <algorithm>

using namespace std;

struct Cube {
    vec3 center;
    float side_length;
};

bool hit_cube(const Cube& cube, const ray& r, float& t, vec3& N) {
    const float EPSILON = 0.0000001;
    float t_min = (cube.center.x() - cube.side_length / 2 - r.origin().x()) / r.direction().x();
    float t_max = (cube.center.x() + cube.side_length / 2 - r.origin().x()) / r.direction().x();

    if (t_min > t_max) swap(t_min, t_max);

    float ty_min = (cube.center.y() - cube.side_length / 2 - r.origin().y()) / r.direction().y();
    float ty_max = (cube.center.y() + cube.side_length / 2 - r.origin().y()) / r.direction().y();

    if (ty_min > ty_max) swap(ty_min, ty_max);

    if ((t_min > ty_max) || (ty_min > t_max))
        return false;

    if (ty_min > t_min)
        t_min = ty_min;

    if (ty_max < t_max)
        t_max = ty_max;

    float tz_min = (cube.center.z() - cube.side_length / 2 - r.origin().z()) / r.direction().z();
    float tz_max = (cube.center.z() + cube.side_length / 2 - r.origin().z()) / r.direction().z();

    if (tz_min > tz_max) swap(tz_min, tz_max);

    if ((t_min > tz_max) || (tz_min > t_max))
        return false;

    if (tz_min > t_min)
        t_min = tz_min;

    if (tz_max < t_max)
        t_max = tz_max;

    t = t_min;
    if (t < EPSILON) {
        t = t_max;
        if (t < EPSILON)
            return false;
    }

    vec3 hit_point = r.point_at_parameter(t);
    vec3 cube_min = cube.center - vec3(cube.side_length / 2, cube.side_length / 2, cube.side_length / 2);
    vec3 cube_max = cube.center + vec3(cube.side_length / 2, cube.side_length / 2, cube.side_length / 2);

    if (abs(hit_point.x() - cube_min.x()) < EPSILON) N = vec3(-1, 0, 0);
    else if (abs(hit_point.x() - cube_max.x()) < EPSILON) N = vec3(1, 0, 0);
    else if (abs(hit_point.y() - cube_min.y()) < EPSILON) N = vec3(0, -1, 0);
    else if (abs(hit_point.y() - cube_max.y()) < EPSILON) N = vec3(0, 1, 0);
    else if (abs(hit_point.z() - cube_min.z()) < EPSILON) N = vec3(0, 0, -1);
    else if (abs(hit_point.z() - cube_max.z()) < EPSILON) N = vec3(0, 0, 1);

    return true;
}

#endif // CUBE_H