#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	char kw[8][100] = {"int", "float", "break", "for", "continue", "if", "else", "FILE"};
	FILE *f1, *f2;
	int c1, c2;
	f1 = fopen("q3_copy.c", "r");
	if(f1 == NULL)
	{
		printf("Cannot open file\n");
		exit(0);
	}
	f2 = fopen("q3_output.c", "w");
	int line, word;
	int i, j, k;
	do
	{
		c1 = getc(f1);
		char buf[10];

		fgets(buf, sizeof(buf), f1);
		for(i = 0; i < 8; i++)
		{
			if(strncmp(buf, kw[i], strlen(kw[i]) - 1) == 0)
		}
	}
}