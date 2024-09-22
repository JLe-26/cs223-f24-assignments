/*----------------------------------------------
 * Author: Jenny Le
 * Date: September 21, 2024
 * Description: This program reads songs from 
 * a text file into an array and is able to set the 
 * contents of the dynamic array of songs.
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printSongList(struct Song array[]){
  for (int i = 0; i < numSongs; i++) //Prints struct Song information
  {
	  printf("%d %c %-20s artist: %-20s duration: %d %c %d D: %.2f E: %f T: %f V: %f \n", i, ')', array[i].title, array[i].artist, array[i].duration/60000, ':', array[i].duration%60000, array[i].danceability, array[i].energy, array[i].tempo, array[i].valence);
  }

}

int main() {
  printf("Welcome to Jenny's Danceability Directory");

  // Song struct
  struct Song
  {
	  char *title[128];
	  char *artist[128];
	  int duration; // in milliseconds
	  float danceability;
	  float energy;
	  float tempo;
	  float valence;

  };

  struct Song song;

  // Opens the songlist.csv file
  FILE *infile;

  infile = fopen("songlist.csv", "r");
  if (infile == NULL)
  {
	  printf("Error: unable to open file %s\n", "songlist.csv");
	  exit(1);
  }

  int numSongs;
  struct Song array[] = malloc(sizeof(Song) * numSongs); 
  
  // Reads the file
  char chunk[128];
  while (fgets(chunk, sizeof(chunk), infile) != NULL)
  {
	  char *token = strtok(chunk, ',');
          if (token != NULL)
	  {
		  numSongs = atoi(token);
	  }

  }


  fclose(infile);
  free(array);
  return 0;
}

