/*----------------------------------------------
 * Author: Jenny Le 
 * Date: October 5, 2024
 * Description: This test calls read_ppm and prints
 * the contents of feep-raw.ppm.
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include "read_ppm.h"

int main() {
  int w, h;
  struct ppm_pixel* pixels = read_ppm("feep-raw.ppm", &w, &h);
  
  printf("Testing file feep-raw.ppm: 4 4\n");

  // Prints out the pixels
  for (int i = 0; i < w; i++)
  {
	  for (int j = 0; j < h; j++)
	  {
		  printf("(%d, %d, %d) ", pixels[i * h + j].red, pixels[i * h + j].green, pixels[i * h + j].blue);
	  }
	  printf("\n");
  }

  free(pixels); // Frees data
  return 0;
}

