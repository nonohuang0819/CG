#include <iostream>
#include <fstream>
#include <vector>
#include <cfloat> // Include this header for FLT_MAX
#include "vec3.h"
#include "ray.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Cube.h"
#include "Plane.h"
using namespace std;

struct Light {
    vec3 position;
    vec3 intensity;
};

vec3 color(const ray& r, const vector<Light>& lights, const vector<Sphere>& spheres, const vector<Triangle>& triangles, const vector<Cube>& cubes, const vector<Plane>& planes) {
    vec3 hit_color(0, 0, 0);
    float t_min = FLT_MAX;
    vec3 N;
    vec3 hit_point;

    for (const auto& sphere : spheres) {
        float t;
        if (hit_sphere(sphere, r, t, N) && t < t_min) {
            t_min = t;
            hit_point = r.point_at_parameter(t);
            hit_color = vec3(0, 0, 0);
            for (const auto& light : lights) {
                vec3 L = unit_vector(light.position - hit_point);
                float diffuse = max(dot(N, L), 0.0f);
                hit_color += light.intensity * diffuse;
            }
        }
    }

    for (const auto& tri : triangles) {
        float t;
        if (hit_triangle(tri, r, t, N) && t < t_min) {
            t_min = t;
            hit_point = r.point_at_parameter(t);
            hit_color = vec3(0, 0, 0);
            for (const auto& light : lights) {
                vec3 L = unit_vector(light.position - hit_point);
                float diffuse = max(dot(N, L), 0.0f);
                hit_color += light.intensity * diffuse;
            }
        }
    }

    for (const auto& cube : cubes) {
        float t;
        if (hit_cube(cube, r, t, N) && t < t_min) {
            t_min = t;
            hit_point = r.point_at_parameter(t);
            hit_color = vec3(0, 0, 0);
            for (const auto& light : lights) {
                vec3 L = unit_vector(light.position - hit_point);
                float diffuse = max(dot(N, L), 0.0f);
                hit_color += light.intensity * diffuse;
            }
        }
    }

    for (const auto& plane : planes) {
        float t;
        if (hit_plane(plane, r, t, N) && t < t_min) {
            t_min = t;
            hit_point = r.point_at_parameter(t);
            hit_color = vec3(0, 0, 0);
            for (const auto& light : lights) {
                vec3 L = unit_vector(light.position - hit_point);
                float diffuse = max(dot(N, L), 0.0f);
                hit_color += light.intensity * diffuse;
            }
        }
    }

    if (t_min < FLT_MAX) {
        return hit_color;
    }

    vec3 unit_direction = unit_vector(r.direction());
    float t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * vec3(1, 1, 1) + t * vec3(0.5, 0.7, 1.0);
}

int main() {
    int width = 800;
    int height = 800;

    fstream file;
    file.open("/home/nonohuang/CG/src/ray.ppm", ios::out);

    file << "P3\n" << width << " " << height << "\n255\n";
    vec3 lower_left_corner(-2, -1, -1);
    vec3 horizontal(4, 0, 0);
    vec3 vertical(0, 2, 0);
    vec3 origin(0, 0, 0);

    vector<Light> lights = {
        {vec3(-1, 1, 0), vec3(1, 0, 1)}, // purple light
        {vec3(2, 1, 0), vec3(1, 1, 0)} // yellow light
    };

    vector<Sphere> spheres = {
        {vec3(0, 0, -1), 0.5}
    };

    vector<Triangle> triangles = {
        {vec3(0.5, -0.25, -1), vec3(1.5, -0.25, -1), vec3(1, 0.25, -1)}
    };

    vector<Cube> cubes = {
        {vec3(-1, 0, -1.25), 0.5}
    };

    vector<Plane> planes = {
        {vec3(0, -0.5, -1), vec3(0, 1, 0)}
    };

    for (int j = height - 1; j >= 0; j--) {
        for (int i = 0; i < width; i++) {
            float u = float(i) / float(width);
            float v = float(j) / float(height);
            ray r(origin, lower_left_corner + (u * horizontal) + (v * vertical) - origin);
            vec3 col = color(r, lights, spheres, triangles, cubes, planes);
            file << int(255.99 * col.x()) << " "
                 << int(255.99 * col.y()) << " "
                 << int(255.99 * col.z()) << "\n";
        }
    }

    file.close();

    return 0;
}