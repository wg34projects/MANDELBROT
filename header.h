#ifndef __HEADER_H_
#define __HEADER_H_

#include <sys/time.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>
#include <math.h>
#include <errno.h>
#include <pthread.h>

#define MAXWIDTH 		4096		// max pic width
#define MAXHEIGHT 		2160		// max pic height
#define MINWIDTH 		100			// min pic width
#define MINHEIGHT 		100			// min pic height
#define MAPS 			15			// amount of mappings in mapping.c
#define THREADS 		12			// amount of parallel threads

// safe exit makros
#define EXIT1			for (int i = 0; i < maxIteration; i++)\
						{\
							free(colorMapEasy[i]);\
						}\
						free(colorMapEasy);

#define EXIT2			for (int i = 0; i < THREADS; i++)\
						{\
							free(codeMatrixThread[i]);\
						}\
						free(codeMatrixThread);

// struct for the threads
struct codeThread
{
	int pixel;
	int startHeight;
	int endHeight;
};

typedef struct codeThread CODETHREAD;
typedef struct timeval TIMEVAL;

// global vars for CTRL-C handling
unsigned char **colorMapEasy;
unsigned char *pixelRGBNumberBuffer;
int maxIteration, width, height, mode;
long double realOffset, imagOffset, zoomStart, zoomInfo;
CODETHREAD **codeMatrixThread;

// function prototypes
void *calcStripeThread(void *code);
void ctrlChandlingGenerator(int dummy);
void ctrlChandlingWriter(int dummy);
int safeExit(int sharedMemID, int semaphoreID);
int writePPM6(unsigned char *pixelValues, FILE *output, int width, int height, int bright);
int readSETUPtxt(int *width, int *height, int *mode, long double *realOffset, long double *imagOffset, long double *zoomStart, int *maxIteration, int *mapType, long double *offsetRGB, char prefix[]);
void colorMap(int maxIteration, int mapType, long double offsetRGB, unsigned char **colorMapEasy);
void help();

#endif
