#include <stdio.h>

void interpolate(float env[], int start, int end) {

   int i;
   float slope = (env[end] - env[start])/(end - start);

   for(i = start; i < end; i++) {
       env[i] = slope * (i - start) + env[start];
   }
}
