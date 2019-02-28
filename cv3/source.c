#include "omp.h"
#include "stdio.h"
#define N 10

int main() {
// NOTE: uncomment only portions bounded by " ==== "
// ======================================================
// vsetko, co sa spusti v "{ ... }" bezi v omp
// vytvoria sa 4 thready, na kazdom sa vypise string
 /* 
 #pragma omp parallel 
	{
		printf_s("Hello world\n");
	}
*/
// ======================================================
// spusti sa na definovanom pocte threadov
/*
#pragma omp parallel num_threads(2)
	{
		printf_s("Hello world\n");
	}
*/
// ======================================================
	// globalne pre cely main

/*
	omp_set_num_threads(2);

#pragma omp parallel 
	{
		printf_s("num threads = %d, from thread %d\n", omp_get_num_threads(), omp_get_thread_num());
	}
*/
// ======================================================
/*
	omp_set_num_threads(2);

#pragma omp parallel 
	{
#pragma omp master // spusti iba na mastri, t.j: vlakno 0. 
		printf_s("num threads = %d, from thread %d\n", omp_get_num_threads(), omp_get_thread_num());
	}
*/
// ======================================================
/*
	omp_set_num_threads(2);

#pragma omp parallel 
	{
#pragma omp single // iba na jednom vlakne
		printf_s("num threads = %d, from thread %d\n", omp_get_num_threads(), omp_get_thread_num());
	}
*/
// ======================================================
/*
	omp_set_num_threads(2);

	int i_shared = 0;
#pragma omp parallel 
	{
		int i;
		for (i = 0; i < N; i++) {
			printf_s("i (private) = %d, i_shared = %d, from thread %d\n", i, i_shared, omp_get_thread_num());
			i_shared++;
		}
	}
*/
// ======================================================
/*
	omp_set_num_threads(2);

	int i_shared = 0;
#pragma omp parallel 
	{
		int i;
#pragma omp for // spusti sa for cyklus paralelne
		for (i = 0; i < N; i++) {
			printf_s("i (private) = %d, i_shared = %d, from thread %d\n", i, i_shared, omp_get_thread_num());
			i_shared++;
		}
	}
*/
// ======================================================
/*
	omp_set_num_threads(2);

	int i;
#pragma omp parallel 
	{		
#pragma omp for
		for (i = 0; i < N; i++) {
			// definovanim i pred omp for bude i private
			printf_s("i = %d, from thread %d\n", i, omp_get_thread_num());
		}
	}
*/
// ======================================================
// skrateny parallel for cyklus:
/*
	omp_set_num_threads(2);

	int i;
#pragma omp parallel for
	for (i = 0; i < N; i++) {
		printf_s("i = %d, from thread %d\n", i, omp_get_thread_num());
	}
*/
// ======================================================
/*
	omp_set_num_threads(2);

	int i, j = 0;
#pragma omp parallel private(j)
	{
#pragma omp for
		for (i = 0; i < N; i++) {
			printf_s("i = %d, j = %d, from thread %d\n", i, j, omp_get_thread_num());
			j++;
		}
	}
*/
// ======================================================
/*
	omp_set_num_threads(2);

	int i, j;
	#pragma omp parallel private(j) // musi byt private
	{
		#pragma omp for
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				printf_s("i = %d, j = %d, from thread %d\n", i, j, omp_get_thread_num());
			}			
		}
	}
*/
// ======================================================
/*
	omp_set_num_threads(2);

	int i, pom = 10; // private (pom) clausule neskompiluje
	#pragma omp parallel firstprivate(pom)
	{
	#pragma omp for
		for (i = 0; i < 5; i++) {
			pom *= (i + 1);
			printf_s("i = %d, pom = %d, from thread %d\n", i, pom, omp_get_thread_num());
		}
	}

	printf_s("pom = %d", pom);
*/
// ======================================================
/*
	omp_set_num_threads(2);

	int i, pom = 10; // private (pom) clausule neskompiluje
	#pragma omp parallel 
	{
	#pragma omp for firstprivate(pom) lastprivate(pom)
		for (i = 0; i < 5; i++) {
			pom *= (i + 1);
			printf_s("i = %d, pom = %d, from thread %d\n", i, pom, omp_get_thread_num());
		}
	}

	printf_s("pom = %d", pom);
*/
// ======================================================
/*
	omp_set_num_threads(2);

	int i, j = 0;
	#pragma omp parallel for
	for (i = 0; i < 5; i++) {
		printf_s("i = %d, pom = %d, from thread %d\n", i, omp_get_thread_num());
		#pragma omp atomic
		j++;
	}

	printf_s("%d", j);
*/
// ======================================================
/*
	omp_set_num_threads(2);

	int a = 0;
#pragma omp parallel reduction(+:a)
{
		a = 1;
}

printf_s("%d", a);
*/
// ======================================================
/*
	omp_set_num_threads(5);

	int i;

	#pragma omp parallel for schedule(dynamic) // schedule(static, 3)
	for (i = 0; i < N; i++) {
		printf_s("%d from thread %d \n", i, omp_get_thread_num());
	}
*/
// ======================================================
/*
	omp_set_num_threads(5);
	int i;

	#pragma omp parallel for ordered schedule (dynamic)
	for (i = 0; i < N; i++) {
		#pragma omp ordered
		printf_s("%d from thread %d \n", i, omp_get_thread_num());
	}
*/
// ======================================================
	omp_set_num_threads(5);

	size_t P[N * N];
	int i, j;
	size_t max = 0;

	#pragma omp parallel for private(j)
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			P[(i * N) + j] = i * j;
			// oznacuje blok, kt. sa vykona sekvencne, pouzitelne napr. pri zapise do zdielanej premennej
			#pragma omp critical
			{
				if (P[(i * N) + j] > max) {
					max = P[(i * N) + j];
				}
			}
		}
	}

	printf("max = %d", max);

	getchar();
}