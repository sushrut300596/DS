#include "enc.h"
#include "dec.h"

int main ( int argc, char **argv)
{
	if (argc < 4)
	{
		printf("Using: name_programm <input_of_file> <output_file>\n");
		return -1;
	}
	if (argv[1][0] == 'e')
	{
		encode(argv[2], argv[3]);
	}
	else if (argv[1][0] == 'd')
	{
		decode(argv[2], argv[3]);
	}
	return 0;
}
