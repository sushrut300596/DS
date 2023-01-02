#include "comp.h"

int low, high;
int value;

int buffer;
int	bits_in_buf;
int end_decoding;

void decode_start()
{
	buffer = 0;
	bits_in_buf = 0;
	end_decoding = 0;

	low = 0;
	high = MAX_VALUE;
}

/*Decode::~Decode(void)
{
}*/

int get_bit()
{
	int t;
	if (bits_in_buf == 0)
	{
		buffer = getc(fp2);
		if (buffer == EOF)
		{
			end_decoding = 1;
			return -1;
		}
		bits_in_buf= 8;
	}
	t = buffer & 1;
	buffer >>= 1;
	bits_in_buf--;
	return t;
}

void load_first_value()
{
	value = 0;
	for (int i = 1; i <= CODE_VALUE; i++)
		value = 2 * value + get_bit();
}

int decode_symbol()
{
	int range;
	int cum; 
	int symbol_index;

	range = high - low;
	cum = ((((value - low) + 1) * cum_freq[0] - 1) / range); // +- Нужно для то ли округления то ли преобразования, но иначе не работает
	for (symbol_index = 1; cum_freq[symbol_index] > cum; symbol_index++);
	high = low + (range * cum_freq[symbol_index - 1]) / cum_freq[0];
	low = low + (range * cum_freq[symbol_index]) / cum_freq[0];
	for (;;)
	{
		if (high < HALF)
		{
		}
		else if (low >= HALF)
		{
			value -= HALF;
			low -= HALF;
			high -= HALF;
		}
		else if (low >= FIRST_QTR && high < THIRD_QTR)
		{
			value -= FIRST_QTR;
			low -= FIRST_QTR;
			high -= FIRST_QTR;
		}
		else
			break;
		low = 2 * low;
		high = 2 * high;
		value = 2 * value + get_bit();
	}
	return symbol_index;
}

void decode(char *infile, char *outfile)
{
	fp2 = fopen(infile, "rb");
	fp3 = fopen(outfile, "wb");
	load_first_value();
	while (1)
	{
		int ch;
		int sym_index;
		sym_index = decode_symbol();
		if ((sym_index == EOF_SYMBOL) || end_decoding == 1)
			break;
		ch = index_to_char[sym_index];
		putc(fp3, ch);
		update_tables(sym_index);
	}
	printf("Decoding is done\n");
	fclose(fp2);
	fclose(fp3);
}



