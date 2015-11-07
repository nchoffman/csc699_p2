#include "breakpoint.h"
#include <stdio.h>
#include <stdlib.h>

//example call: getMaxError(cmag, curEnvelope, header.npts, nhar1);
struct breakPoint getMaxError(float data[], float env[], int length, int nhar1) {

  int i, index;
  float max = 0, temp;

  struct breakPoint current;

  printf("\nInside getMaxError()\n");
  for (i = 0; i < length; i++) {
    temp = abs(env[i] - data[i*nhar1]);
    if (max < temp) {
      max = temp;
      index = i;
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
