#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include "read_ppm.h"
#include "write_ppm.h"
#include <bits/getopt_core.h>

int main(int argc, char* argv[]) {
  int size = 480; // width and height of the mandelbrot image
  float xmin = -2.0;
  float xmax = 0.47;
  float ymin = -1.12;
  float ymax = 1.12;
  int maxIterations = 1000;

  time_t timestamp = time(0);
  char filename[50];
  snprintf(filename, sizeof(filename), "mandelbrot-%dx%d-%ld.ppm", size, size, timestamp);

  int opt;
  while ((opt = getopt(argc, argv, ":s:l:r:t:b:")) != -1) {
    switch (opt) {
      case 's': size = atoi(optarg); break;
      case 'l': xmin = atof(optarg); break;
      case 'r': xmax = atof(optarg); break;
      case 't': ymax = atof(optarg); break;
      case 'b': ymin = atof(optarg); break;
      case '?': printf("usage: %s -s <size> -l <xmin> -r <xmax> -b <ymin> -t <ymax>\n", argv[0]); break;
    }
  }
  printf("Generating mandelbrot with size %dx%d\n", size, size);
  printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
  printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);

  // todo: your work here
  // generate palette
  srand(time(0));
  unsigned char palette[256][3]; // 2D array to hold RGB color values
  for (int i = 0; i < 256; i++) {
    unsigned char color = (unsigned char)i;
    palette[i][0] = i; // Red channel
    palette[i][1] = i; // Green channel
    palette[i][2] = i; // Blue channel 
  }

  // compute image
  FILE *fp = fopen(filename, "w");
  if (!fp) {
    fprintf(stderr, "Error: Could not open file for writing\n");
    return 1;
  }

  for (int row = 0; row < size; row++) {
    for (int col = 0; col < size; col++){
      double xfrac = (double) row / size;
      double yfrac = (double) col / size;
      double x0 = xmin + xfrac * (xmax - xmin);
      double y0 = ymin + yfrac * (ymax - ymin);

      int x = 0;
      int y = 0;
      int iter = 0;

      while(iter < maxIterations && x*x + y*y < 4.0) {
          double xtmp = x*x - y*y + x0;
          y = 2.0 * x * y + y0;
          x = xtmp;
          iter++;
      }

       struct ppm_pixel pixel;

      if (iter < maxIterations){  // escaped
        int index = rand() % 255; // randomly picks and index in the palette
        pixel->red = palette[index][0];
        pixel->green = palette[index][1];
        pixel->blue = palette[index][2];
      }
      else {
        // not escaped, color is black
        pixel->red = 0;
        pixel->green = 0;
        pixel->blue = 0;
      }

      // writing RGB values to the PPM file
      fprintf(fp, "%d %d %d ", red, green, blue);
      
      if ((col + 1) % 10 == 0) {
        fprintf(fp, "\n");
      }

    }
    fprintf(fp, "\n");
  }

  fclose(fp);

  float compTime; // Num seconds needed to compute the image

  // Output
  printf("Computed mandelbrot set (480x480) in %f seconds\n", compTime);
  printf("Writing file: '%s'\n", filename); 
}
