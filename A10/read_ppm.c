#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_ppm.h"

struct ppm_pixel* read_ppm(const char* filename, int* w, int* h) {
  // Filename is taken as input
  FILE *infile = fopen(filename, "r");
  char buffer[100];
  int maxValue; // 255

  if (infile == NULL)
  {
	  printf("Invalid file\n");
	  exit(1);
  }

  // Skips headers
  fgets(buffer, sizeof(buffer), infile); // gimp 
  fgets(buffer, sizeof(buffer), infile); // plug-in

  fscanf(infile, "%d %d\n %d\n", w, h, &maxValue); // Reads data from stream

  // Pixel struct which is the size of w and h
  struct ppm_pixel* pixel = malloc(sizeof(struct ppm_pixel) * (*w * *h));

  if (pixel == NULL) // If there is no space, user is responsible for freeing 
  {
	  printf("Memory cannot be allocated.\n");
	  exit(1);
  }

  // Reads and gets the content
  fread(pixel, sizeof(struct ppm_pixel), (*w * *h), infile);

  fclose(infile);
  return pixel;
}

struct ppm_pixel** read_ppm_2d(const char* filename, int* w, int* h) {

  return NULL;
}
