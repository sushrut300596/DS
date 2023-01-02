#include<stdio.h>
#include<stdlib.h>
#include"compress.h"

int low, high;
int opposite_bits;
unsigned int buffer;
int	bits_in_buf;
FILE *fp1;

void encode_start()
{
	buffer = 0;
	bits_in_buf = 0;

	low = 0;
	high = MAX_VALUE;
	opposite_bits = 0;
}

void encode(int *input, int slen)
{
	int i =0;
	while (i < slen)
	{
		int ch;
		int symbol;
		symbol = input[i];
		encode_symbol(symbol);
		update_tables(symbol);
		i++;
	}
	encode_symbol(EOF_SYMBOL);
	end_encoding();
}

void encode_symbol(int symbol)
{
	int range;

	range = high - low;
	high = low + (range * cum_freq [symbol - 1]) / cum_freq [0];
	low = low + (range * cum_freq [symbol]) / cum_freq [0];
	for (;;)
	{
	if (high < HALF)
		output_bits(0);
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
	high = 2 * high;
	}
}

void end_encoding()
{
  opposite_bits++;
  if (low < FIRST_QTR)
    output_bits(0);
  else
    output_bits(1);

  //out.put(buffer >> bits_in_buf);
  fprintf(fp1, "%d",buffer>>bits_in_buf );
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

void write_bit(int bit)
{
	buffer >>= 1;
	if (bit)
	buffer |= 0x80;
	bits_in_buf++;
	if (bits_in_buf == 8)
	{
		//out.put(buffer);
		fprintf(fp1,"%d",buffer)
		bits_in_buf = 0;
	}
}
