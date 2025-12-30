/*
    Gabriele Tomai 
    Student ID: IN2300006
    Degree Program: Computer Engineering
*/

#include "ppm.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

error_t write_ppm(const char *path, const pixel *pixels, int width, int height) {

    FILE *fp = fopen(path, "w+");
    if (fp == NULL) {
        return PPM_ERR_FILE_OPEN;  // failed to open file
    }

    int fd = fileno(fp);

    int header_len = snprintf(NULL, 0, "P6\n%d %d\n255\n", width, height);
    size_t pixel_bytes = (size_t)width * height * 3;
    size_t file_size = header_len + pixel_bytes;

    if (ftruncate(fd, file_size) == -1) {
        fclose(fp);
        return PPM_ERR_FTRUNCATE; // error setting file size
    }

    unsigned char *mapped = mmap(NULL, file_size,
                                 PROT_WRITE | PROT_READ,
                                 MAP_SHARED, fd, 0);
    if (mapped == MAP_FAILED) {
        fclose(fp);
        return PPM_ERR_MMAP;       // error mapping file to memory
    }

    snprintf((char *)mapped, header_len + 1, "P6\n%d %d\n255\n", width, height);
    memcpy(mapped + header_len, pixels, pixel_bytes);

    if (munmap(mapped, file_size) == -1) {
        fclose(fp);
        return PPM_ERR_MUNMAP;     // error unmapping memory
    }

    fclose(fp);
    return PPM_ERR_NONE;
}