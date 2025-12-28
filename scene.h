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

typedef enum {
    SCENE_ERR_NONE = 0,
    SCENE_ERR_FILE_OPEN,
    SCENE_ERR_VIEWPORT,
    SCENE_ERR_BACKGROUND,
    SCENE_ERR_OBJECT_COUNT,
    SCENE_ERR_MEMORY,
    SCENE_ERR_SPHERE_MALFORMED,
    SCENE_ERR_SPHERE_INVALID,
    SCENE_ERR_NO_VALID_SPHERES
} scene_error_t;

static inline const char* scene_error_message(scene_error_t err) {
    switch (err) {
        case SCENE_ERR_NONE:               return "Scene loaded successfully";
        case SCENE_ERR_FILE_OPEN:          return "Error: Cannot open scene file";
        case SCENE_ERR_VIEWPORT:           return "Error: Malformed viewport header";
        case SCENE_ERR_BACKGROUND:         return "Error: Malformed background color header";
        case SCENE_ERR_OBJECT_COUNT:       return "Error: Malformed object count header";
        case SCENE_ERR_MEMORY:             return "Error: Memory allocation failed for spheres";
        case SCENE_ERR_SPHERE_MALFORMED:   return "Error: Malformed sphere definition";
        case SCENE_ERR_SPHERE_INVALID:     return "Error: Sphere has invalid radius or color";
        case SCENE_ERR_NO_VALID_SPHERES:   return "Error: No valid spheres found in scene file";
        default:                           return "Unknown scene error";
    }
}

scene_error_t read_scene_from_file(char *path, scene_ptr scene);
int render_image(scene_ptr scn, pixel_ptr framebuffer, int w, int h);

#endif