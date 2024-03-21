// by Erik Wrenholt
#include <stdio.h>
#include <time.h>

#define BAILOUT 16
#define MAX_ITERATIONS 100000

int mandelbrot(double x, double y)
{
	double cr = y - 0.5;
	double ci = x;
	double zi = 0.0;
	double zr = 0.0;
	int i = 0;

	while (1)
	{
		i++;
		double temp = zr * zi;
		double zr2 = zr * zr;
		double zi2 = zi * zi;
		zr = zr2 - zi2 + cr;
		zi = temp + temp + ci;
		if (zi2 + zr2 > BAILOUT)
			return i;
		if (i > MAX_ITERATIONS)
			return 0;
	}
}

int main(int argc, const char *argv[])
{
	struct timespec start;
	struct timespec stop;
	clock_gettime(CLOCK_MONOTONIC_RAW, &start);

	int x, y;
	for (y = -39; y < 39; y++)
	{
		printf("\n");
		for (x = -39; x < 39; x++)
		{
			int i = mandelbrot(x / 40.0, y / 40.0);
			if (i == 0)
				printf("*");
			else
				printf(" ");
		}
	}
	printf("\n");

	clock_gettime(CLOCK_MONOTONIC_RAW, &stop);

	double query_time = (stop.tv_sec - start.tv_sec) * 1000 + (stop.tv_nsec - start.tv_nsec) / 1000000.0;
	printf("C Elapsed %.2f ms\n", query_time);
	return 0;
}
