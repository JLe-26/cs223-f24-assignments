/**
 * This program turns a word into an exclamation by scanning a string of characters,
 * looping through each character, and replacing each lowercase letter with a randomly 
 * chosen character from the set {'@', '!', '#', '*'}.
 *
 * @author: Jenny Le
 * @version: September 11, 2024
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
  char buff[32]; // Player word input with 32 character buffer
  char exclaim[5] = {'@', '!', '#', '*','\0'};
    printf("Enter a word: ");
    scanf("%s", buff);
  srand(time(NULL)); // Seed value is random every time the program runs
  int i = 0; // iterator  

  // Loop
  while(buff[i] != '\0')
  {
	if(buff[i] >= 97 && buff[i] <= 122) // Checks if char in buff is lowercase
	{
		buff[i] = exclaim[rand() % 4];
	}
    i++;	
  }
    
    printf("OMG! %s\n", buff);
  
  return 0;
}

