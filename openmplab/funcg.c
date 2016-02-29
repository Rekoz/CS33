#include "func.h"
#include "util.h"

void func0(double *weights, double *arrayX, double *arrayY, int xr, int yr, int n)
{
	int i;
	double w = 1/((double)(n));
	#pragma omp parallel for
	for(i = 0; i < n; i++){
		weights[i] = w;
		arrayX[i] = xr;
		arrayY[i] = yr;
	}
}

void func1(int *seed, int *array, double *arrayX, double *arrayY,
			double *probability, double *objxy, int *index,
			int Ones, int iter, int X, int Y, int Z, int n)
{
	int i, j;
	int max_size = X*Y*Z, YZ = Y*Z, dO = (double) Ones;

	#pragma omp parallel for private(j)
	for(i = 0; i<n; i++){
		arrayX[i] += 1 + 5*rand2(seed, i);
		arrayY[i] += -2 + 2*rand2(seed, i);
		probability[i] = 0;
		int raX = round(arrayX[i]);
		int raY = round(arrayY[i]);
		for(j = 0; j < Ones; j++){
			int index_X = raX + objxy[j*2 + 1];
			int index_Y = raY + objxy[j*2];
			index[i*Ones + j] = fabs(index_X*YZ + index_Y*Z + iter);
			if(index[i*Ones + j] >= max_size)
				index[i*Ones + j] = 0;
			probability[i] += (pow((array[index[i*Ones + j]] - 100),2) -
							  pow((array[index[i*Ones + j]]-228),2))/50.0;
		}
		probability[i] = probability[i]/dO;
	}
}

void func2(double *weights, double *probability, int n)
{
	int i;
	double sumWeights=0;

	#pragma omp parallel for reduction(+:sumWeights)
	for(i = 0; i < n; i++) {
		weights[i] = weights[i] * exp(probability[i]);
		sumWeights += weights[i];
	}

	#pragma omp parallel for
	for(i = 0; i < n; i++)
		weights[i] = weights[i]/sumWeights;
}

void func3(double *arrayX, double *arrayY, double *weights, double *x_e, double *y_e, int n)
{
	double estimate_x=0.0;
	double estimate_y=0.0;
	int i;

	#pragma omp parallel for reduction(+:estimate_x, estimate_y)
	for(i = 0; i < n; i++){
		estimate_x += arrayX[i] * weights[i];
		estimate_y += arrayY[i] * weights[i];
	}

	*x_e = estimate_x;
	*y_e = estimate_y;

}

void func4(double *u, double u1, int n)
{
	int i;
	double dn = (double)n;

	#pragma omp parallel for
	for(i = 0; i < n; i++){
		u[i] = u1 + i/dn;
	}
}

void func5(double *x_j, double *y_j, double *arrayX, double *arrayY, double *weights, double *cfd, double *u, int n)
{
	int i, j;
	#pragma omp parallel for private(i)
	for(j = 0; j < n; j++){
		i = findIndexBin(cfd, 0, n, u[j]);
		if(i == -1)
			i = n-1;
		x_j[j] = arrayX[i];
		y_j[j] = arrayY[i];
	}

	double w = 1/((double)(n));
	#pragma omp parallel for
	for(i = 0; i < n; i++){
		arrayX[i] = x_j[i];
		arrayY[i] = y_j[i];
		weights[i] = w;
	}
}
