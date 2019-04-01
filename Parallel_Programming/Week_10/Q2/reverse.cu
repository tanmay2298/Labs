#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 100

__global__ void CUDAStrCopy(char *str, int *len)
{
	int i = blockIdx.x;

	char temp = str[i];
	str[i] = str[*len - i - 1];
	str[*len - i - 1] = temp;
}

int main()
{
	char str[N];
	printf("Enter the string: ");
	scanf("%s", str);

	char *pstr;
	int len = strlen(str);
	int *plen;
	char str2[len];

	cudaMalloc((void**)&pstr, len * sizeof(char));
	cudaMalloc((void**)&plen, sizeof(int));
	cudaMemcpy(pstr, str, len * sizeof(char), cudaMemcpyHostToDevice);
	cudaMemcpy(plen, &len, sizeof(int), cudaMemcpyHostToDevice);

	CUDAStrCopy<<<len, 1>>>(pstr, plen);

	cudaMemcpy(str, pstr, len * sizeof(char), cudaMemcpyDeviceToHost);

	printf("Reversed String is: %s\n", str);

	cudaFree(pstr);
	cudaFree(plen);

	printf("\n");

		return 0;
}
