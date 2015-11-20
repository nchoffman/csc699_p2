#include <stdlib.h>
#include <stdio.h>
#include "breakpoint.h"

void printBP(struct breakPoint bpList[], int length) {

   int i;

   for(i = 0; i < length; i++) {
	printf("Index: %d, Amplitude: %8.2f\n", bpList[i].index, bpList[i].amplitude);
   }	

}