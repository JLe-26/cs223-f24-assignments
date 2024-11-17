#include "write_ppm.h"
#include <stdio.h>
#include <string.h>

void write_ppm(const char* filename, struct ppm_pixel* pixels, int w, int h) {
  FILE *outfile;
  outfile = fopen(filename, "w"); // Writing into the new file

  if (!outfile)
  {
	  printf("Error: Unable to open file! %s\n", filename);
	  return;
  }

  fprintf(outfile, "P6.\n #Created by Jenny Le ver\n%d %d", w, h); // For header
  // Put binary data into a new file; writing it
  for (int i = 0; i < w; i++)
  {
	  for (int j = 0; j < h; j++)
	  {
		  unsigned char pixelBlock[3] = {pixels[i * h + j].red,
			  pixels[i * h + j].green, pixels[i * h + j].blue};
		  fwrite(pixelBlock, sizeof(unsigned char), 3, outfile); // Copy
	  }
  }
  fclose(outfile); // Closes file    
}

void write_ppm_2d(const char* filename, struct ppm_pixel** pxs, int w, int h) {
}
