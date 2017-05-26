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

#define MAXWIDTH 4096
#define MAXHEIGHT 2160
#define MINWIDTH 100
#define MINHEIGHT 100

#define EXIT1	for (int i = 0; i < maxIteration; i++)\
		{\
			free(colorMapEasy[i]);\
		}\
		free(colorMapEasy);\
		free(pixelRGBNumberBuffer);\


struct codeThread
{
	int pixel;
	int startHeight;
	int endHeight;
};

typedef struct codeThread CODETHREAD;
typedef struct timeval TIMEVAL;

unsigned char **colorMapEasy;
unsigned char *pixelRGBNumberBuffer;
int maxIteration, width, height, mode;
long double realOffset, imagOffset, zoomStart, zoomInfo;

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
