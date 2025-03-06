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

struct Triangle {
    vec3 v0, v1, v2;
};

struct Cube {
    vec3 center;
    float side_length;
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
    // At this stage we can compute t to find out where the intersection point is on the line.
    t = f * dot(edge2, q);
    if (t > EPSILON) { // ray intersection
        N = unit_vector(cross(edge1, edge2));
        return true;
    }
    else // This means that there is a line intersection but not a ray intersection.
        return false;
}

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

float hit_sphere(const vec3 &center, float radius, const ray& r) {
    vec3 objectCenter = r.origin() - center;
    float a = dot(r.direction(), r.direction());
    float b = 2.0 * dot(objectCenter, r.direction());
    float c = dot(objectCenter, objectCenter) - radius * radius;
    float discriminant = b * b - 4 * a * c;
    if (discriminant < 0)  return -1.0;
    else return (-b - sqrt(discriminant)) / (2.0 * a);
}

vec3 color(const ray& r, const vector<Light>& lights, const vector<Triangle>& triangles, const vector<Cube>& cubes) {
    vec3 center(0, 0, -1);
    float t = hit_sphere(center, 0.2, r);

    vec3 hit_color(0, 0, 0);
    if (t > 0.0) {
        vec3 N = unit_vector(r.point_at_parameter(t) - center);
        for (const auto& light : lights) {
            vec3 L = unit_vector(light.position - r.point_at_parameter(t));
            float diffuse = max(dot(N, L), 0.0f);
            hit_color += light.intensity * diffuse;
        }
    }

    for (const auto& tri : triangles) {
        vec3 N;
        float t_tri;
        if (hit_triangle(tri, r, t_tri, N)) {
            vec3 tri_color(0, 0, 0);
            for (const auto& light : lights) {
                vec3 L = unit_vector(light.position - r.point_at_parameter(t_tri));
                float diffuse = max(dot(N, L), 0.0f);
                tri_color += light.intensity * diffuse;
            }
            if (t_tri < t || t < 0.0) {
                t = t_tri;
                hit_color = tri_color;
            }
        }
    }

    for (const auto& cube : cubes) {
        vec3 N;
        float t_cube;
        if (hit_cube(cube, r, t_cube, N)) {
            vec3 cube_color(0, 0, 0);
            for (const auto& light : lights) {
                vec3 L = unit_vector(light.position - r.point_at_parameter(t_cube));
                float diffuse = max(dot(N, L), 0.0f);
                cube_color += light.intensity * diffuse;
            }
            if (t_cube < t || t < 0.0) {
                t = t_cube;
                hit_color = cube_color;
            }
        }
    }

    if (t > 0.0) {
        return hit_color;
    }

    vec3 unit_direction = unit_vector(r.direction());
    t = 0.5 * (unit_direction.y() + 1.0);
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

    vector<Triangle> triangles = {
        {vec3(0.5, -0.25, -1), vec3(1.5, -0.25, -1), vec3(1, 0.25, -1)}
    };

    vector<Cube> cubes = {
        {vec3(-1, 0, -1.25), 0.5}
    };

    for (int j = height - 1; j >= 0; j--) {
        for (int i = 0; i < width; i++) {
            float u = float(i) / float(width);
            float v = float(j) / float(height);
            ray r(origin, lower_left_corner + (u * horizontal) + (v * vertical) - origin);
            vec3 col = color(r, lights, triangles, cubes);
            file << int(255.99 * col.x()) << " "
                 << int(255.99 * col.y()) << " "
                 << int(255.99 * col.z()) << "\n";
        }
    }

    file.close();

    return 0;
}