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

/*
    Writes a PPM image to disk using memory mapping.
    The image buffer is copied directly after the PPM header.

    @param output_path  output file path
    @param pixels       pointer to the pixel buffer
    @param width        image width in pixels
    @param height       image height in pixels

    @return error_t: ERR_NONE on success, specific error code on failure
 */

error_t write_ppm(const char *output_path, const pixel *pixels, int width, int height) {

    // wb+ => open for reading and writing in binary mode; create file if it does not exist
    FILE *fp = fopen(output_path, "wb+");
    if (fp == NULL) {
        return PPM_ERR_FILE_OPEN;
    }

    // calculate header length and total file size
    // sprintf returns the number of characters that would have been written
    int header_len = snprintf(NULL, 0, "P6\n%d %d\n255\n", width, height);
    size_t pixel_bytes = (size_t)width * height * 3;
    size_t file_size = header_len + pixel_bytes;

    int fd = fileno(fp);
    if (ftruncate(fd, file_size) == -1) {
        fclose(fp);
        return PPM_ERR_FTRUNCATE;
    }

    // NULL lets the kernel choose the address; file_size bytes are mapped.
    // MAP_SHARED propagates writes to the underlying file; fd+offset=0 sets the mapping start.
    unsigned char *mapped = mmap(NULL, file_size,
                                 PROT_WRITE | PROT_READ,
                                 MAP_SHARED, fd, 0);
    if (mapped == MAP_FAILED) {
        fclose(fp);
        return PPM_ERR_MMAP;
    }

    // Write PPM header directly into the mapped region (includes null terminator for safety).
    // snprintf bounds: [0 .. header_len], calculated previously to avoid overflow.
    snprintf((char *)mapped, header_len + 1, "P6\n%d %d\n255\n", width, height);
    
    // Copy pixel buffer immediately after the header in memory-mapped file.
    // mapped + header_len points to the first byte of image data.
    memcpy(mapped + header_len, pixels, pixel_bytes);

    if (munmap(mapped, file_size) == -1) {
        fclose(fp);
        return PPM_ERR_MUNMAP;
    }

    fclose(fp);
    return PPM_ERR_NONE;
}