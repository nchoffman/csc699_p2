#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "breakpoint.h"

void interpolate(float env[], struct breakPoint bpList[], int length) {

   int i, j, start, end;
   float slope;

   for(j = 0; j < length-1; j++) {
	if(bpList[j].index == -1) {
		
	} else {
		printf("\nInside test interpolate... %d\n", j);
		start = bpList[j].index;
		end = bpList[j+1].index;
		//slope = (env[end]-env[start])/(end-start);
		slope = (bpList[j+1].amplitude-bpList[j].amplitude)/(end-start);
	
		for(i = start; i <= end; i++) {
			env[i] = env[start] + slope*i;
 	  	}
		printf("\nPost interpolate...%d\n\n",j);
	}  	
   }

}