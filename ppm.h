/*
    Gabriele Tomai 
    Student ID: IN2300006
    Degree Program: Computer Engineering
*/

#ifndef _PPM_H
#define _PPM_H

#include "scene.h"
typedef enum {
    PPM_ERR_NONE = 0,
    PPM_ERR_FILE_OPEN,
    PPM_ERR_FTRUNCATE,
    PPM_ERR_MMAP,
    PPM_ERR_MUNMAP
} error_t;

static inline const char* ppm_error_message(error_t err) {
    switch(err) {
        case PPM_ERR_NONE: return "File written successfully";
        case PPM_ERR_FILE_OPEN: return "Error: cannot open file";
        case PPM_ERR_FTRUNCATE: return "Error: cannot set file size";
        case PPM_ERR_MMAP: return "Error: cannot map file to memory";
        case PPM_ERR_MUNMAP: return "Error: cannot unmap memory";
        default: return "Unknown error";
    }
}

error_t write_ppm(const char *path, const pixel *image, int width, int height);

#endif