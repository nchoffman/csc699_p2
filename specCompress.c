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
void clean(float env[], int length, struct breakPoint bpList[], int length2);

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
  /* NOTE FROM MICHAEL: I moved all the variable declarations here, just because
                        it is a commont c language convention to do so */
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


   for (i = 0; i < 20; i++) {		//for 20 harmonics
	//new function to clean bpList & curEnvelope clean();
	clean(curEnvelope, npts, bpList, brPts);	

	

	bpList[0].amplitude = cmag[i];
	bpList[1].amplitude = cmag[nhar1*(npts-1)+i];

   /* NOTE FROM MICHAEL: At this point you should do call the linear interpolation function
                         to interpolate the envelope from the first point to the last point,
                         but right now I still kept it so that the envelope is initialized to all zeros */
	
	//interpolate curEnvelope
   
	for(j = 0; j < brPts; j++) {	//find x # of brPts

		//current = getMaxError(&cmag[i], curEnvelope, npts, nhar1);
		//bpList[j+2] = current;
                /** NOTE FROM MICHAEL: you'll probably want to write a seperate sort function, find function, and
                                       interpolate function in seperate files, just like the getMaxError.c file, 
                                       make sure to update the Makefile to reflect those changes (just like how I 
                                       updated the Makefile to include getMaxError.c) */
		//sort
		//find 'current' in bpList by index - int find(struct breakPoint bpList, int current.index){}
		//interpolate -1 index 
		//interpolate +1 index
		
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

/* NOTE FROM MICHAEL: you will want to put this in a seperate file just like I
                      did with the getMaxError function */
void interpolate(float env[], int start, int end) {

}
