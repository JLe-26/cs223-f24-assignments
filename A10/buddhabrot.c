#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <pthread.h>
#include "read_ppm.h"
#include "write_ppm.h"
#include <bits/getopt_core.h>
#include <sys/time.h>

// Global variables
int size = 480;
float xmin = -2.0;
float xmax = 0.47;
float ymin = -1.12;
float ymax = 1.12;
int maxIterations = 1000;
int numProcesses = 4;

int **membership; // Array to store Mandelbrot membership
int **count; // Visit counts
struct ppm_pixel **image;
int max_count = 0; // Max count for color scaling

pthread_mutex_t count_mutex;
pthread_barrier_t barrier;

void *start(void *data) {
  int thread_id = *(int*)data;
  int rows_per_thread = size / 2;
  int cols_per_thread = size / 2;

  int start_row = (thread_id / 2) * rows_per_thread;
  int end_row = start_row + rows_per_thread;
  int start_col = (thread_id % 2) * cols_per_thread;
  int end_col = start_col + cols_per_thread;

  // determine mandelbrot membership
  for (int row = 0; row < size; row++) {
    for (int col = 0; col < size; col++){
      double xfrac = (double) row / size;
      double yfrac = (double) col / size;
      double x0 = xmin + xfrac * (xmax - xmin);
      double y0 = ymin + yfrac * (ymax - ymin);

      double x = 0;
      double y = 0;
      int iter = 0;

      while(iter < maxIterations && x*x + y*y < 2*2) {
          double xtmp = x*x - y*y + x0;
          y = 2.0 * x * y + y0;
          x = xtmp;
          iter++;
      }

      if (iter < maxIterations) { // escaped
        membership[row][col] = 0; // Point escaped, not in Mandelbrot
      }
      else {
        membership[row][col] = 1; // Point not escaped, in Mandelbrot set
      }
    }
  }

  // compute visited counts
  for (int row = start_row; row < end_row; row++) {
    for (int col = start_col; col < end_col; col++) {
        if (membership[row][col] == 1){
          continue; // point is in Mandelbrot set, skip to next point
        }

        double xfrac = (double)row / size;
        double yfrac = (double)col / size;
        double x0 = xmin + xfrac * (xmax-xmin);
        double y0 = ymin + yfrac * (ymax - ymin);

        double x = 0.0, y = 0.0, xtmp;
        while (x*x + y*y < 2*2) {
          xtmp = x*x - y*y + x0;
          y = 2*x*y + y0;
          x = xtmp;
          int yrow = (int)round(size * (y - ymin) / (ymax - ymin));
          int xcol = (int)round(size * (x - xmin) / (xmax - xmin));

          if (yrow < 0 || yrow >= size) {
            continue; // out of range
          }
          
          if(xcol < 0 || xcol >= size){
            continue; // out of range
          }

          pthread_mutex_lock(&count_mutex);
          count[yrow][xcol]++;
          if (count[yrow][xcol] > max_count) {
            max_count = count[yrow][xcol];
          }
          pthread_mutex_unlock(&count_mutex);
        }
    }
  }

  // wait for all threads to finish steps 1 and 2
  pthread_barrier_wait(&barrier);

  // compute colors
  double gamma = 0.681;
  double factor = 1.0 / gamma;
  for (int row = start_row; row < end_row; row++) {
    for (int col = start_col; col < end_col; col++) {
      double value = 0.0;
      if (count[row][col] > 0) {
        value = log(count[row][col]) / log(max_count);
        value = pow(value, factor);
      }
      image[row][col].red = (unsigned int)(value * 255);
      image[row][col].green = (unsigned int)(value * 255);
      image[row][col].blue = (unsigned int)(value * 255);
    }
  }

  return NULL;
}

int main(int argc, char* argv[]) {
  int opt;
  while ((opt = getopt(argc, argv, ":s:l:r:t:b:p:")) != -1) {
    switch (opt) {
      case 's': size = atoi(optarg); break;
      case 'l': xmin = atof(optarg); break;
      case 'r': xmax = atof(optarg); break;
      case 't': ymax = atof(optarg); break;
      case 'b': ymin = atof(optarg); break;
      case '?': printf("usage: %s -s <size> -l <xmin> -r <xmax> "
        "-b <ymin> -t <ymax> -p <numProcesses>\n", argv[0]); break;
    }
  }
  printf("Generating mandelbrot with size %dx%d\n", size, size);
  printf("  Num processes = %d\n", numProcesses);
  printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
  printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);

  // todo: your code here
  membership = malloc(size * sizeof(int*));
  count = malloc(size * sizeof(int*));
  image = malloc(size * sizeof(struct ppm_pixel*));
  for (int i = 0; i < size; i++) {
    membership[i] = malloc(size * sizeof(int));
    count[i] = malloc(size * sizeof(int));
    image[i] = malloc(size * sizeof(struct ppm_pixel));
  }

  pthread_t threads[numProcesses];
  int thread_ids[numProcesses];
  pthread_mutex_init(&count_mutex, NULL);
  pthread_barrier_init(&barrier, NULL, numProcesses);

  // Start the timer
  double timer;
  struct timeval tstart, tend;
  gettimeofday(&tstart, NULL);

  // create threads
  for (int i = 0; i < numProcesses; i++) {
    thread_ids[i] = i;
    pthread_create(&threads[i], NULL, start, &thread_ids[i]);
  }

  // wait for threads to finish
  for (int i = 0; i < numProcesses; i++) {
    pthread_join(threads[i], NULL);
  }

  // End the timer
  gettimeofday(&tend, NULL);
  timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec)/1.e6;

  // Get the current timestamp
  time_t timestamp = time(0);
  char filename[100];
  snprintf(filename, sizeof(filename), "buddhabrot-%d-%ld.ppm", size, timestamp);

  printf("Computed buddhabrot set (%dx%d) in %.6f seconds\n", size, size, timer);
  write_ppm(filename, image, size, size);

  // cleanup
  pthread_barrier_destroy(&barrier);
  pthread_mutex_destroy(&count_mutex);

  for (int i = 0; i < size; i++){
    free(membership[i]);
    free(count[i]);
    free(image[i]);
  }
  free(membership);
  free(count);
  
}
