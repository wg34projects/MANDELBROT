#include "header.h"

// color Map as global variable to have access with CTRL-C handler

void colorMap(int maxIteration, int mapType, long double offsetRGB, unsigned char **colorMapEasy)
{
	int i = 0;

	for(i = 0; i < maxIteration; i++)
	{
		switch(mapType)
		{
		case 1: // red linear
		{
			colorMapEasy[i][0] = i * offsetRGB;
			colorMapEasy[i][1] = 0;
			colorMapEasy[i][2] = 0;
			break;
		}
		case 2: // green linear
		{
			colorMapEasy[i][0] = 0;
			colorMapEasy[i][1] = i * offsetRGB;
			colorMapEasy[i][2] = 0;
			break;
		}

		case 3: // blue linear
		{
			colorMapEasy[i][0] = 0;
			colorMapEasy[i][1] = 0;
			colorMapEasy[i][2] = i * offsetRGB;
			break;
		}
		case 4: // PT1 approach red
		{
			long double temp = llround(255.0 - 255.0 * exp(i / offsetRGB * (-1.0)));
			if (temp > 255.0)
			{
				temp = 255.0;
			}
			unsigned char value = (unsigned char)temp;
			colorMapEasy[i][0] = value;
			colorMapEasy[i][1] = 0;
			colorMapEasy[i][2] = 0;
			break;
		}
		case 5: // PT1 approach green
		{
			long double temp = llround(255.0 - 255.0 * exp(i / offsetRGB * (-1.0)));
			if (temp > 255.0)
			{
				temp = 255.0;
			}
			unsigned char value = (unsigned char)temp;
			colorMapEasy[i][0] = 0;
			colorMapEasy[i][1] = value;
			colorMapEasy[i][2] = 0;
			break;
		}
		case 6: // PT1 approach blue
		{
			long double temp = llround(255.0 - 255.0 * exp(i / offsetRGB * (-1.0)));
			if (temp > 255.0)
			{
				temp = 255.0;
			}
			unsigned char value = (unsigned char)temp;
			colorMapEasy[i][0] = 0;
			colorMapEasy[i][1] = 0;
			colorMapEasy[i][2] = value;
			break;
		}
		case 7: // pending sinus approach red
		{
			long double temp = llround(255.0/2.0*sin(i/(offsetRGB)-1.57079632679)+255.0/2.0);
			if (temp > 255.0)
			{
				temp = 255.0;
			}
			unsigned char value = (unsigned char)temp;
			colorMapEasy[i][0] = value;
			colorMapEasy[i][1] = 0;
			colorMapEasy[i][2] = 0;
			break;
		}
		case 8: // pending sinus approach green
		{
			long double temp = llround(255.0/2.0*sin(i/(offsetRGB)-1.57079632679)+255.0/2.0);
			if (temp > 255.0)
			{
				temp = 255.0;
			}
			unsigned char value = (unsigned char)temp;
			colorMapEasy[i][0] = 0;
			colorMapEasy[i][1] = value;
			colorMapEasy[i][2] = 0;
			break;
		}
		case 9: // pending sinus approach blue
		{
			long double temp = llround(255.0/2.0*sin(i/(offsetRGB)-1.57079632679)+255.0/2.0);
			if (temp > 255.0)
			{
				temp = 255.0;
			}
			unsigned char value = (unsigned char)temp;
			colorMapEasy[i][0] = 0;
			colorMapEasy[i][1] = 0;
			colorMapEasy[i][2] = value;
			break;
		}
		case 10: // pending sinus approach cyan
		{
			long double temp = llround(255.0/2.0*sin(i/(offsetRGB)-1.57079632679)+255.0/2.0);
			if (temp > 255.0)
			{
				temp = 255.0;
			}
			unsigned char value = (unsigned char)temp;
			colorMapEasy[i][0] = 0;
			colorMapEasy[i][1] = value;
			colorMapEasy[i][2] = value;
			break;
		}
		default: // b&w
		{
			colorMapEasy[i][0] = 0;
			colorMapEasy[i][1] = 0;
			colorMapEasy[i][2] = 0;
			break;
		}
		}
	}
}
