#include "mpi.h"
#include <stdio.h>

int main(int argc, char* argv[])
{
	int rank, size, N, A[100];
	float 	B[10];
	int M;
	int temp[10];
	int i, j;
	MPI_Init(&argc, &argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if(rank == 0)
	{
		N = size;
		printf("Enter the value of M: ");
		scanf("%d", &M);
		printf("Enter values: \n");
		for(i = 0; i < N; i++)
			for(j = 0; j < M; j++)
				scanf("%d", &A[i*N + j]);
	}
	MPI_Bcast(&M, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Scatter(A, M, MPI_INT, &temp, M, MPI_INT, 0, MPI_COMM_WORLD);
	float c = 0;
	for(i = 0; i < M; i++)
		c += (float)(temp[i]/M);
	printf("Rank = %d\tAverage = %f\n", rank, c);
	MPI_Gather(&c, 1, MPI_FLOAT, B, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);

	if(rank == 0)
	{
		printf("The result gathered in the root\n");
		float avg = 0;
		for(i = 0; i < N; i++)
		{
			avg += B[i]/N;
			printf("%f\t", B[i]);
		}
		printf("Total Average: %f\n", avg);
	}
	MPI_Finalize();
}
