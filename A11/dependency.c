#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "tree.h"

// Global variables
pthread_mutex_t mutex;
struct tree_node* root = NULL; // Root of BST

struct thread_data {
  char** files;
  int start_index;
  int end_index;
};

// Function to parse dependencies from a file
void parse_dependency(const char* filename, struct tree_node* node) {
  FILE *file = NULL;
  file = fopen(filename, "r");
  if (!file) {
    perror("Error opening file");
    return;
  }

  char line[1024];
  while (fgets(line, sizeof(line), file)) {
    if (strstr(line, "#include")) {
      // Extract the filename from the #include line
      char* include_file = strchr(line, '"');
      if (include_file) {
        include_file++; // Skip the opening quote
        char* end_quote = strchr(include_file, '"');
        if (end_quote) {
          *end_quote = '\0'; // Ensure null-terminating character in filename
          // Add the dependency to the current node
          pthread_mutex_lock(&mutex);
          root = insert(include_file, root);
          pthread_mutex_unlock(&mutex);
        }
      }
    }
  }

  fclose(file);
}

// Function to process a set of files in a thread
void* process_files(void* arg) {
  struct thread_data* data = (struct thread_data*)arg;
  for (int i = data->start_index; i < data->end_index; i++) {
    char* filename = data->files[i];

    // Add the file to the BST
    pthread_mutex_lock(&mutex);
    root = insert(filename, root);
    pthread_mutex_unlock(&mutex);

    // Parse dependencies in the file
    parse_dependency(filename, root);
  }
  return NULL;
}

void check_dependency(const char* filename) {
  struct tree_node* node = find(filename, root);
  if (node) {
    printf("%s has the following dependencies:\n", filename);
    print(node->left); // Assume dependencies are inserted into left subtree
  } else {
    printf("%s not found", filename);
  }
}

void commands() {
  char command[256];
  
  while (1) {
    printf("$ ");
    // Infinite loop
    if (!fgets(command, sizeof(command), stdin)) {
       break;
    }

    // Remove newline character if present
    command[strcspn(command, "\n")] = '\0';

    if (strcmp(command, "quit") == 0) {
      printf("Exiting...\n");
      break;
    } else if (strcmp(command, "list") == 0) {
      printSorted(root);
    } else {
       check_dependency(command);
    }
  }
}

int main(int argc, char* argv[])
{
  if (argc < 4) {
    printf("Usage: %s <NumThreads> <Keyword> <Files>\n", argv[0]);
    return 1;
  }

  int num_threads = atoi(argv[1]);
  if (num_threads <= 0) {
    printf("Invalid number of threads.\n");
    return 1;
  }

  char** files = &argv[3];
  int num_files = argc - 3;

  pthread_mutex_init(&mutex, NULL);
  pthread_t *threads = malloc(num_threads * sizeof(pthread_t));
  struct thread_data* thread_data = (struct thread_data*)malloc(sizeof(struct thread_data) * num_threads);

  for (int i = 0; i < num_threads; i++) {
    thread_data[i].files = files;
    thread_data[i].start_index = i * num_files / num_threads;
    thread_data[i].end_index = (i + 1) * num_files / num_threads;
    // Last thread processes the remaining files
    if (i == num_threads - 1) {
      thread_data[i].end_index = num_files;
    }

    pthread_create(&threads[i], NULL, process_files, &thread_data[i]);
  }

  // Wait for all threads to finish
  for (int i = 0; i < num_threads; i++) {
    pthread_join(threads[i], NULL);
  }

  printf("Processing completed.\n");

  clear(root);
  return 0;
}
