#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void toggle(char *str)
{
	int i;
	for(i = 0; i < strlen(str); i++)
	{
		if(str[i] > 65 && str[i] < 90)
			str[i] += 32;
		else
			str[i] -= 32;
	}
}

int main(int argc, char* argv[])
{
	int rank, size;
	char *x = (char*)malloc(100 * sizeof(char));
	MPI_Status status;
	int len = 0;

	MPI_Init(&argc, &argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if(rank == 0)
	{
		printf("Enter a string: ");
		scanf("%s", x);
		len = strlen(x);
		len++;
		MPI_Ssend(&len, 1, MPI_INT, 1, 1, MPI_COMM_WORLD);
		MPI_Ssend(x, len, MPI_CHAR, 1, 1, MPI_COMM_WORLD);
		fprintf(stdout, "I have send %s from process 0\n", x);
		fflush(stdout);

		MPI_Recv(x, len, MPI_CHAR, 1, 1, MPI_COMM_WORLD, &status);
		fprintf(stdout, "Received toggled %s from process 1\n", x);
		fflush(stdout);
	}
	else
	{
		MPI_Recv(&len, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
		MPI_Recv(x, len, MPI_CHAR, 0, 1, MPI_COMM_WORLD, &status);
		fprintf(stdout, "I have received %s in process 1\n", x);
		fprintf(stdout, "Toggling\n");
		fflush(stdout);
		toggle(x);
		fprintf(stdout, "Sending to process 0 %s\n", x);
		fflush(stdout);
		MPI_Ssend(x, len, MPI_CHAR, 0, 1, MPI_COMM_WORLD);
	}

	MPI_Finalize();
	return 0;
}
