#include "comp.h"

int low, high;
int value;

unsigned char buffer;
int bits_in_buf;
int end_decoding;

FILE *fp2;
FILE *fp3;

void dstart_model()
{
	int i;
	for ( i = 0; i < NO_OF_CHARS; i++)
	{
		char_to_index[i] = i + 1;
		index_to_char[i + 1] = i;
	}
	for ( i = 0; i <= NO_OF_SYMBOLS; i++)
	{
		freq[i] = 1;
		cum_freq[i] = NO_OF_SYMBOLS - i;
	}
	freq[0] = 0;
}


void dupdate_tables(int sym_index)
{
  int i;
  if (cum_freq[0] == MAX_FREQ)
  {
    int cum = 0;
    for ( i = NO_OF_SYMBOLS; i >= 0; i--)
    {
      freq[i] = (freq[i] + 1) / 2;
      cum_freq[i] = cum;
      cum += freq[i];
    }
  }
  for ( i = sym_index; freq[i] == freq[i - 1]; i--);
  if (i < sym_index)
  {
	int ch_i, ch_symbol;
    ch_i = index_to_char[i];
    ch_symbol = index_to_char[sym_index];
    index_to_char[i] = ch_symbol;
    index_to_char[sym_index] = ch_i;
    char_to_index[ch_i] = sym_index;
    char_to_index[ch_symbol] = i;
  }
  freq[i]++;
  while (i > 0)
  {
	i--;
    cum_freq[i]++;
  }
}


void decode_start()
{
	buffer = 0;
	bits_in_buf = 0;
	end_decoding = 0;

	low = 0;
	high = MAX_VALUE;
}

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
		bits_in_buf = 8;
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
	cum = ((((value - low) + 1) * cum_freq[0] - 1) / range);
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
		high = 2 * high + 1;
		value = 2 * value + get_bit();
	}
	return symbol_index;
}

void decode(char *infile, char *outfile)
{
	fp2 = fopen(infile, "rb");
	fp3 = fopen(outfile, "w");
	decode_start();
	dstart_model();
	load_first_value();
	while (1)
	{
		int ch;
		int sym_index;
		sym_index = decode_symbol();
		// printf("%d %c\n", sym_index, sym_index);
		// putc(sym_index,stdout);
		// getchar();
		if ((sym_index == EOF_SYMBOL) || end_decoding)
			break;
		ch = index_to_char[sym_index];
		// putc(ch, fp3);
		fwrite(&ch, sizeof(char), 1, fp3);
		// fprintf(fp3, "%c", ch);
		dupdate_tables(sym_index);
	}
	printf("Decoding is done\n");
	fclose(fp2);
	fclose(fp3);
}



