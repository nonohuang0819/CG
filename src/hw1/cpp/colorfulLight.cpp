#include <iostream>
#include <fstream>
#include <vector>
#include "vec3.h"
#include "ray.h"
using namespace std;

struct Light {
    vec3 position;
    vec3 intensity;
};

float hit_sphere(const vec3 &center, float radius, const ray& r) {
    vec3 objectCenter = r.origin() - center;
    float a = dot(r.direction(), r.direction());
    float b = 2.0 * dot(objectCenter, r.direction());
    float c = dot(objectCenter, objectCenter) - radius * radius;
    float discriminant = b * b - 4 * a * c;
    if (discriminant < 0)  return -1.0;
    else return (-b - sqrt(discriminant)) / (2.0 * a);
    
}

vec3 color(const ray& r, const vector<Light>& lights) {
    vec3 center(0, 0, -1);
    float t = hit_sphere(center, 0.5, r);

    if (t > 0.0) {
        vec3 N = unit_vector(r.point_at_parameter(t) - center);
        vec3 color(0, 0, 0);
        for (const auto& light : lights) {
            vec3 L = unit_vector(light.position - r.point_at_parameter(t));
            float diffuse = max(dot(N, L), 0.0f);
            color += light.intensity * diffuse;
        }
        return color;
    }
    vec3 unit_direction = unit_vector(r.direction());
    t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * vec3(1, 1, 1) + t * vec3(0.5, 0.7, 1.0);
}

int main() {
    int width = 200;
    int height = 100;

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

    for (int j = height - 1; j >= 0; j--) {
        for (int i = 0; i < width; i++) {
            float u = float(i) / float(width);
            float v = float(j) / float(height);
            ray r(origin, lower_left_corner + (u * horizontal) + (v * vertical) - origin);
            vec3 col = color(r, lights);
            file << int(255.99 * col.x()) << " "
                 << int(255.99 * col.y()) << " "
                 << int(255.99 * col.z()) << "\n";
        }
    }

    file.close();

    return 0;
}