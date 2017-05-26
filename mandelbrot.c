#include "header.h"

int main(int argc, char *argv[])
{
	FILE *outputFilePointer;
	long double offsetRGB = 0.0;
	int mapType = 0, loopCounter = 0;
	char prefix[11], mainFileName[101];

	// screen output and check

	system("clear");

	printf("\nMandelbrot calculation tool www.dwell.at\n\n");

	if (argc > 1)
	{
		printf("tool needs no options, check SETUP.txt for infos\n");
		exit(EXIT_FAILURE);
	}

	// setup from textfile, some check, screen output

	printf("configuration from SETUP.txt\n");

	if (readSETUPtxt(&width, &height, &mode, &realOffset, &imagOffset, &zoomStart, &maxIteration, &mapType, &offsetRGB, prefix) != 1)
	{
		printf("problems reading setup file, check syntax\n");
		exit(EXIT_FAILURE);
	}

	if (width < MINWIDTH || height < MINHEIGHT || width > MAXWIDTH || height > MAXHEIGHT)
	{
		printf("picture size limitations width[min %d][max %d] height[min %d][max %d]\n", MINWIDTH, MAXWIDTH, MINHEIGHT, MAXHEIGHT);
		exit(EXIT_FAILURE);
	}

	/*	if (realOffset > 1 || imagOffset > 1)*/
	/*	{*/
	/*		printf("mistakes with zoom and offset values, check config\n");*/
	/*		exit(EXIT_FAILURE);*/
	/*	}*/

	if (mapType < 1 || mapType > 15)
	{
		printf("mistakes with map type, check config\n");
		exit(EXIT_FAILURE);
	}
	if (mode < 1)
	{
		printf("at least one rendereing\n");
		exit(EXIT_FAILURE);
	}

	printf("%d x %d pixel\n", width, height);
	printf("realoffset %Lf\n", realOffset);
	printf("imagoffset %Lf\n", imagOffset);
	printf("zoomstart  %LE\n", zoomStart);
	printf("iterations %d\n", maxIteration);
	printf("color map %d\n", mapType);
	printf("color factor %Lf\n", offsetRGB);
	printf("prefix to filename %s-xyz.ppm\n", prefix);
	printf("pictures to calculate %d\n\n", mode);

	printf("press enter to start or CTRL-C to cancel > ");
	getchar();
	printf("\n");

	// calloc enough place for colormap and initialize

	colorMapEasy = (unsigned char **) calloc(maxIteration, sizeof(unsigned char *));
	if (colorMapEasy == NULL)
	{
		printf("calloc error\n");
		exit(EXIT_FAILURE);
	}
	for (int i = 0; i < maxIteration; i++)
	{
		colorMapEasy[i] = (unsigned char *) calloc(3, sizeof(unsigned char));
		if(colorMapEasy[i] == NULL)
		{
			printf("calloc error\n");
			free(colorMapEasy);
			colorMapEasy = NULL;
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

	// IPC setup, GENERATOR creates all necessary IDs

	signal(SIGINT, ctrlChandlingGenerator);

	while(loopCounter < mode)
	{
		loopCounter++;
		zoomInfo = (4.0 / pow(2, loopCounter));

		// main mandelbrot calculations and color mapping

		TIMEVAL tCalcStart, tCalcEnd;
		pthread_t calcThreadA, calcThreadB, calcThreadC, calcThreadD;
		pthread_attr_t attributeA, attributeB, attributeC, attributeD;
		CODETHREAD *codeMatrixThreatA = (CODETHREAD *) malloc(sizeof(CODETHREAD));
		CODETHREAD *codeMatrixThreatB = (CODETHREAD *) malloc(sizeof(CODETHREAD));
		CODETHREAD *codeMatrixThreatC = (CODETHREAD *) malloc(sizeof(CODETHREAD));
		CODETHREAD *codeMatrixThreatD = (CODETHREAD *) malloc(sizeof(CODETHREAD));
		int pixelDivider = 0, threadStatus = 0;
		double timeCalc;

		gettimeofday(&tCalcStart, NULL);

		pixelDivider = width * height / 4;

		codeMatrixThreatA->pixel = 0;
		codeMatrixThreatA->startHeight = 0;
		codeMatrixThreatA->endHeight = height / 4;

		codeMatrixThreatB->pixel = pixelDivider;
		codeMatrixThreatB->startHeight = codeMatrixThreatA->endHeight;
		codeMatrixThreatB->endHeight = codeMatrixThreatA->endHeight + height / 4;

		codeMatrixThreatC->pixel = pixelDivider * 2;
		codeMatrixThreatC->startHeight = codeMatrixThreatB->endHeight;
		codeMatrixThreatC->endHeight = codeMatrixThreatB->endHeight + height / 4;

		codeMatrixThreatD->pixel = pixelDivider * 3;
		codeMatrixThreatD->startHeight = codeMatrixThreatC->endHeight;
		codeMatrixThreatD->endHeight = height;

		pthread_attr_init(&attributeA);
		threadStatus += pthread_create(&calcThreadA, &attributeA, calcStripeThread, codeMatrixThreatA);
		printf("thrA ");
		pthread_attr_init(&attributeB);
		threadStatus += pthread_create(&calcThreadB, &attributeB, calcStripeThread, codeMatrixThreatB);
		printf("thrB ");
		pthread_attr_init(&attributeC);
		threadStatus += pthread_create(&calcThreadC, &attributeC, calcStripeThread, codeMatrixThreatC);
		printf("thrC ");
		pthread_attr_init(&attributeD);
		threadStatus += pthread_create(&calcThreadD, &attributeD, calcStripeThread, codeMatrixThreatD);
		printf("thrD \n");

		threadStatus -= pthread_join(calcThreadA, NULL);
		threadStatus -= pthread_join(calcThreadB, NULL);
		threadStatus -= pthread_join(calcThreadC, NULL);
		threadStatus -= pthread_join(calcThreadD, NULL);

		gettimeofday(&tCalcEnd, NULL);
		timeCalc = (tCalcEnd.tv_sec+tCalcEnd.tv_usec*0.000001)-(tCalcStart.tv_sec+tCalcStart.tv_usec*0.000001);
		printf("\n%d err. %f s calc. time\n", threadStatus, timeCalc);

		free(codeMatrixThreatA);
		free(codeMatrixThreatB);
		free(codeMatrixThreatC);
		free(codeMatrixThreatD);

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

		if (writePPM6(pixelRGBNumberBuffer, outputFilePointer, width, height, 255) != 0)
		{
			perror("error writing output file");
			EXIT1
			exit(EXIT_FAILURE);
		}
		else
		{
			printf("%3d stored %s - CTRL-C to quit\n\n", loopCounter, mainFileName);
		}

		if(fclose(outputFilePointer) != 0)
		{
			printf("\nerror closing output file\n");
			EXIT1
			exit(EXIT_FAILURE);
		}
		outputFilePointer = NULL;
	}

	exit(EXIT_SUCCESS);
}
