#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

#include <errno.h>

#define BYTES_PER_BLOCK 32

#define ROWS_ARG 1
#define COLUMNS_ARG 2
#define INFILE_ARG 3
#define OUTFILE_ARG 4

int main(int argc, char **argv)
{
	if(argc != (OUTFILE_ARG + 1))
	{
		printf("Usage: blocktranspose rows columns infile outfile\n");
		return -1;
	}

	unsigned int rows = atoi(argv[ROWS_ARG]);
	if(rows <= 0)
	{
		printf("Error: Row count must be greater than zero\n");
		return -1;
	}

	unsigned int columns = atoi(argv[COLUMNS_ARG]);
	if(columns <= 0)
	{
		printf("Error: Column count must be greater than zero\n");
		return -1;
	}

	FILE *infile = fopen(argv[INFILE_ARG], "r");
	if(infile == NULL)
	{
		printf("Error: Couldn't open %s for reading\n", argv[INFILE_ARG]);
		return -1;
	}

	FILE *outfile = fopen(argv[OUTFILE_ARG], "w");
	if(outfile == NULL)
	{
		printf("Error: Couldn't open %s for writing\n", argv[OUTFILE_ARG]);
		return -1;
	}

	unsigned int elems = rows * columns;
	char *mat = (char *)malloc(sizeof(*mat) * BYTES_PER_BLOCK * rows * columns);
	if(mat == NULL)
	{
		printf("Error: Couldn't allocate memory for input matrix\n");
		return -1;
	}

	size_t elems_read = 0;

	while(1)
	{
		elems_read = fread(mat, BYTES_PER_BLOCK, elems, infile);
		if(elems_read == 0)
		{
			break;
		}
		else if(elems_read != elems)
		{
			printf("Error: fread() read %zu blocks when %d were expected\n", elems_read, elems);
			free(mat);
			fclose(infile);
			fclose(outfile);
			return -1;
		}

		for(unsigned int i = 0; i < rows; i++)
		{
			for(unsigned int j = 0; j < columns; j++)
			{
				if(!(fwrite((mat + (i * BYTES_PER_BLOCK) + (j * columns * BYTES_PER_BLOCK)), BYTES_PER_BLOCK, 1, outfile)))
				{
					printf("Error: Write fragmentation\n");
					free(mat);
					fclose(infile);
					fclose(outfile);
					return -1;
				}
			}
		}
	}

	free(mat);
	fclose(infile);
	fclose(outfile);

	return 0;
}
