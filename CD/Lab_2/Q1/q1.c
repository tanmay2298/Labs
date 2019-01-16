#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int index_struct = 0;
struct Token
{
	char lexename[100];
	int index;
	int row, col;
	char type[100];
}Tkn[50];

void getNextToken(char lexename[], int index, int row, int col, char type[])
{
	strcpy(tkn[index_struct].lexename, lexename);
	tkn[index_struct].index = index;
	tkn[index_struct].row = row;
	tkn[index_struct].col = col;
	strcpy(tkn[index_struct].type, type);
}

int search_kw(char buf[100])
{
	char kw[12][100] = {"int", "if", "else", "FILE", "char", "struct", "for", "break", "continue", "float", "bool", "double"};
	for(int i = 0; i < 12; i++)
	{
		if(strcmp(buf, kw[i]) == 0)
			return i;
	}
	return -1;
}
int main()
{
	char kw[12][100] = {"int", "if", "else", "FILE", "char", "struct", "for", "break", "continue", "float", "bool", "double"};
	FILE *f1;
	f1 = fopen("q1_copy.c", "r");
	if(f1 == NULL)
	{
		printf("Cannot open file\n");
		exit(0);
	}
	int c1;
	char buf[100];
	int line = 1, col = 1;
	int i, j, k;
	int flag;
	c1 = getc(f1);
	int index = 0;
	while(c1 != EOF)
	{
		char type[100];
		if(c1 == '+' || c1 == '-' || c1 == '/' || c1 == '*' || c1 == '%')
		{
			buf[index++] = c1;
			buf[index] = '\0';
			strcpy(type, "Arithmetic Operator");
			col++;
			getNextToken(buf, index, line, col, type);
			index = 0;
		}
		else if(c1 == '=')
		{
			buf[index++] = c1;
			c1 = getc(f1);
			col++;
			if(c1 == '=')
			{
				buf[index++] = c1;
				buf[index] = '\0';
				col++;
				strcpy(type, "Relational Operator");
				getNextToken(buf, index, line, col, type);
				index = 0;		
			}
			else
			{
				buf[index] = '\0';
				strcpy(type, "Assignment Operator");
				col++;
				getNextToken(buf, index, line, col, type);
				index = 0;
			}
		}
		else if(c1 == '<' || c1 == '>' || c1 == '!') 
		{
			char temp = c1;
			buf[index++] = c1;
			col++;
			c1 = getc(f1);
			if(c1 == '=')
			{
				buf[index++] = c1;
				col++;
				buf[index] = '\0';
				strcpy(type, "Relational Operator");
				getNextToken(buf, index, line, col, type);
				index = 0;		
			}
			else if(c1 == ' ' && temp!= '!')
			{
				buf[index++] = c1;
				col++;
				buf[index] = '\0';
				strcpy(type, "Relational Operator");
				getNextToken(buf, index, line, col, type);
				index = 0;
			}
			else
			{
				buf[index] = '\0';
				strcpy(type, "Logical Operator");
				getNextToken(buf, index, line, col, type);
				index = 0;
			}
		}
		else if(c1 == '|' || c1 == '&')
		{
			buf[index++] = c1;
			c1 = getc(f1);
			col++;
			if(c1 == '&' || c1 == '&')
			{
				buf[index++] = c1;
				col++;
			}
			buf[index] = '\0';
			strcpy(type, "Logical Operator");
			getNextToken(buf, index, line, col, type);
			index = 0;
		}
		else if(isalpha(c1))
		{
			buf[index++] = c1;
			col++;
			c1 = getc(f1);
			while(isalpha(c1))
			{
				col++;
				buf[index++] = c1;
				c1 = getc(f1);
			}
			buf[index] = '\0';

			if(search_kw[buf] >= 0)
			{
				strcpy(type, "Keyword");
				getNextToken(buf, index, line, col, type);
				index = 0;
			}
			else
			{
				strcpy(type, "Identifier");
				getNextToken(buf, index, line, col, type);
				index = 0;
			}
		}
		else if()

	}	
}