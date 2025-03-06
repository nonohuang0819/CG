#ifndef PLANE_H
#define PLANE_H

#include "vec3.h"
#include "ray.h"

// Define the Plane structure
struct Plane {
    vec3 point;   // A point on the plane
    vec3 normal;  // The normal vector of the plane
};

// Function to check if a ray intersects with a plane
bool hit_plane(const Plane& plane, const ray& r, float& t, vec3& N) {
    float denom = dot(plane.normal, r.direction());  // Compute the dot product of the plane normal and the ray direction
    if (abs(denom) > 0.0001) {  // Check if the ray is not parallel to the plane
        t = dot(plane.point - r.origin(), plane.normal) / denom;  // Compute the intersection distance
        if (t >= 0) {  // Check if the intersection is in front of the ray origin
            N = plane.normal;  // Set the normal at the intersection point
            return true;  // Return true if there is an intersection
        }
    }
    return false;  // Return false if there is no intersection
}

#endif // PLANE_H