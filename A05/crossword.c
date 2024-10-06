/*----------------------------------------------
 * Author: Jenny Le 
 * Date: October 4, 2024
 * Description: This program generates a crossword using two words from a user.
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
  if (argc < 3)
  {
	  printf("Usage: %s <word1> <word2>\n", argv[0]);
	  exit(0);
  }
 
  char word1[100];
  strcpy(word1, argv[1]); // argv[1] is the first word
  int len1 = strlen(argv[1]);

  char word2[100];
  strcpy(word2, argv[2]); // argv[2] is the second word
  int len2 = strlen(argv[2]); 
  
  // Finding common letter
  char common;
  int posI; // position of common letter in first word
  int posJ; // position of common letter in second word 

  for (int i = 0; word1[i] != '\0'; i++)
  {
	for (int j = 0; word2[j] != '\0'; j++)
	{
		if (word1[i] == word2[j])
		{
			common = word1[i];
			posI = i;
			posJ = j;
		}
	}
  }

  // Creating 2d array
  if (common != '\0')
  {
	// If there is a common letter, a crossword is created
  	char **arr = malloc(sizeof(char *) * len1); // The rows are determined by the first word, which is printed vertically
  	for (int i = 0; i < len1; i++)
  	{	
		arr[i] = malloc(sizeof(char) * len2); // The cols are determined by the second word, which is printed horizontally
  	}

	// Filling the crossword with blanks
	char blank = '.';
	for (int i = 0; i < len1; i++)
	{
		for (int j = 0; j < len2; j++)
		{
			arr[i][j] = blank;
		}
	}
	
	// Adding the words to the array
	for (int k = 0; k < len2; k++)
	{
		arr[posI][k] = word2[k]; // Adds the second word to a row
	}
	for (int l = 0; l < len1; l++)
	{
		arr[l][posJ] = word1[l]; // Adds the first word to a column
	}

	// Print the array as a table
	for (int m = 0; m < len1; m++)
	{
		for(int n = 0; n < len2; n++)
		{
			printf("%c", arr[m][n]); // Prints each element in the current row of the 2d array
		}
		printf("\n"); // Prints new line after each row
	}
	
  	for (int i = 0; i < len1; i++) 
  	{
	  	free(arr[i]); // Freeing each row
  	}
  	free(arr);
  }
  else
  {
	  common = '\0';
	  printf("No common letter!\n");
  }

  return 0;
}
