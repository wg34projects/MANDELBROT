#include "header.h"

int main(int argc, char *argv[])
{
	FILE *outputFilePointer;
	long double offsetRGB = 0.0;
	int mapType = 0, loopCounter = 0, pixelDivider = 0, threadStatus = 0;
	char prefix[11], mainFileName[101];
	pthread_t calcThread[THREADS];
	pthread_attr_t attribute;
	TIMEVAL tCalcStart, tCalcEnd;
	double timeCalc = 0.0;

	// screen output and hello
	system("clear");
	printf("\nMandelbrot calculation tool dwell.goip.de:8888\n\n");

	// check commandline argv
	if (argc > 1)
	{
		printf("tool needs no options, check SETUP.txt for infos\n\n");
		exit(EXIT_FAILURE);
	}

	// setup from textfile, some check, screen output
	if (readSETUPtxt(&width, &height, &mode, &realOffset, &imagOffset, &zoomStart, &maxIteration, &mapType, &offsetRGB, prefix) != 1)
	{
		printf("problems reading setup file, check syntax\n\n");
		exit(EXIT_FAILURE);
	}
	if (width < MINWIDTH || height < MINHEIGHT || width > MAXWIDTH || height > MAXHEIGHT)
	{
		printf("picture size limitations width[min %d][max %d] height[min %d][max %d]\n\n", MINWIDTH, MAXWIDTH, MINHEIGHT, MAXHEIGHT);
		exit(EXIT_FAILURE);
	}
	if (mapType < 1 || mapType > MAPS)
	{
		printf("mistake with map type, check config\n\n");
		exit(EXIT_FAILURE);
	}
	if (height % THREADS != 0)
	{
		printf("height must be divideable by %02d without rest!\nplease update in SETUP.txt\n\n", THREADS);
		exit(EXIT_FAILURE);
	}
	if (mode < 1)
	{
		printf("at least one picture to calculate, I set for you to 1\n\n");
		mode = 1;
	}

	// if all settings are OK screeninfo
	printf("configuration successfully read from SETUP.txt\n\n");
	printf("width pixel\t%d\n", width);
	printf("height pixel\t%d\n", height);
	printf("realoffset\t%Lf\n", realOffset);
	printf("imagoffset\t%Lf\n", imagOffset);
	printf("zoomstart\t%LE\n", zoomStart);
	printf("iterations\t%d\n", maxIteration);
	printf("color map\t%d\n", mapType);
	printf("color factor\t%Lf\n", offsetRGB);
	printf("prefix file\t%s-xyz.ppm\n", prefix);
	printf("pictures\t%d\n\n", mode);
	printf("please check if OK, continue with ENTER and cancel with CTRL-C\n\n");
	getchar();
	printf("\n");

	// calloc enough place for colormap and initialize
	colorMapEasy = (unsigned char **) calloc(maxIteration, sizeof(unsigned char *));
	if (colorMapEasy == NULL)
	{
		perror("ERROR calloc colorMapEasy");
		exit(EXIT_FAILURE);
	}
	for (int i = 0; i < maxIteration; i++)
	{
		colorMapEasy[i] = (unsigned char *) calloc(3, sizeof(unsigned char));
		if(colorMapEasy[i] == NULL)
		{
			perror("ERROR calloc colorMapEasy[i]");
			free(colorMapEasy);
			exit(EXIT_FAILURE);
		}
	}
	colorMap(maxIteration, mapType, offsetRGB, colorMapEasy);

	// calloc enough place for pixel buffer and initalize
	pixelRGBNumberBuffer = (unsigned char *) calloc(3*width*height, sizeof(unsigned char));
	if (pixelRGBNumberBuffer == NULL)
	{
		printf("calloc error\n");
		EXIT1
		exit(EXIT_FAILURE);
	}

	// calloc space for threads data exchange
	codeMatrixThread = (CODETHREAD **) calloc(THREADS, sizeof(CODETHREAD *));
	if (codeMatrixThread == NULL)
	{
		perror("ERROR calloc codeMatrixThread");
		EXIT1
		free(pixelRGBNumberBuffer);
		exit(EXIT_FAILURE);
	}
	for (int i = 0; i < THREADS; i++)
	{
		calcThread[i] = 0;
		codeMatrixThread[i] = (CODETHREAD *) calloc(1, sizeof(CODETHREAD));
		if (codeMatrixThread[i] == NULL)
		{
			perror("ERROR calloc codeMatrixThread[i]");
			free(codeMatrixThread);
			EXIT1
			free(pixelRGBNumberBuffer);
			exit(EXIT_FAILURE);
		}
	}
	
	// set start variables
	pixelDivider = width * height / THREADS;	
	codeMatrixThread[0]->pixel = 0;
	codeMatrixThread[0]->startHeight = 0;
	codeMatrixThread[0]->endHeight = height / THREADS;
	for (int i = 1; i < THREADS; i++)
	{
		codeMatrixThread[i]->pixel = pixelDivider * i;
		codeMatrixThread[i]->startHeight = codeMatrixThread[i-1]->endHeight;
		codeMatrixThread[i]->endHeight = codeMatrixThread[i-1]->endHeight + height / THREADS;
	}

	// CTRL-C handling
	signal(SIGINT, ctrlChandlingGenerator);

	// calculation
	while(loopCounter < mode)
	{
		// set vars
		loopCounter++;
		zoomInfo = (4.0 / pow(2, loopCounter));
		threadStatus = 0;
		timeCalc = 0.0;

		// start timer
		gettimeofday(&tCalcStart, NULL);

		// unfold the threads
		for (int i = 0; i < THREADS; i++)
		{
			pthread_attr_init(&attribute);
			threadStatus += pthread_create(&calcThread[i], &attribute, calcStripeThread, codeMatrixThread[i]);
			printf("[%05d]\t", i+1);
		}
		printf("\n");

		// wait for threads to be finished
		for (int i = 0; i < THREADS; i++)
		{
			threadStatus -= pthread_join(calcThread[i], NULL);
		}

		// timer info
		gettimeofday(&tCalcEnd, NULL);
		timeCalc = (tCalcEnd.tv_sec+tCalcEnd.tv_usec*0.000001)-(tCalcStart.tv_sec+tCalcStart.tv_usec*0.000001);
		printf("\n%d errors\t%f s calculation time\n", threadStatus, timeCalc);

		// zoom info
		if (mode == 1)
		{
			printf("%.30LE zoom\n", zoomStart);
		}
		else
		{
			printf("%.30LE zoom\n", zoomInfo);
		}

		// prepare output file name and write file
		sprintf(mainFileName, "%s-%03d.ppm", prefix, loopCounter-1);
		mainFileName[strlen(mainFileName)] = '\0';
		outputFilePointer = fopen(mainFileName, "wb");

		// write PPM6
		if (writePPM6(pixelRGBNumberBuffer, outputFilePointer, width, height, 255) != 0)
		{
			perror("ERROR writing output file");
			EXIT1
			free(pixelRGBNumberBuffer);
			EXIT2
			exit(EXIT_FAILURE);
		}
		else
		{
			printf("%03d pictures stored (%s)\tCTRL-C to quit\n\n", loopCounter, mainFileName);
		}
		if(fclose(outputFilePointer) != 0)
		{
			printf("\nerror closing output file\n");
			EXIT1
			free(pixelRGBNumberBuffer);
			EXIT2
			exit(EXIT_FAILURE);
		}
		outputFilePointer = NULL;
	}
	EXIT1
	free(pixelRGBNumberBuffer);
	EXIT2
	exit(EXIT_SUCCESS);
}
