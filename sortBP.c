#include <stdio.h>
#include <stdlib.h>
#include "breakpoint.h"

void sortBP(struct breakPoint bpList[], int length) {

   int i, j;
   struct breakPoint temp;

   for(i = 0; i < length; ++i) {
	for(j = i+1; j < length; ++j) {
		if(bpList[i].index > bpList[j].index) {
			temp = bpList[i];
			bpList[i] = bpList[j];
			bpList[j] = temp;
		}
	}
   }


}