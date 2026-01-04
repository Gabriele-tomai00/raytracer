/*
    Gabriele Tomai
    Student ID: IN2300006
    Degree Program: Computer Engineering
*/

#include "scene.h"
#include <stdlib.h>
#include <math.h>

/*
    Function: read_scene_from_file
    Reads a scene description from a text file and initializes a scene structure.
    The function parses the scene header (viewport, background color, object count) 
    and loads all valid sphere definitions. Malformed or invalid spheres are skipped.
    At least one valid sphere must be present for the function to succeed.

    @param scene_file Path to the scene configuration file
    @param scene Pointer to the scene structure to initialize
    @return 0 on success, 1 on critical failure

    FILE FORMAT (exemple):
        VP 1.777 1 1
        BG 255 255 255
        OBJ_N 16
        S 0.2 1 4 0.2 200 0 0
        S 0.2 1.5 5 0.4 150 0 0
        ...
*/

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
               &scene->background_color.red,
               &scene->background_color.green,
               &scene->background_color.blue) != 3) {
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
        if (fscanf(fd, "S %f %f %f %f %hhu %hhu %hhu\n",
            &scene->spheres[i].center.x,
            &scene->spheres[i].center.y,
            &scene->spheres[i].center.z,
            &scene->spheres[i].radius,
            &scene->spheres[i].color.red,
            &scene->spheres[i].color.green,
            &scene->spheres[i].color.blue) != 7) {
            free(scene->spheres);
            fclose(fd);
            return SCENE_ERR_SPHERE_MALFORMED;
        }

        // check sphere validity: radius > 0, color components in [0,255]
        if (scene->spheres[i].radius <= 0.0f ||
            scene->spheres[i].color.red > 255 ||
            scene->spheres[i].color.green > 255 ||
            scene->spheres[i].color.blue > 255)
        {
            free(scene->spheres);
            fclose(fd);
            return SCENE_ERR_SPHERE_INVALID;
        }

    }

    fclose(fd);
    return SCENE_ERR_NONE;
}

static inline float dot_product(vector v1, vector v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

/*
    Vector normalization using multiplication by inverse length
    @param v    input vector
    @return     unit-length version of the input vector
*/ 
static inline vector normalize_vector(vector v) {
    float norm = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    if (norm == 0.0f) {
        // fallback: return zero vector
        return (vector){0.0f, 0.0f, 0.0f};
    }
    float inv_norm = 1.0f / norm;
    return (vector){v.x * inv_norm, v.y * inv_norm, v.z * inv_norm};
}

/*
    Renders the scene into a pixel buffer using ray–sphere intersection.
    For each pixel, a ray is cast through the viewport and the closest
    intersected sphere determines the pixel color; if no intersection
    occurs, the background color is used.

    @param scn         - pointer to the scene description
    @param framebuffer - pointer to the output pixel array
    @param w           - image width in pixels
    @param h           - image height in pixels
    @return 0 on successful completion.
 */

int render_img(scene_ptr scn, pixel_ptr framebuffer, int w, int h) {

    vector vp_size = scn->viewport_size;
    // precompute pixel size in viewport
    float dx = vp_size.x / (w - 1);
    float dy = vp_size.y / (h - 1);

#pragma omp parallel for collapse(2)
    for (int y = 0; y < h; y++) {
        for (int i = 0; i < w; i++) {
            // compute ray direction for pixel (i, y)
            vector ray_vec = {
                i * dx - vp_size.x * 0.5f,
                y * dy - vp_size.y * 0.5f,
                vp_size.z
            };

            vector direction = normalize_vector(ray_vec);

            // hit: valid intersection along the ray
            float closest_hit_t = INFINITY;
            const sphere *closest_sphere = NULL;

            // test intersection with all spheres
            for (int s = 0; s < scn->sphere_count; ++s) {
                const sphere *sp = &scn->spheres[s];

                // compute ray-sphere intersection
                // a b c coefficients of the quadratic equation
                float a = 1.0f; // direction is normalized
                float b = -2.0f * dot_product(sp->center, direction);
                float c = dot_product(sp->center, sp->center) - sp->radius * sp->radius;

                float delta = b * b - 4.0f * a * c;
                // no intersection if delta < 0
                if (delta < 0.0f) continue;

                float discriminant_sqrt = sqrt(delta);

                // t: real distance along the ray
                // intersection points along the ray
                float t_entry = (-b - discriminant_sqrt) / (2.0f * a);
                float t_exit  = (-b + discriminant_sqrt) / (2.0f * a);

                // find the nearest positive intersection
                float sphere_hit_t = INFINITY;
                if (t_entry > 0.0f) 
                    sphere_hit_t = t_entry;
                if (t_exit > 0.0f && t_exit < sphere_hit_t) 
                    sphere_hit_t = t_exit;

                // update the closest hit sphere along the ray
                if (sphere_hit_t < closest_hit_t) {
                    closest_hit_t = sphere_hit_t;
                    closest_sphere = sp;
                }
            }

                // set pixel color based on intersection, or background color if no hit
                int idx = (h - 1 - y) * w + i;
                framebuffer[idx] = (closest_sphere != NULL) ? closest_sphere->color : scn->background_color;
        }
    }

    return 0;
}
