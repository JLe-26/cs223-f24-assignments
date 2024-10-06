/*----------------------------------------------
 * Author: Jenny Le 
 * Date: October 5, 2024
 * Description: This test reads in feep_raw.ppm
 * and writes it to a file test.ppm by calling write_ppm.c
 ---------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "read_ppm.h"
#include "write_ppm.h"

int main(int argc, char** argv) {
  int w, h;
  struct ppm_pixel* pixels = read_ppm("feep-raw.ppm", &w, &h);

  // Test writing the file to test.ppm, reload it, and print the contents
  write_ppm("test.ppm", pixels, w, h);
  free(pixels); // Need to free before next read

  pixels = read_ppm("test.ppm", &w, &h); // Reads and assigns it to pixels

  printf("Testing file feep-raw.ppm: 4 4\n");
  // Print
  for (int i = 0; i < w; i++)
  {
	  for (int j = 0; j < h; j++)
	  {
		  printf("(%d, %d, %d) ", pixels[i * h + j].red, pixels[i * h + j].green, pixels[i * h + j].blue);
	  }
	  printf("\n");
  }
  
  free(pixels); // Free memory
  return 0;
}
