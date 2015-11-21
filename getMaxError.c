#include "breakpoint.h"
#include <stdio.h>
#include <stdlib.h>

//example call: getMaxError(cmag, curEnvelope, header.npts, nhar1);
struct breakPoint getMaxError(float data[], float env[], int length, int nhar1) {

  int i, index;
  float max = 0, temp;

  struct breakPoint current;

  for (i = 0; i < length; i++) {
    temp = abs(env[i] - data[i*nhar1]);
    if (max < temp) {
      max = temp;
      index = i;
    }
  }

  current.amplitude = max;
  current.index = index;

  return current;
}
