#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "header.h"
#include "breakpoint.h"

void clean(float env[], int length, struct breakPoint bpList[], int length2) {
	
	int i;
	for(i = 0; i < length; i++) {
		env[i] = 0;
	}
	for(i = 0; i < length2; i++) {
		bpList[i].amplitude = 0.0;
		bpList[i].index = 0;
	}

}