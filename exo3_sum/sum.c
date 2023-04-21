#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "omp.h"

typedef struct {double local_sum; char pad[128];} tval;

// array of size 10^7
const size_t size = 10000000;
const size_t iterations = 25; //how many times we repeat measurments of a certain function
double *array;




void generate_random(double *input, size_t size)
{
  for (size_t i = 0; i < size; i++) {
    input[i] = rand() / (double)(RAND_MAX);
  }
}


double serial_sum(double *x, size_t size)
{
  double sum_val = 0.0;

  for (size_t i = 0; i < size; i++) {
    sum_val += x[i];
  }

  return sum_val;
}

double omp_sum(double *x, size_t size)
{
	double sum_val = 0.0;
	#pragma omp parallel for 
	{
		for (size_t i = 0; i < size; i++) {
			sum_val += x[i];
  		}
	}
	

  return sum_val;
}

double omp_critical_sum(double *x, size_t size){
	double sum_val = 0.0;
	#pragma omp parallel for 
	{
		for (size_t i = 0; i < size; i++) {
			#pragma omp critical
			{
			sum_val += x[i];				
			}
  		}
	}
	

  	return sum_val;
}

double omp_local_sum(double *x, size_t size){
	double sum_val = 0.0;
	int MAX_THREADS = omp_get_max_threads();
	double local_sum[MAX_THREADS];

	#pragma omp parallel 
	{
		int id = omp_get_thread_num();
		local_sum[id] = 0.0;
		#pragma omp for
		for(int i = 0; i < size; ++i) {
			local_sum[id] += x[i];
		}
		
	}
	for (int i = 0; i < MAX_THREADS; ++i)
	{
		sum_val += local_sum[i]; 
	}
	return sum_val;
}


double opt_local_sum(double *x, size_t size){
	double sum_val = 0.0;
	int MAX_THREADS = omp_get_max_threads();
	tval local_sums[MAX_THREADS];

	#pragma omp parallel shared(local_sums)
	{
		int id = omp_get_thread_num();
		local_sums[id].local_sum = 0.0;
		#pragma omp for
		for(int i = 0; i < size; ++i) {
			local_sums[id].local_sum += x[i];
		}
		
	}
	for (int i = 0; i < MAX_THREADS; ++i)
	{
		sum_val += local_sums[i].local_sum; 
	}
	return sum_val;
}

void mesure_performance(double (*measured_function)(double *x, size_t size), size_t threads_num){
	/**
	*
	* computes the avg time + std of 'iterations' times of the @measured_function
	* using threads_num thread
	**/

	//assert(threads_num <= omp_get_max_threads());
	omp_set_num_threads(threads_num);



	double execution_times[iterations];	
	double start_time, end_time;
	double total_time = 0, average_time, variance = 0, std_deviation;
	double res = 0;

	for (int i = 0; i < iterations; ++i)
	{
		start_time = omp_get_wtime();
		res = measured_function(array, size);
		end_time = omp_get_wtime();
		execution_times[i] = end_time - start_time;
		total_time += execution_times[i];
	}

	 average_time = total_time / iterations;
	 // Calculate variance and standard deviation
    for (int i = 0; i < iterations; i++) {
        variance += (execution_times[i] - average_time) * (execution_times[i] - average_time);
    }
    variance /= iterations;
    std_deviation = sqrt(variance);
    printf("	Sum == %f, Average execution time %f s , std = %f s \n", res, average_time, std_deviation);
}

 int main(int argc, char const *argv[])
 {
 	array = (double*) malloc ( size * sizeof(double));
 	size_t threads_array_size;
 	if (array == NULL){
 		printf("Memory allocation failed");
    	exit(1);
 	}

 	generate_random(array, size);

 	printf("Testing serial code \n");
 	mesure_performance(&serial_sum, 1);
 	printf("--------------------------------- \n");


 	printf("Testing omp_sum  with 32 threads \n");
 	mesure_performance(&omp_sum, 32);
 	printf("--------------------------------- \n");

 	int threads_numbers_omp_critical[] = {1, 2, 4, 8, 16, 20, 24, 28};
 	threads_array_size = sizeof(threads_numbers_omp_critical) / sizeof(threads_numbers_omp_critical[0]);
 	for (int n = 0; n < threads_array_size ; n++)
 	{
 		printf("Testing omp_critical_sum  with %d threads \n", threads_numbers_omp_critical[n]);
 		mesure_performance(&omp_critical_sum, threads_numbers_omp_critical[n]);
 		printf("--------------------------------- \n");

 	}

	int threads_numbers_omp_local[] = {1, 32, 64, 128};
	threads_array_size =  sizeof(threads_numbers_omp_local) / sizeof(threads_numbers_omp_local[0]);
 	for (int n = 0; n < threads_array_size; n++)
 	{
 		printf("Testing omp_local_sum  with %d threads \n", threads_numbers_omp_local[n]);
 		mesure_performance(&omp_local_sum, threads_numbers_omp_local[n]);
 		printf("--------------------------------- \n");

 	}


 	int threads_numbers_opt_local [] = {1, 32, 64, 128};
	threads_array_size =  sizeof(threads_numbers_opt_local) / sizeof(threads_numbers_opt_local[0]);
 	for (int n = 0; n < threads_array_size; n++)
 	{
 		printf("Testing opt_local_sum  with %d threads \n", threads_numbers_opt_local[n]);
 		mesure_performance(&opt_local_sum, threads_numbers_omp_local[n]);
 		printf("--------------------------------- \n");

 	}


 	free(array);
 	return 0;
 }


