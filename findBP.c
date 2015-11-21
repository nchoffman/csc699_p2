#include <stdio.h>
#include <stdlib.h>
#include "breakpoint.h"

int findBP(struct breakPoint bpList[], int index, int length) {

   int i;

   for(i = 0; i < length; ++i) {
     if (index == bpList[i].index) {
       break;
     }
   }

   return i;
}
