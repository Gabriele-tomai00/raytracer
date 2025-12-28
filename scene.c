/*
    Gabriele Tomai
    Student ID: IN2300006
    Degree Program: Computer Engineering
*/

#include "scene.h"
#include <stdlib.h>
#include <math.h>

scene_error_t read_scene_from_file(char *scene_file, scene_ptr scene) {
    FILE *fd = fopen(scene_file, "r");
    if (!fd) {
        return SCENE_ERR_FILE_OPEN;
    }

    // read viewport
    if (fscanf(fd, "VP %f %f %f\n",
               &scene->viewport_size.x,
               &scene->viewport_size.y,
               &scene->viewport_size.z) != 3) {
        fclose(fd);
        return SCENE_ERR_VIEWPORT;
    }

    // read background color
    if (fscanf(fd, "BG %hhu %hhu %hhu\n",
               &scene->bg_color.red,
               &scene->bg_color.green,
               &scene->bg_color.blue) != 3) {
        fclose(fd);
        return SCENE_ERR_BACKGROUND;
    }

    // read object count
    if (fscanf(fd, "OBJ_N %d\n", &scene->sphere_count) != 1) {
        fclose(fd);
        return SCENE_ERR_OBJECT_COUNT;
    }

    // memory allocation for spheres
    scene->spheres = malloc(scene->sphere_count * sizeof(struct rgb_sphere));
    if (!scene->spheres) {
        fclose(fd);
        return SCENE_ERR_MEMORY;
    }

    // read and validate spheres (fail-fast)
    for (int i = 0; i < scene->sphere_count; i++) {
        struct rgb_sphere *sphere = &scene->spheres[i];

        if (fscanf(fd, "S %f %f %f %f %hhu %hhu %hhu\n",
                   &sphere->center.x,
                   &sphere->center.y,
                   &sphere->center.z,
                   &sphere->radius,
                   &sphere->color.red,
                   &sphere->color.green,
                   &sphere->color.blue) != 7) {
            free(scene->spheres);
            fclose(fd);
            return SCENE_ERR_SPHERE_MALFORMED;
        }

        if (sphere->radius <= 0.0f ||
            sphere->color.red   > 255 ||
            sphere->color.red   < 0 ||
            sphere->color.green > 255 ||
            sphere->color.green < 0 ||
            sphere->color.blue  > 255 ||
            sphere->color.blue  < 0) {
            free(scene->spheres);
            fclose(fd);
            return SCENE_ERR_SPHERE_INVALID;
        }
    }

    fclose(fd);
    return SCENE_ERR_NONE;
}



// Dot product between two vectors
static inline float dot_product(vector v1, vector v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

static inline vector normalize_vector(vector v) {
    float norm = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    if (norm == 0.0f) {
        // fallback: return zero vector
        return (vector){0.0f, 0.0f, 0.0f};
    }
    float inv_norm = 1.0f / norm;
    return (vector){v.x * inv_norm, v.y * inv_norm, v.z * inv_norm};
}

int render_image(scene_ptr scn, pixel_ptr framebuffer, int w, int h) {

    vector vp_size = scn->viewport_size;
    float dx = vp_size.x / (w - 1);
    float dy = vp_size.y / (h - 1);

    for (int y = 0; y < h; y++) {
        for (int i = 0; i < w; i++) {

            vector ray_vec = {
                i * dx - vp_size.x * 0.5f,
                y * dy - vp_size.y * 0.5f,
                vp_size.z
            };

            vector dir = normalize_vector(ray_vec);

            float nearest_t = INFINITY;
            // pointer to the closest hit sphere
            const sphere *hit = NULL;

            for (int s = 0; s < scn->sphere_count; ++s) {
                const sphere *sp = &scn->spheres[s];

                // compute ray-sphere intersection
                float a = 1.0f; // since dir is normalized
                float b = -2.0f * dot_product(sp->center, dir);
                float c = dot_product(sp->center, sp->center) - sp->radius * sp->radius;

                float delta = b * b - 4.0f * a * c;
                if (delta < 0.0f) continue;

                float sqrt_delta = sqrt(delta);
                // check both intersection points and take the nearest positive t
                float t1 = (-b - sqrt_delta) / (2.0f * a);
                float t2 = (-b + sqrt_delta) / (2.0f * a);

                // scegli il più piccolo t positivo
                float t = INFINITY;
                if (t1 > 0.0f) t = t1;
                if (t2 > 0.0f && t2 < t) t = t2;


                if (t < nearest_t) {
                    nearest_t = t;
                    hit = sp;
                }
            }

            // set pixel color based on intersection, or background color if no hit
            int idx = (h - 1 - y) * w + i;
            framebuffer[idx] = (hit != NULL) ? hit->color : scn->bg_color;
        }
    }

    return 0;
}
