#include <iostream>
#include <fstream>
#include <vector>
#include "vec3.h"
#include "ray.h"
using namespace std;

// Structure to represent a light source with position and intensity
struct Light {
    vec3 position;
    vec3 intensity;
};

// Function to check if a ray intersects with a sphere
// Returns the parameter t at which the intersection occurs, or -1.0 if there is no intersection
float hit_sphere(const vec3 &center, float radius, const ray& r) {
    vec3 objectCenter = r.origin() - center; // Vector from ray origin to sphere center
    float a = dot(r.direction(), r.direction()); // Coefficient a in the quadratic equation
    float b = 2.0 * dot(objectCenter, r.direction()); // Coefficient b in the quadratic equation
    float c = dot(objectCenter, objectCenter) - radius * radius; // Coefficient c in the quadratic equation
    float discriminant = b * b - 4 * a * c; // Discriminant of the quadratic equation
    if (discriminant < 0)  return -1.0; // No intersection if discriminant is negative
    else return (-b - sqrt(discriminant)) / (2.0 * a); // Return the nearest intersection point
}

// Function to check if a ray intersects with a plane
// Returns the parameter t at which the intersection occurs, or -1.0 if there is no intersection
float hit_plane(const vec3 &point_on_plane, const vec3 &normal, const ray& r) {
    float denom = dot(normal, r.direction()); // Dot product of plane normal and ray direction
    if (fabs(denom) > 1e-6) { // Avoid division by zero
        float t = dot(point_on_plane - r.origin(), normal) / denom; // Calculate intersection parameter t
        return (t >= 0) ? t : -1.0; // Return t if it is positive, otherwise return -1.0
    }
    return -1.0; // No intersection if denom is zero
}


// Function to compute the color of a ray based on its intersection with objects in the scene
vec3 color(const ray& r, const vector<Light>& lights) {
    vec3 center(0, 0, -1); // Center of the sphere
    float t_sphere = hit_sphere(center, 0.5, r); // Check for intersection with the sphere

    vec3 plane_point(0, -0.5, -1); // A point on the plane
    vec3 plane_normal(0, 1, 0); // Normal vector of the plane
    float t_plane = hit_plane(plane_point, plane_normal, r); // Check for intersection with the plane

    // If the ray intersects with the sphere and it is closer than the plane
    if (t_sphere > 0.0 && (t_plane < 0.0 || t_sphere < t_plane)) {
        vec3 N = unit_vector(r.point_at_parameter(t_sphere) - center); // Normal at the intersection point
        vec3 color(0, 0, 0); // Initialize color to black
        for (const auto& light : lights) { // Loop through each light source
            vec3 L = unit_vector(light.position - r.point_at_parameter(t_sphere)); // Direction to the light source
            float diffuse = max(dot(N, L), 0.0f); // Calculate diffuse lighting
            color += light.intensity * diffuse; // Add the contribution of the light source to the color
        }
        return color; // Return the final color
    } 
    // If the ray intersects with the plane
    else if (t_plane > 0.0) {
        vec3 N = plane_normal; // Normal of the plane
        vec3 color(0.8, 0.8, 0.8); // Initialize color to gray
        for (const auto& light : lights) { // Loop through each light source
            vec3 L = unit_vector(light.position - r.point_at_parameter(t_plane)); // Direction to the light source
            float diffuse = max(dot(N, L), 0.0f); // Calculate diffuse lighting
            color += light.intensity * diffuse; // Add the contribution of the light source to the color
        }
        return color; // Return the final color
    }

    // If the ray does not intersect with any object, return the background color
    vec3 unit_direction = unit_vector(r.direction()); // Unit vector of the ray direction
    float t = 0.5 * (unit_direction.y() + 1.0); // Calculate interpolation parameter
    return (1.0 - t) * vec3(1, 1, 1) + t * vec3(0.5, 0.7, 1.0); // Interpolate between white and light blue
}

int main() {
    int width = 200; // Image width
    int height = 100; // Image height

    fstream file; 
    file.open("/home/nonohuang/CG/src/ray.ppm", ios::out); 

    file << "P3\n" << width << " " << height << "\n255\n"; // Write the PPM header
    vec3 lower_left_corner(-2, -1, -1); // Lower left corner of the view frustum
    vec3 horizontal(4, 0, 0); // Horizontal vector of the view frustum
    vec3 vertical(0, 2, 0); // Vertical vector of the view frustum
    vec3 origin(0, 0, 0); // Origin of the camera

    // Define light sources
    vector<Light> lights = {
        {vec3(-1, 1, 0), vec3(1, 0, 1)}, // Purple light
        {vec3(2, 1, 0), vec3(1, 1, 0)} // Yellow light
    };

    // Loop through each pixel in the image
    for (int j = height - 1; j >= 0; j--) {
        for (int i = 0; i < width; i++) {
            float u = float(i) / float(width); // Horizontal coordinate
            float v = float(j) / float(height); // Vertical coordinate
            ray r(origin, lower_left_corner + (u * horizontal) + (v * vertical) - origin); // Generate ray for the pixel
            vec3 col = color(r, lights); // Compute the color for the ray
            file << int(255.99 * col.x()) << " " // Write the color to the file
                 << int(255.99 * col.y()) << " "
                 << int(255.99 * col.z()) << "\n";
        }
    }

    file.close(); 

    return 0; 
}