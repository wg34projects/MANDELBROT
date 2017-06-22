#include "header.h"

void *calcStripeThread(void *code)
{
	int xIterate = 0, yIterate = 0, iteration = 0, pixelCounter = 0, realIteration = 0;
	long double realPart = 0.0, imagPart = 0.0, x = 0.0, y = 0.0, xNew = 0.0;
	CODETHREAD *codeDecode = (CODETHREAD *)code;

	pixelCounter = codeDecode->pixel;

	for (xIterate = codeDecode->startHeight; xIterate < codeDecode->endHeight; xIterate++)
	{
		for (yIterate = 0; yIterate < width; yIterate++)
		{
			if (mode == 1)
			{
				realPart = (yIterate - width  / 2.0) * zoomStart + realOffset;
				imagPart = (xIterate - height / 2.0) * zoomStart + imagOffset;
			}
			else if (mode == 2)
			{
				realPart = (yIterate - width  / 2.0) * 4.0 / width;
				imagPart = (xIterate - height / 2.0) * 4.0 / width;
			}
			else
			{
				realPart = (yIterate - width  / 2.0) * zoomInfo + realOffset;
				imagPart = (xIterate - height / 2.0) * zoomInfo + imagOffset;
			}

			x = 0.0, y = 0.0;
			iteration = 0;

			while ((pow(x,2)+pow(y,2)) <= 4 && iteration < maxIteration)
			{
				xNew = pow(x,2) - pow(y,2) + realPart;
				y = 2.0*x*y + imagPart;
				x = xNew;
				iteration++;
			}

			if (iteration < maxIteration)
			{
				pixelRGBNumberBuffer[pixelCounter*3+0] = colorMapEasy[iteration][0];
				pixelRGBNumberBuffer[pixelCounter*3+1] = colorMapEasy[iteration][1];
				pixelRGBNumberBuffer[pixelCounter*3+2] = colorMapEasy[iteration][2];
				if (iteration > realIteration)
				{
					realIteration = iteration;
				}
			}
			else
			{
				pixelRGBNumberBuffer[pixelCounter*3+0] = 0;
				pixelRGBNumberBuffer[pixelCounter*3+1] = 0;
				pixelRGBNumberBuffer[pixelCounter*3+2] = 0;
			}
			pixelCounter++;
		}
	}

	printf("%4d ", realIteration);
	pthread_exit(0);
}

int writePPM6(unsigned char *pixelValues, FILE *output, int width, int height, int bright)
{
	int rvfprintf = 0;

	rvfprintf = fprintf(output, "P6\n");
	if(rvfprintf < 0)
	{
		fprintf(stderr, "error writing to output file\n");
		return 1;
	}
	else
	{
		rvfprintf = 0;
	}

	rvfprintf = fprintf(output, "# PRG BEL2 sharedmemory semaphore task\n");
	if(rvfprintf < 0)
	{
		fprintf(stderr, "error writing to output file\n");
		return 1;
	}
	else
	{
		rvfprintf = 0;
	}

	rvfprintf = fprintf(output, "# created by el16b005 BEL1 helmut resch www.dwell.at\n");
	if(rvfprintf < 0)
	{
		fprintf(stderr, "error writing to output file\n");
		return 1;
	}
	else
	{
		rvfprintf = 0;
	}

	rvfprintf = fprintf(output, "%u %u\n", width, height);
	if(rvfprintf < 0)
	{
		fprintf(stderr, "error writing to output file\n");
		return 1;
	}
	else
	{
		rvfprintf = 0;
	}

	rvfprintf = fprintf(output, "%u\n", bright);
	if(rvfprintf < 0)
	{
		fprintf(stderr, "error writing to output file\n");
		return 1;
	}
	else
	{
		rvfprintf = 0;
	}

	for(int i = 0; i < width*height; i++)
	{
		rvfprintf = fprintf(output, "%c%c%c", pixelValues[i*3+0], pixelValues[i*3+1], pixelValues[i*3+2]);
		if(rvfprintf < 3) // 3 pixelvalues MUST be written
		{
			fprintf(stderr, "error writing to output file\n");
			return 1;
		}
		else
		{
			rvfprintf = 0;
		}
	}

	return 0;
}

void ctrlChandlingGenerator(int dummy)
{
	printf("\n\nyou typed CTRL-C...\n");
	EXIT1
	exit(EXIT_SUCCESS);
}

int readSETUPtxt(int *width, int *height, int *mode, long double *realOffset, long double *imagOffset, long double *zoomStart, int *maxIteration, int *mapType, long double *offsetRGB, char prefix[])
{
	FILE *inputFilePointer = NULL;
	char setupFile[] = "SETUP.txt\0";
	char line[300];
	fpos_t positionFilePointer;

	inputFilePointer = fopen(setupFile, "rb");
	if (inputFilePointer == NULL)
	{
		printf("\nerror opening setup file, check permissions\n");
		return 0;
	}

	while(!feof(inputFilePointer))
	{
		fgetpos(inputFilePointer, &positionFilePointer);
		fgets(line, 100, inputFilePointer);
		if(line[0]  != '#')
		{
			fsetpos(inputFilePointer, &positionFilePointer);
			fscanf(inputFilePointer, "%11s", prefix);
			fscanf(inputFilePointer, "%99d", width);
			fscanf(inputFilePointer, "%99d", height);
			fscanf(inputFilePointer, "%99d", mode);
			fscanf(inputFilePointer, "%999Lf", realOffset);
			fscanf(inputFilePointer, "%999Lf", imagOffset);
			fscanf(inputFilePointer, "%999LE", zoomStart);
			fscanf(inputFilePointer, "%99d", maxIteration);
			fscanf(inputFilePointer, "%99d", mapType);
			fscanf(inputFilePointer, "%99Lf", offsetRGB);
			break;
		}
	}
	if (fclose(inputFilePointer) != 0)
	{
		printf("\nerror closing setup, but continue\n");
	}
	prefix[strlen(prefix)] = '\0';

	return 1;
}

void help()
{
	system("clear");
	printf("\nwelcome to the help section, find some infos here, more in SETUP.txt\n\n");
}
