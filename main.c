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


/*
 * Writes a P6 PPM image to disk using memory mapping.
 * The image buffer is copied directly after the PPM header.
 *
 * Optional parameters:
 * @param scene_filename    input scene file path
 * @param output_filename   output image file path
 * @param viewpoint_width   image width in pixels
 * @param viewpoint_height  image height in pixels
 *
 * @return 0 on success, 1 on failure
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

  // rises an error if number of parameters is wrong
  if (argc != 5 && argc != 1) {
    fprintf(stderr, "Error while reading parameters.\n If you don't want to use the default "
                    "'./main' "
                    "use the following format: './main <scene-file.txt> "
                    "<img-name.ppm> <img-width> <img-height>'\n");
    return 1;
  }

  // file neme of the scene to be rendered and output image file name
  char *scene_filename = "resources/test.txt";
  char *output_filename = "results/image.ppm";

  // default parameter values of viewpoint resolution in piexels
  int viewpoint_width = 1920;
  int viewpoint_height = 1080;

  // overwrite parameters
  if (argc == 5) {
    scene_filename = argv[1];
    output_filename = argv[2];
    viewpoint_width = atoi(argv[3]);
    viewpoint_height = atoi(argv[4]);
  } else {
    printf("Number of paramters not correct. Using default parameters\n");
  }

  printf("%s", art);







  
  printf("Execution completed\n");

  return 0;
}