#include <iostream>
#include <fstream>
#include <vector>
#include <cfloat> 
#include "vec3.h"
#include "ray.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Cube.h"
#include "Plane.h"
using namespace std;

// Define the Light structure
struct Light {
    vec3 position;   // Position of the light source
    vec3 intensity;  // Intensity of the light source
};

// Function to compute the color of a ray
vec3 color(const ray& r, const vector<Light>& lights, const vector<Sphere>& spheres, const vector<Triangle>& triangles, const vector<Cube>& cubes, const vector<Plane>& planes) {
    vec3 hit_color(0, 0, 0);  // Initialize the hit color to black
    float t_min = FLT_MAX;    // Initialize the minimum hit distance to the maximum float value
    vec3 N;                   // Normal at the hit point
    vec3 hit_point;           // Hit point

    // Check for intersection with spheres
    for (const auto& sphere : spheres) {
        float t;
        if (hit_sphere(sphere, r, t, N) && t < t_min) {
            t_min = t;
            hit_point = r.point_at_parameter(t);
            hit_color = vec3(0, 0, 0);
            for (const auto& light : lights) {
                vec3 L = unit_vector(light.position - hit_point);  // Direction to the light source
                float diffuse = max(dot(N, L), 0.0f);              // Diffuse lighting component
                hit_color += light.intensity * diffuse;            // Accumulate the light contribution
            }
        }
    }

    // Check for intersection with triangles
    for (const auto& tri : triangles) {
        float t;
        if (hit_triangle(tri, r, t, N) && t < t_min) {
            t_min = t;
            hit_point = r.point_at_parameter(t);
            hit_color = vec3(0, 0, 0);
            for (const auto& light : lights) {
                vec3 L = unit_vector(light.position - hit_point);  // Direction to the light source
                float diffuse = max(dot(N, L), 0.0f);              // Diffuse lighting component
                hit_color += light.intensity * diffuse;            // Accumulate the light contribution
            }
        }
    }

    // Check for intersection with cubes
    for (const auto& cube : cubes) {
        float t;
        if (hit_cube(cube, r, t, N) && t < t_min) {
            t_min = t;
            hit_point = r.point_at_parameter(t);
            hit_color = vec3(0, 0, 0);
            for (const auto& light : lights) {
                vec3 L = unit_vector(light.position - hit_point);  // Direction to the light source
                float diffuse = max(dot(N, L), 0.0f);              // Diffuse lighting component
                hit_color += light.intensity * diffuse;            // Accumulate the light contribution
            }
        }
    }

    // Check for intersection with planes
    for (const auto& plane : planes) {
        float t;
        if (hit_plane(plane, r, t, N) && t < t_min) {
            t_min = t;
            hit_point = r.point_at_parameter(t);
            hit_color = vec3(0, 0, 0);
            for (const auto& light : lights) {
                vec3 L = unit_vector(light.position - hit_point);  // Direction to the light source
                float diffuse = max(dot(N, L), 0.0f);              // Diffuse lighting component
                hit_color += light.intensity * diffuse;            // Accumulate the light contribution
            }
        }
    }

    // If there was an intersection, return the accumulated color
    if (t_min < FLT_MAX) {
        return hit_color;
    }

    // If no intersection, return the background color
    vec3 unit_direction = unit_vector(r.direction());
    float t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * vec3(1, 1, 1) + t * vec3(0.5, 0.7, 1.0);
}

int main() {
    int width = 800;            // Image width
    int height = 800;           // Image height
    int samples_per_pixel = 4;  // Number of samples per pixel for anti-aliasing

    fstream file;
    file.open("/home/nonohuang/CG/src/ray.ppm", ios::out);  // Open the output file

    file << "P3\n" << width << " " << height << "\n255\n";  // Write the PPM header
    vec3 lower_left_corner(-2, -1, -1);  // Lower left corner of the viewport
    vec3 horizontal(4, 0, 0);            // Horizontal span of the viewport
    vec3 vertical(0, 2, 0);              // Vertical span of the viewport
    vec3 origin(0, 0, 0);                // Camera origin

    // Define the lights in the scene
    vector<Light> lights = {
        {vec3(-1, 1, 0), vec3(1, 0, 1)}, // Purple light
        {vec3(2, 1, 0), vec3(1, 1, 0)}   // Yellow light
    };

    // Define the spheres in the scene
    vector<Sphere> spheres = {
        {vec3(0, 0, -1), 0.3}
    };

    // Define the triangles in the scene
    vector<Triangle> triangles = {
        {vec3(0.5, -0.25, -1), vec3(1.5, -0.25, -1), vec3(1, 0.25, -1)}
    };

    // Define the cubes in the scene
    vector<Cube> cubes = {
        {vec3(-1, 0, -1.25), 0.5}
    };

    // Define the planes in the scene
    vector<Plane> planes = {
        {vec3(0, -0.5, -1), vec3(0, 1, 0)}
    };

    // Loop over each pixel in the image
    for (int j = height - 1; j >= 0; j--) {
        for (int i = 0; i < width; i++) {
            vec3 col(0, 0, 0);  // Initialize the color to black
            // Perform anti-aliasing by taking multiple samples per pixel
            for (int s = 0; s < samples_per_pixel; s++) {
                float u = float(i + drand48()) / float(width);
                float v = float(j + drand48()) / float(height);
                ray r(origin, lower_left_corner + (u * horizontal) + (v * vertical) - origin);
                col += color(r, lights, spheres, triangles, cubes, planes);
            }
            col /= float(samples_per_pixel);  // Average the color samples
            file << int(255.99 * col.x()) << " "
                 << int(255.99 * col.y()) << " "
                 << int(255.99 * col.z()) << "\n";  // Write the color to the file
        }
    }

    file.close();  // Close the output file

    return 0;
}