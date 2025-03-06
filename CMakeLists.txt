cmake_minimum_required(VERSION 3.0.0)
project(my_CG_project VERSION 0.1.0)
cmake_policy(SET CMP0072 NEW)

find_package(OpenGL REQUIRED)

add_executable(my_CG_project 
    src/hw1.cpp
    src/glad.h
    src/glad.c
    src/vec3.h
    src/khrplatform.h
    src/ray.h
    src/Sphere.h
    src/Plane.h
    src/Triangle.h
    src/Cube.h
)

target_link_libraries(my_CG_project 
    glfw
    OpenGL::GL
)