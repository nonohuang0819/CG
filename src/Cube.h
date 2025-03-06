#ifndef CUBE_H
#define CUBE_H

#include "vec3.h"
#include "ray.h"
#include <algorithm>  // Include algorithm header for std::swap

using namespace std;

// Define the Cube structure
struct Cube {
    vec3 center;      // Center of the cube
    float side_length;  // Side length of the cube
};

// Function to check if a ray intersects with a cube
bool hit_cube(const Cube& cube, const ray& r, float& t, vec3& N) {
    const float EPSILON = 0.0000001;  // Small value for floating-point comparisons
    // Compute the intersection distances along the x-axis
    float t_min = (cube.center.x() - cube.side_length / 2 - r.origin().x()) / r.direction().x();
    float t_max = (cube.center.x() + cube.side_length / 2 - r.origin().x()) / r.direction().x();

    if (t_min > t_max) swap(t_min, t_max);  // Ensure t_min is less than t_max

    // Compute the intersection distances along the y-axis
    float ty_min = (cube.center.y() - cube.side_length / 2 - r.origin().y()) / r.direction().y();
    float ty_max = (cube.center.y() + cube.side_length / 2 - r.origin().y()) / r.direction().y();

    if (ty_min > ty_max) swap(ty_min, ty_max);  // Ensure ty_min is less than ty_max

    // Check if there is an intersection
    if ((t_min > ty_max) || (ty_min > t_max))
        return false;

    // Update t_min and t_max
    if (ty_min > t_min)
        t_min = ty_min;

    if (ty_max < t_max)
        t_max = ty_max;

    // Compute the intersection distances along the z-axis
    float tz_min = (cube.center.z() - cube.side_length / 2 - r.origin().z()) / r.direction().z();
    float tz_max = (cube.center.z() + cube.side_length / 2 - r.origin().z()) / r.direction().z();

    if (tz_min > tz_max) swap(tz_min, tz_max);  // Ensure tz_min is less than tz_max

    // Check if there is an intersection
    if ((t_min > tz_max) || (tz_min > t_max))
        return false;

    // Update t_min and t_max
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

    vec3 hit_point = r.point_at_parameter(t);  // Compute the hit point
    vec3 cube_min = cube.center - vec3(cube.side_length / 2, cube.side_length / 2, cube.side_length / 2);
    vec3 cube_max = cube.center + vec3(cube.side_length / 2, cube.side_length / 2, cube.side_length / 2);

    // Determine the normal at the hit point
    if (abs(hit_point.x() - cube_min.x()) < EPSILON) N = vec3(-1, 0, 0);
    else if (abs(hit_point.x() - cube_max.x()) < EPSILON) N = vec3(1, 0, 0);
    else if (abs(hit_point.y() - cube_min.y()) < EPSILON) N = vec3(0, -1, 0);
    else if (abs(hit_point.y() - cube_max.y()) < EPSILON) N = vec3(0, 1, 0);
    else if (abs(hit_point.z() - cube_min.z()) < EPSILON) N = vec3(0, 0, -1);
    else if (abs(hit_point.z() - cube_max.z()) < EPSILON) N = vec3(0, 0, 1);

    return true;
}

#endif // CUBE_H