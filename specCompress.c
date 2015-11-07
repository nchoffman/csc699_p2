/*
  Sample code for CSc 635 Project 2

  compile:
  gcc -O cpan.c header.c anread.c -o cpan -lm

  run:
  cpan [.an] 

*/
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "header.h"
#include "breakpoint.h"
#define P printf

/** function declarations **/
int anread(char*, int);					    /* 05/06 /96 */
struct breakPoint getMaxError(float data[], float env[],	int length,	int nhar1);
void interpolate(float env[], int start, int end);

/*    global variables declared as externs in monan.h need a root position  */
HEADER header;
int nhar, nhar1, nchans, npts;
float *cmag, *dfr, *phase, *br, *time, tl, dt, fa, smax, *newmag, *newfr,
  *finalfr;

/* 
   cmag[] - amp values per harmonic
   dt	- time of each frame
   fa	- frequency of fundamental
   tl	- sound duration
   header.npts - total # of frames
*/
//cmag[]      curEnvelope	length		nhar1

double ampscale;
struct breakPoint first, last, current;


int main(int argc, char **argv)
{
  int i, k, j, curIndex;
  

  if (argc < 4) {
	printf("Using: specCompress [.pv.an file] [# of breakpoints] [.saol file] [.sasl file]\n");
	return 1;  
  }

/* read in an analysis file */
  anread(argv[1],10);
  

  int brPts = atoi(argv[2]);

  float curEnvelope[npts];
  for (i = 0; i<npts; i++) {
	curEnvelope[i] = 0;
  }

  struct breakPoint bpList[brPts+2];


   /************************************
    	CREATE/WRITE SAOL & SASL FILES
   *************************************/


   FILE *saolF, *saslF;
   saolF = fopen(argv[3], "w");
   saslF = fopen(argv[4], "w");

   /*WRITE TO SAOL FILE*/
   //create wavetable
   fprintf(saolF, "global {\n   table cyc(harm, 128, 1);\n   srate 44100;\n}\n\n");


   //instr block
   fprintf(saolF,"instr tone(fr) {\n   imports exports table cyc;\n\n   ");


   //write ksigs
   fprintf(saolF, "ksig ");
   for (i = 1; i <= 20; i++) {
	if (i == 20) {
		fprintf(saolF, "harm%d;\n\n   ", i);
	} else {
		if(i % 5 == 0) {
			fprintf(saolF, "\n	harm%d, ", i);
		} else {
			fprintf(saolF, "harm%d, ", i);
		}
	}
   }


  //write asigs
  fprintf(saolF, "asig ");
   for (i = 1; i <= 20; i++) {
	if (i == 20) {
		fprintf(saolF, "harm_freq%d, y;\n\n   ", i);
	} else {
		if(i % 5 == 0) {
			fprintf(saolF, "\n	harm_freq%d, ", i);
		} else {
			fprintf(saolF, "harm_freq%d, ", i);
		}
	}
   }



   //write klines
   
   fprintf(saolF, "//start klines\n   ");
   for(i = 0; i < npts; i++) {
	printf("Index: %d, Amp: %8.2f\n", i, cmag[1+i*nhar1]);
   }
   //current = getMaxError(cmag, curEnvelope, header.npts, nhar1);
   //curEnvelope[current.index] = current.amplitude;
   //printf("\n\noutside getMaxError()\n\nMax error at index: %d, Inputting amplitude: %8.2f \n", current.index, current.amplitude);

  
   for (i = 1; i < 21; i++) {		//for 20 harmonics
	
	bpList[0].amplitude = cmag[i];
	bpList[1].amplitude = cmag[nhar1*(npts-1)+i];

	for(j = 0; j < brPts; j++) {	//find x # of brPts

		current = getMaxError(&cmag[i], curEnvelope, npts, nhar1);
		bpList[j+2] = current;
		//sort
		//find 'current' in bpList by index - int find(struct breakPoint bpList, int current.index){}
		//interpolate -1 index 
		//interpolate +1 index
		
	}
    }
		

   /*cmag[k + i*nhar1]
    k = which harmonic #
    i = which amp value
   */
   //find specified number of breakpoints by findMaxError()
   /* for (i = 0; i < 5; i++) {	//for loop to write amp values in saolF
	   for (k = 0; k < 10;k++) {
		fprintf(saolF, "%8.2f, ", cmag[i + k*nhar1]);
	   }
	fprintf(saolF, "\n");
   } */




   //scale klines by short value (32767)
   fprintf(saolF, "\n\n   ");
   for (i = 1; i <= 20; i++) {
	fprintf(saolF, "harm%d = harm%d/32767;\n   ", i, i);
   }

   //set frequency of each harmonic


   //sum all sinusoids into complex waveform


   //output summed waveform
   

   //WRITE TO SASL FILE
   fprintf(saslF, "0.0 tone %5.3f %6.2f\n", tl, fa);
   fprintf(saslF, "3.0 end\n");

   fclose(saolF);
   fclose(saslF);

}

void interpolate(float env[], int start, int end) {

}
