#include <stdio.h>
#include <stdlib.h>

void printEnv(float env[], int length) {

   int i;
   printf("Printing envelope...\n");
   for(i = 0; i < length; i++) {
	printf("Index: %d, Amplitude: %8.2f\n", i, env[i]);
   }

}