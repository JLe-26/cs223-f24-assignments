#include <stdio.h>

int main() {
  char phrase[1024]; // Player input
  int i = 0; // Iterator
  int j = 0; // Iterator for acronym
  printf("Enter a phrase: \n");
  scanf("%[^\n]%*c", phrase);
  char acronym[1024];

  // Loop
  while(phrase[i] != '\0')
  {
	  if(phrase[i] >= 65 && phrase[i] <= 90) //Checks if char is Uppercase
	  {
		  acronym[j]=phrase[i]; //Inserts Uppercase char into acronym
	  	  j++;
	  }
    i++;
  }  

  acronym[j] = '\0'; // Adds terminating character to acronym array

  printf("Your acronym is %s\n", acronym);
  return 0;
}
