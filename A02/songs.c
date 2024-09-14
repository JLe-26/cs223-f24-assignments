/**
 * This program fills an array with three or more song structs and then allows the user
 * to edit the data in the array. 
 *
 * @author: Jenny Le
 * @version: September 13, 2024
 */ 

#include <stdio.h>
#include <string.h>

struct Song {
  int duration; // Duration is in seconds
  float danceability;
  char artist[32];
  char title[32];
};

void printSongList(struct Song array[]){
  for(int i = 0; i < 3; i++) //Prints struct Song information
  {
	  printf("%d %c %-20s artist: %-20s duration: %d %c %d danceability: %.2f \n", i, ')', array[i].title, array[i].artist, array[i].duration/60, ':', array[i].duration%60, array[i].danceability);
  } 
} 

int main() {
  int id;	
  char* attr1 = "artist";
  char* attr2 = "title";
  char* attr3 = "duration";
  char* attr4 = "danceability";
  char response[13]; // User input for attribute selection
  char str[32]; // User input for artist reassignment
  char str2[32]; // User input for title reassignment
  int num; // User input for duration reassignment
  float num2; // User input for danceability reassignment

  printf("Welcome to Jenny's Song List\n");

  // Initialize three songs
  struct Song song1, song2, song3;

  song1.duration = 37;
  song1.danceability = 0.2f;
  strcpy(song1.artist, "Wave to Earth");
  strcpy(song1.title, "Summer Flows");

  song2.duration = 216;
  song2.danceability = 0.8f;
  strcpy(song2.artist, "NIKI");
  strcpy(song2.title, "Every Summertime");

  song3.duration = 225;
  song3.danceability = 0.5f;
  strcpy(song3.artist, "HYBS");
  strcpy(song3.title, "Tip Toe");

  // Store them into an array
  struct Song songList[3];
  songList[0] = song1;
  songList[1] = song2;
  songList[2] = song3;

  // Table output
  printSongList(songList);

  printf("====================\n\n");
  
  // Adjust attribute values
  printf("Enter a song id to edit [0,1,2] : \n");
  scanf("%d", &id);
  
  // Check for valid song id
  if(id < 0 || id > 2)
  {
	  printf("This is not a valid song id! Please print a valid id.\n");
  }
  else
  {
	  printf("Which attribute do you wish to edit? [artist, title, duration, danceability] : \n");
          scanf("%s", response);
	  if(strcmp(response, attr1) == 0)
	  {
		 printf("Enter an artist: \n");
		 scanf("%s", str);
	         strcpy(songList[id].artist, str);
	  }
	  else if(strcmp(response, attr2) == 0)
	  {
		 printf("Enter a title: \n");
	         scanf("%s", str2);
		 strcpy(songList[id].title, str2);
	  }
          else if(strcmp(response, attr3) == 0)
	  {
	  	 printf("Enter a duration: \n");
                 scanf("%d%*c", &num);
		 songList[id].duration = num;
	  }
	  else if(strcmp(response, attr4) == 0)
	  {
		 printf("Enter a danceability: \n");
		 scanf("%f%*c", &num2);
		 songList[id].danceability = num2;
	  }
	  else
	  {
		 printf("You have not entered a valid attribute. Please try again.");
          }		 
  }
  printSongList(songList);
  printf("====================\n\n");
  return 0;
}

