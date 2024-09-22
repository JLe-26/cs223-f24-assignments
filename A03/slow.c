/*----------------------------------------------
 * Author: Jenny Le 
 * Date: September 20, 2024
 * Description: This program turns a phrase into ent-speech
 * by asking for pause length and containing '.' between each
 * letter in the input.
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  int num;
  char phrase[32];
  char *slow_text;
  int j = 0; // Iterator

  printf("Pause length: \n ");
  scanf("%d", &num);

  printf("Text: \n");
  scanf("%s", phrase);

  // Allocates an array of chars with enough space for pauses
  slow_text = malloc(sizeof(char) * (strlen(phrase)+((strlen(phrase)-1) * num)));

  for(int i = 0; i < strlen(phrase); i++)
  {
	  if(phrase[i] != '\0'){
		  slow_text[j++] = phrase[i];
	  }
	  for(int k = 0; k < num; k++)
	  {
		  slow_text[j++] = '.';
	  } 
  }  
  printf("%s \n", slow_text);
  free(slow_text);  
  return 0;
}
