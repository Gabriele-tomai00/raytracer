/*
    Gabriele Tomai
    Student ID: IN2300006
    Degree Program: Computer Engineering
*/

#ifndef _SCENE_H
#define _SCENE_H

#include <stdio.h>
#include <stdint.h>

struct xyz_vector {
  float x, y, z;
};

struct __attribute__((packed)) rgb_pixel {
  uint8_t red, green, blue;
};

typedef struct rgb_pixel pixel;
typedef struct rgb_pixel *pixel_ptr;

typedef struct xyz_vector vector;
typedef struct xyz_vector *vector_ptr;

struct rgb_sphere {
  vector center;
  float radius;
  pixel color;
};

typedef struct rgb_sphere sphere;
typedef struct rgb_sphere *sphere_ptr;


struct scene {
  int sphere_count;
  pixel bg_color;
  sphere_ptr spheres;
  vector viewport_size;
};

typedef struct scene scene;
typedef struct scene *scene_ptr;


int read_scene_from_file(char *path, scene_ptr scene);
int render_image(scene_ptr scn, pixel_ptr framebuffer, int w, int h);

#endif