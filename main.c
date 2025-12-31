/*
    Gabriele Tomai
    Student ID: IN2300006
    Degree Program: Computer Engineering
*/
#include "scene.h"
#include "ppm.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>


/*
    Prints help message for using the program
*/
void print_help() {
    printf("Usage: ./main [scene_file output_image width height]\n");
    printf("\nOptional arguments:\n");
    printf("  scene_file     Input scene file path (default: resources/test.txt)\n");
    printf("  output_image   Output PPM image file path (default: results/image.ppm)\n");
    printf("  width          Image width in pixels (default: 1920)\n");
    printf("  height         Image height in pixels (default: 1080)\n");
    printf("\nExample:\n");
    printf("  ./main scene.txt image.ppm 1280 720\n");
    printf("\nUse --help to display this message\n");
}


/*
  Writes a P6 PPM image to disk using memory mapping.
  The image buffer is copied directly after the PPM header.

  Optional parameters:
  @param scene_filename    input scene file path
  @param output_filename   output image file path
  @param viewpoint_width   image width in pixels
  @param viewpoint_height  image height in pixels
  @return 0 on success, 1 on failure
 */

int main(int argc, char *argv[]) {

  const char *art = "  _____             _                           \n"
                    " |  __ \\           | |                          \n"
                    " | |__) |__ _ _   _| |_ _ __ __ _  ___ ___ _ __ \n"
                    " |  _  // _` | | | | __| '__/ _` |/ __/ _ \\ '__|\n"
                    " | | \\ \\ (_| | |_| | |_| | | (_| | (_|  __/ |   \n"
                    " |_|  \\_\\__,_|\\__, |\\__|_|  \\__,_|\\___\\___|_|   \n"
                    "               __/ |                            \n"
                    "              |___/                             \n\n";

  // handle --help
  if (argc == 2 && (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0)) {
      print_help();
      return 0;
  }

  // rises an error if number of parameters is wrong
  if (argc != 5 && argc != 1) {
    fprintf(stderr, "Error while reading parameters.\n If you don't want to use the default "
                    "'./main' "
                    "use the following format: './main <input-scene-file.txt> "
                    "<output-img-name.ppm> <img-width> <img-height>'\n");
    return 1;
  }

  // default file name of the scene to be rendered and output image file name
  char *scene_filename = "resources/test.txt";
  char *output_filename = "results/image.ppm";

  // default parameter values of viewpoint resolution in piexels
  int viewpoint_width = 1920;
  int viewpoint_height = 1080;

  // overwrite parameters
  if (argc == 5) {
    scene_filename = argv[1];
    output_filename = argv[2];
    // atoi parses string to integer
    viewpoint_width = atoi(argv[3]);
    viewpoint_height = atoi(argv[4]);
    // if width or height are not positive integers, atoi returns 0, so we raise an error
    if (viewpoint_width <= 0 || viewpoint_height <= 0) {
      printf("Width and height must be positive integers\n");
      return 1;
    }
  } else {
    printf("Number of paramters not correct. Using default parameters\n");
  }

  printf("%s", art);
  printf("- Scene: %s\n", scene_filename);
  printf("- Output: %s\n", output_filename);
  printf("- Resolution: %dx%d px\n", viewpoint_width, viewpoint_height);

  scene sc;
  pixel_ptr image = NULL;

  // load scene
  scene_error_t serr = read_scene_from_file(scene_filename, &sc);
  if (serr != SCENE_ERR_NONE) {
      fprintf(stderr, "%s\n", scene_error_message(serr));
      return 1;
  }

  // allocate pixel buffer
  image = malloc((size_t)viewpoint_width * viewpoint_height * sizeof(*image));
  if (!image) {
      fprintf(stderr, "Memory allocation failed\n");
      return 1;
  }

  // render
  if (render_img(&sc, image, viewpoint_width, viewpoint_height) != 0) {
      fprintf(stderr, "Error rendering image\n");
      free(image);
  }

  // save image
  error_t err = write_ppm(output_filename, image, viewpoint_width, viewpoint_height);
  if (err != PPM_ERR_NONE) {
      fprintf(stderr, "%s\n", ppm_error_message(err));
      free(image);
      return 1;
  }

  puts("Execution completed");
  free(image);

  return 0;
}