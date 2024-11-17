/***************************************************
 * mathgame.c
 * Author: Jenny Le
 * Implements a math game
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
 int r; //num rounds
 int counter = 0; //score
 printf("Welcome to Math Game!\nHow many rounds do you want to play?\t"); 
 scanf("%d", &r); //reads player response

 //game loop
 for(int i= 0;i<r;i++){
 srand(time(NULL)); //seeds the random number generator according to time 
 int val1 = (rand() % 9) + 1; //randomized values
 int val2 = (rand() % 9) + 1;
 int answer; //player response

 printf("%i %s %i %s\t %s\t", val1, "+", val2, "=", "?"); //prints math question
 scanf("%i", &answer);
 if(answer == val1 + val2){
	 printf("Correct!\n");
	 counter++;
 }
 else{
	 printf("Incorrect :(\n");
 }
 }
 printf("%s %i %s %i %s\n", "You answered", counter, "/", r, "correctly."); 
  return 0;
}


