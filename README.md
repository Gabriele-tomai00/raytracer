# Raytracer

**Project for the "Advanced and Parallel Programming" exam**  
Author: Gabriele Tomai – Student ID: IN2300006 – Computer Engineering

![Raytracing image](img_readme/raytracing.png)

Implementation in C of a simple ray tracer.  
- Input: a .txt file representing a scene containing a declared number of colored spheres  
- Output: render the corresponding image in .ppm format

# Description

## Components
1. Camera: located at coordinates (0, 0, 0)  
2. Viewport: a rectangle perpendicular to the z-axis (the z-axis passes through its center)  
3. Spheres: spheres with center \(C=(x_s, y_s, z_s)\) and radius \(r\)  

## Ray tracing algorithm

For each pixel, a primary ray is generated from the camera origin through the corresponding point on the viewport. The ray is tested against all spheres in the scene by solving the ray–sphere intersection equation. If multiple intersections occur, the closest positive intersection along the ray is selected. If no intersection is found, the background color is used.


## Input file: .txt
There is an example file in the `/resources` folder.  
The file follows this format:
```
VP 1.777 1 1
BG 255 255 255
OBJ_N 16
S 0.2 1 4 0.2 200 0 0
...
```
- VP float float float indicates the viewport  
- BG uint8_t uint8_t uint8_t indicates the background color of the scene  
- OBJ_N int indicates the number of objects to follow (we only deal with spheres)  
`S float float float float uint8_t uint8_t uint8_t` defines a sphere where:  
    - The first three float values are the coordinates \((x, y, z)\)  
    - The fourth float value is the sphere radius  
    - The three uint8_t values indicate the RGB color of the sphere

## Output file: .ppm
Extremely simple format for handling colors. The format used in this project is a binary format. The general structure is:
```
P6
width height
255
[binary data]
```
- P6 is the format; width and height refer to the image in pixels; 255 is the maximum intensity value for each color channel.

The PPM file is written using memory-mapped I/O (mmap).

## Parallelization
The rendering loop is parallelized using OpenMP. Pixel computation is independent, therefore a parallel for with loop collapse is used to distribute the workload across multiple threads.

---

# Compilation

## Requirements

- GCC with OpenMP support
- POSIX-compliant system (Linux/macOS)

Compile the program with: `make`
## Execution
Run the program with: `./main` 
(in this case the input is `resources/test.txt` and the ouput is `results/image.ppm`)

Or run with custom parameters (./main scene-file img-name img-width-px img-height-px). For example:
`./main scene.txt output.ppm 800 600`

## Conclusion and check
The `/results` folder contains the file `reference.ppm` (1920x1080 image not generated with this program).
It is therefore possible to use this program to generate a new image from the same test.txt file (`./main ./resources/test.txt ./results/test.ppm 1920 1080`) and verify that the two images are identical.


