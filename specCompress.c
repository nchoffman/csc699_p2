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
void printBP(struct breakPoint bpList[], int length);
void printEnv(float env[], int length);
void sortBP(struct breakPoint bpList[], int length);
int findBP(struct breakPoint bpList[], int index, int length);

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


int main(int argc, char **argv)
{

  int i, k, j, curIndex, bpIndex;
  int brPts; // number of breakpoints
  FILE *saolF, *saslF; // pointer to saol and sasl file
  struct breakPoint *bpList;
  float* curEnvelope; // approximation envelope for a harmonic
  struct breakPoint current;

  // check command-line arguments
  if (argc < 4) {
	printf("Using: specCompress [.pv.an file] [# of breakpoints] [.saol file] [.sasl file]\n");
	return 1;  
  }

  /* read in an analysis file */
  anread(argv[1],10);

  /* get the number of breakpoints, include the first and last breakpoint */
  brPts = atoi(argv[2]) + 2;

  /* create memory for breakpoints */
  //current = (struct breakPoint *) malloc (sizeof(struct breakPoint));
  bpList = (struct breakPoint *) malloc (sizeof(struct breakPoint) * brPts);
  //for (i = 0; i < brPts; i++) {
  //  bpList[i] = (struct breakPoint *) malloc (sizeof(struct breakPoint));
  //}

  /* create memory for envelope */
  curEnvelope = (float*)(malloc(sizeof(float) * npts));

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
    //Reset arrays for next harmonic
    //clean(curEnvelope, npts, bpList, brPts);

    //Gather first and last points
    bpList[0].amplitude = cmag[i];
    bpList[0].index = 0;
    bpList[1].amplitude = cmag[nhar1*(npts-1)+i];
    bpList[1].index = npts-1;

    curEnvelope[0] = bpList[0].amplitude;
    curEnvelope[npts-1] = bpList[1].amplitude;

    //interpolate the first and last points in curEnvelope
    interpolate(curEnvelope, bpList[0].index, bpList[1].index);

    for(j = 2; j < brPts + 2; j++) {	//find x # of brPts
    
      //Gather current maxError into struct
      current = getMaxError(&cmag[i], curEnvelope, npts, nhar1);
	
      //add to bpList[]
      bpList[j].index = current.index;
      bpList[j].amplitude = current.amplitude;

      //update curEnvelope with the current maxError breakpoint
      curEnvelope[current.index] = current.amplitude;

      //sort bpList[] by index
      sortBP(bpList, j + 1);

      //find where the current breakpoint is in the sorted list
      bpIndex = findBP(bpList, current.index, j + 1);
		
      // interpolate from the previous to current breakpoint
      interpolate(curEnvelope, bpList[bpIndex - 1].index, bpList[bpIndex].index);

      // interpolate from the current to next breakpoint
      interpolate(curEnvelope, bpList[bpIndex].index, bpList[bpIndex + 1].index);
    }

    fprintf(saolF, "  harm%d = kline(", i+1);
    for (j = 0; j < brPts + 2; j++) {
      if (j == 0) {
        fprintf(saolF, "%4.6f, ", bpList[j].amplitude);
      }
      else if (j == brPts + 1) {
        fprintf(saolF, "%f, %4.6f);\n\n", dt*bpList[j].index - dt*bpList[j-1].index, bpList[j].amplitude);
      } else {
        fprintf(saolF, "%f, %4.6f, ", dt*bpList[j].index - dt*bpList[j-1].index, bpList[j].amplitude);
      }
      if (j % 4 == 0 && j < brPts + 1) {
        fprintf(saolF, "\n             ");
      }
    }

  }


  //set frequency of each harmonic
  fprintf(saolF, "\n\n   ");
  for (i = 1; i <= 20; i++) {
    fprintf(saolF, "harm_freq%d = oscil(cyc, %d*fr);\n", i, i);
  }

  //sum all sinusoids into complex waveform
  fprintf(saolF, "\n\n   ");
  fprintf(saolF, "y = ");
  for (i = 1; i <= 20; i++) {
    if (i < 20) {
      fprintf(saolF, "harm%d*harm_freq%d + ", i, i);
    } else {
      fprintf(saolF, "harm%d*harm_freq%d;", i, i);
    }
    if (i % 4 == 0) {
      fprintf(saolF, "\n       ");
    }
  }

  //scale output
  fprintf(saolF, "\n\n   ");
  fprintf(saolF, "y = y / 32767;");

  //output summed waveform
  fprintf(saolF, "\n\n   ");
  fprintf(saolF, "output (y);\n");
  fprintf(saolF, "}");
  
   
  //WRITE TO SASL FILE
  fprintf(saslF, "0.0 tone %5.3f %6.2f\n", tl, fa);
  fprintf(saslF, "3.0 end\n");

  //close soal and sasl files
  fclose(saolF);
  fclose(saslF);

  return 0;
}
