#include "breakpoint.h"
#include <stdio.h>
#include <stdlib.h>

//example call: getMaxError(cmag, curEnvelope, header.npts, nhar1);
struct breakPoint getMaxError(float data[], float env[], int length, int nhar1) {

  int i, temp, temp2;
  float max = 0;
  struct breakPoint current;

  printf("\nInside getMaxError()\n");
  for (i = 0; i < length; i++) {
    temp2 = abs(env[i] - data[1 + i*nhar1]);
    if (max < temp2) {
      max = temp2;
      temp = i;
      //printf(" Current max: %8.2f \n", max);
    } else {
      //printf("\n NO NEW MAX: %d", i);
    }
  }

  current.amplitude = max;
  current.index = temp;

  printf("Max amp error: %8.2f, Index: %d \n ", max, temp);

  return current;

}

