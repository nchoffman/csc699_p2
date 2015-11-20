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
void interpolate(float env[], struct breakPoint bpList[], int length);
void clean(float env[], int length, struct breakPoint bpList[], int length2);
void printBP(struct breakPoint bpList[], int length);
void printEnv(float env[], int length);
void sortBP(struct breakPoint bpList[], int length);

/*    global variables declared as externs in monan.h need a root position  */
HEADER header;
int nhar, nhar1, nchans, npts;
float *cmag, *dfr, *phase, *br, *time, tl, dt, fa, smax, *newmag, *newfr,
  *finalfr;

float curEnvelope[1551];

/* 
   cmag[] - amp values per harmonic
   dt	- time of each frame
   fa	- frequency of fundamental
   tl	- sound duration
   header.npts - total # of frames
*/


double ampscale;
struct breakPoint first, last, current;


int main(int argc, char **argv)
{

  int i, k, j, curIndex;
  int brPts = atoi(argv[2]);
  FILE *saolF, *saslF;
  struct breakPoint bpList[brPts+2];
  

  if (argc < 4) {
	printf("Using: specCompress [.pv.an file] [# of breakpoints] [.saol file] [.sasl file]\n");
	return 1;  
  }

/* read in an analysis file */
  anread(argv[1],10);


   /************************************
    	CREATE/WRITE SAOL & SASL FILES
   *************************************/


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


   for (i = 0; i < 1; i++) {		//for 20 harmonics
	//Reset arrays for next harmonic
	clean(curEnvelope, npts, bpList, brPts);

	//Gather first and last points
	bpList[0].amplitude = cmag[i];
	bpList[0].index = 0;
	bpList[1].amplitude = cmag[nhar1*(npts-1)+i];
	bpList[1].index = npts-1;

	curEnvelope[0] = cmag[i];
	curEnvelope[npts-1] = 0.0;

	sortBP(bpList, brPts);	
	printBP(bpList, brPts);

	//interpolate curEnvelope
	printf("\nTesting new interpolate...\n");
	interpolate(curEnvelope, bpList, brPts);

	for(j = 0; j < brPts; j++) {	//find x # of brPts

		//Gather current maxError into struct
		current = getMaxError(&cmag[i], curEnvelope, npts, nhar1);
	
		//add to bpList[]
		bpList[0].index = current.index;
		bpList[0].amplitude = current.amplitude;

		//sort bpList[] by index
		sortBP(bpList, brPts);
		
		printBP(bpList, brPts);

		printf("\n Testing new interpolate...\n");
		interpolate(curEnvelope, bpList, brPts);
	}
    }


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
