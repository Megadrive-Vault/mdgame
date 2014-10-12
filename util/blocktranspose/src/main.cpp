#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#include <errno.h>

#define BYTES_PER_BLOCK 32

#define BLOCK_SIZE_ARG 1
#define ROWS_ARG 2
#define COLUMNS_ARG 3
#define INFILE_ARG 4
#define OUTFILE_ARG 5

int main(int argc, char **argv)
{
	if(argc != 6)
	{
		printf("Usage: blocktranspose block_size rows columns infile outfile\n");
		return -1;
	}

	unsigned int block_count = atoi(argv[BLOCK_SIZE_ARG]);
	if((block_count < 1) || (block_count > 4))
	{
		printf("Error: Block size must be in range 1-4\n");
		return -1;
	}
	size_t block_size = BYTES_PER_BLOCK * block_count;

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
	char *mat = (char *)malloc(sizeof(*mat) * block_size * rows * columns);
	if(mat == NULL)
	{
		printf("Error: Couldn't allocate memory for input matrix\n");
		return -1;
	}

	size_t elems_read = fread(mat, block_size, elems, infile);
	if(elems_read != elems)
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
			if(fwrite((mat + (i * block_size) + (j * columns * block_size)), block_size, 1, outfile) != block_size)
			{
				printf("Error: Write fragmentation\n");
				free(mat);
				fclose(infile);
				fclose(outfile);
				return -1;
			}
		}
	}

	free(mat);
	fclose(infile);
	fclose(outfile);

	return 0;
}
