# Raytracer

![Raytracing img](img_readme/raytracing.png)

implementation in C of a simple raytracer for the 'advanced and parallel programming' exam

# Description

## Components
1. Camrea: will be located at coordinates (0,0,0)
2. Viewpoint: a rectangle perpendicular to the z-axis (the z-axis passes through its center)
3. Props: they are spheres of center C (x<sub>s</sub>, y<sub>s</sub>, z<sub>s</sub>) and radius r

## Compilation
You need to compile the program with the following command:
```bash
make
```

## Execution
To execute the program run the following command:
```bash
./main
```
or with the custom parameters (./main scene-file  img-name  img-width-px  img-height-px).
For exemple:
```bash
./main scene.txt output.ppm 800 600
```