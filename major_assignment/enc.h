#include "comp.h"

int low, high;
int opposite_bits;
unsigned char buffer;
int	bits_in_buf;

FILE *fp;
FILE *fp1;

#define BUF 10000000

void start_model()
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


void update_tables(int sym_index)
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


void encode_start()
{
	buffer = 0;
	bits_in_buf = 0;

	low = 0;
	high = MAX_VALUE;
	opposite_bits = 0;
}

void write_bit(int bit)
{
	buffer >>= 1;
	if (bit) 
		buffer |= 0x80;
	bits_in_buf++;
	if (bits_in_buf == 8)
	{
		// putc(buffer, fp1);
		// fprintf(fp1, "%d", buffer);
		// printf("\n");
		fwrite(&buffer, sizeof(char), 1, fp1);
		bits_in_buf = 0;
	}
}

void output_bits(int bit)
{
	write_bit(bit);
	while (opposite_bits > 0)
	{
		write_bit(!bit);
		opposite_bits--;
	}
}

void end_encoding()
{
  opposite_bits++;
  if (low < FIRST_QTR)
    output_bits(0);
  else
    output_bits(1);
  unsigned char num = 0;
  num = buffer >> bits_in_buf;
  // putc(buffer >> bits_in_buf, fp1);
  // fprintf(fp1, "%d", buffer >> bits_in_buf);
  // printf("\n");
  fwrite(&num, sizeof(char), 1, fp1);
}

void encode_symbol(int symbol)
{
	int range;

	range = high - low;
	high = low + (range * cum_freq [symbol - 1]) / cum_freq [0];
	low = low + (range * cum_freq [symbol]) / cum_freq [0];
	for (;;)
	{
		if (high < HALF) {
			output_bits(0);
		}
		else if (low >= HALF)
		{
			output_bits(1);
			low -= HALF;
			high -= HALF;
		}
		else if (low >= FIRST_QTR && high < THIRD_QTR)
		{
			opposite_bits++;
			low -= FIRST_QTR;
			high -= FIRST_QTR;
		}
		else
			break;
		low = 2 * low;
		high = 2 * high + 1;
	}
}

void encode(char *infile, char *outfile)
{
	fp = fopen(infile, "r");
	fp1 = fopen(outfile, "wb");
	start_model();
	encode_start();
	while (1)
	{
		int ch;
		int symbol;
		ch = getc(fp);
		if (ch == EOF)
		{
			printf("Encoding is done\n");
			break;
		}
		symbol = char_to_index[ch];
		encode_symbol(symbol);
		update_tables(symbol);
	}
	encode_symbol(EOF_SYMBOL);
	end_encoding();
	fclose(fp);
	fclose(fp1);
}
