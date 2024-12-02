/*
* This program uses a simplified version of the grep command that searches
* for a keyword in multiple files using multithreading.
* This program divides the files among N threads, each searching for 
* a keyword in the assigned files.
* 
* @author: Jenny Le
* @version: December 1, 2024
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

pthread_mutex_t mutex;

struct thread_data_t {
  int thread_id;
  int num_threads;
  char **files;
  int start_index;
  int end_index;
  char *keyword;
  pthread_mutex_t *mutex;
};

// File search function
void *search_file(void *arg) {
  struct thread_data_t *data = (struct thread_data_t *)arg;
  FILE *file = NULL;
  int i = data->start_index;

  // Iterate over the files assigned to this thread
  for (int i = data->start_index; i < data->end_index; i++) {
    file = fopen(data->files[i], "r");
    if(!file) {
      perror("Error opening file");
      continue;
    }
  }

  char line[1024]; // Store each line from the file
  int line_num = 0;
  int match_count = 0;

  // Search the file line by line
  while (fgets(line, sizeof(line), file)) {
    line_num++;

    // Check if the line contains the keyword
    if (strstr(line, data->keyword)) {
      pthread_mutex_lock(&mutex);
      printf("Match in file: %s, line %d: %s\n", data->files[i], line_num, line);
      pthread_mutex_unlock(&mutex);
      match_count++;
    }
  }

  // Print total matches for this file
  if (match_count > 0) {
    pthread_mutex_lock(&mutex);
    printf("Total matches in file %s: %d\n", data->files[i], match_count);
    pthread_mutex_unlock(&mutex);
  }
  
  fclose(file);

  return NULL;
}

int main(int argc, char *argv[]) {
  if (argc < 4) {
    printf("usage: %s <NumThreads> <Keyword> <Files>\n", argv[0]);
    return 1;
  }

  int num_threads = atoi(argv[1]);
  char *keyword = argv[2];
  char **files = &argv[3];
  int num_files = argc - 3;

  // Create threads and assign files to each thread
  pthread_t *threads = (pthread_t*)malloc(num_threads * sizeof(pthread_t));
  pthread_mutex_init(&mutex, NULL);

  for (int i = 0; i < num_threads; i++) {
    struct thread_data_t *data = (struct thread_data_t *)malloc(sizeof(struct thread_data_t));
    data-> thread_id = i + 1;
    data->num_threads = num_threads;
    data->files = files;
    data->keyword = keyword;
    data->mutex = &mutex; // Passing mutex ptr to thread data

    // Range of files for each thread
    data->start_index = i * num_files / num_threads;
    data->end_index = (i + 1) * num_files / num_threads;

    if (i == num_threads - 1) {
      data->end_index = num_files; // Remaining files go to the last thread
    }

    // Create threads
    pthread_create(&threads[i], NULL, search_file, data);
  }

  // Wait for all threads to finish
  for (int i = 0; i < num_threads; i++) {
    pthread_join(threads[i], NULL);
  }

  pthread_mutex_destroy(&mutex);
  free(threads);

  return 0;
}
