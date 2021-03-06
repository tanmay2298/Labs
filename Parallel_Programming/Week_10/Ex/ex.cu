#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <stdio.h>
#include <stdlib.h>
#define N 5

__global__ void CUDAStrCopy(char *A, char C[N])
{
	int i = threadIdx.x;
	C[i] = A[i] - 32;
	printf("%c\t", C[i]);
}

int main()
{
	char A[N];
	char C[N];
	char *pa, *pc;

	for(int i = 0; i < N; i++)
		A[i] = 'a';
	printf("C = \n");
	cudaEvent_t start, stop;
	cudaEventCreate(&start);
	cudaEventCreate(&stop);
	cudaEventRecord(start, 0);
	cudaMalloc((void**)&pa, N * sizeof(char));
	cudaMalloc((void**)&pc, N * sizeof(char));
	cudaMemcpy(pa, A, N * sizeof(char), cudaMemcpyHostToDevice);
	cudaError_t error = cudaGetLastError();

	if(error != cudaSuccess)
		printf("CUDA Error1: %s\n", cudaGetErrorString(error));
	CUDAStrCopy<<<1, N>>>(pa, pc);
	error = cudaGetLastError();

	if(error != cudaSuccess)
		printf("CUDA Error2: %s\n", cudaGetErrorString(error));

	cudaMemcpy(C, pc, N * sizeof(char), cudaMemcpyDeviceToHost);
	cudaEventRecord(stop, 0);
	cudaEventSynchronize(stop);
	float elapsedTime;
	cudaEventElapsedTime(&elapsedTime, start, stop);
	int i;
	/*
		printf("Value of C in host after kernel execution\n");
		for(int i = 0; i < N; i++)
			printf("%c\n", C[i]);
	*/
	printf("Time taken = %f", elapsedTime);
	cudaFree(pa);
	cudaFree(pc);
	printf("\n");

	return 0;
}
