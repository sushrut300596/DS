#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF 1000000

// FILE *fp;
FILE *fp1;
FILE *fp2;
char buf[BUF];

int n;

/* Declarations used for Arithmetic encoding and decoding */

/* Size of Arithmetic Code Values */

#define Code_value_bits 16  // Number of bits in a code value
typedef long double code_value;	// Type of an arithmetic code value

#define Top_value (((long)1 << Code_value_bits) - 1)	// Largest code value


/* Half and quarter points in the code value range */

#define First_qtr (Top_value / 4 + 1)	// Point after first quarter
#define Half (2 * First_qtr)	// Point after first half
#define Third_qtr (3 * First_qtr)	//Point after third quarter


/* Interface to the model */

/* The set of symbols that may be encoded */

#define No_of_chars 256 	// Number of character symbols
#define EOF_symbol (No_of_chars + 1)	// Index of EOF symbol

#define No_of_symbols (No_of_chars + 1)		// Total number of symbols


/* Translation tables between characters and symbol indexes */

int char_to_index[No_of_chars];		// To index from character
unsigned char index_to_char[No_of_symbols + 1];		// To character from index


/* Cumulative frequency table */

#define Max_frequency 16383 	// Maximum allowed frequency count

int cum_freq[No_of_symbols + 1];		// Cumulative symbol frequencies


/* The Adaptive Source Model */

int freq[No_of_symbols + 1];	// Symbol frequencies


/* Initialize the model */

void start_model() {
	int i;
	for(i = 0; i < No_of_chars; i++) {		// Set up tables that
		char_to_index[i] = i + 1;			// translate between symbol
		index_to_char[i + 1] = i;			// indexes and characters
	}
	for(i = 0; i <= No_of_symbols; i++) {		// Set up initial frequency
		freq[i] = 1;							// counts to be one for all
		cum_freq[i] = No_of_symbols - i;		// symbols
	}
	freq[0] = 0;		// freq[0] must not be the
}						// same as freq[1]


/* Update the model to account for a new symbol */

void update_model(int symbol) {		// Index of new symbol
	int i;		// New index for symbol
	if(cum_freq[0] == Max_frequency) {		// See if frequency counts
		int cum;							// are at their maximum
		cum = 0;
		for(i = No_of_symbols; i >= 0; i--) {	// If so, halve all the
			freq[i] = (freq[i] + 1) / 2;		// counts (keeping them
			cum_freq[i] = cum;					// non-zero)
			cum += freq[i];
		}
	}
	for(i = symbol; freq[i] == freq[i - 1]; i--);	// Find symbol's new index
	if(i < symbol) {
		int ch_i, ch_symbol;
		ch_i = index_to_char[i];			// Update the translation
		ch_symbol = index_to_char[symbol];	// tables if the symbol has
		index_to_char[i] = ch_symbol;		// moved
		index_to_char[symbol] = ch_i;
		char_to_index[ch_i] = symbol;
		char_to_index[ch_symbol] = i;
	}
	freq[i] += 1;			// Increment the frequency
	while(i > 0) {			// count for the symbol and
		i -= 1;				// update the cumulative
		cum_freq[i] += 1;	// frequencies
	}
}



/* Bit output routines */

/* The bit buffer */

static int buffer;		// Bits buffered for output
unsigned int bits_to_go;	// Number of bits free in buffer


/* Initialize for bit output */

void start_outputing_bits() {
	buffer = 0;			// Buffer is empty to start with
	bits_to_go = 8;
}


/* Output a bit */

void output_bit(int bit) {
	buffer >>= 1;		// Put bit in top of buffer
	//printf("%d\n", bit);
	if(bit)
		buffer |= 0x80;
	bits_to_go -= 1;
	if(bits_to_go == 0) {		// Output buffer if it is
		fputc(buffer, fp1);
		//fprintf(fp1, "%d\n", buffer);		// now full
		// fwrite(buffer, 1, n, fp1);
		printf("%d\n", buffer);
		bits_to_go = 8;
	}
}


/* Flush out the last bits */

void done_outputing_bits() {
	putc(buffer >> bits_to_go, fp1);
	// fprintf(fp1, "%d\n", buffer >> bits_to_go);
	// printf("%d\n", buffer >> bits_to_go);
}


/* Bit input routines */

/* The bit buffer */

static int bufferin;		// Bits waiting to be input
static int bits_in_go;	// Number of bits still in buffer
static int garbage_bits;	// Number of bits past EOF


/* Initialize bit input */

void start_inputing_bits() {
	bits_in_go = 0;			// Buffer starts out with
	garbage_bits = 0;		// no bits in it
}


/* Input a bit */

int input_bit() {
	int t;
	// int i = 0;
	if(bits_in_go == 0) {			// Read the next byte if no
		//while((fgets(buf, BUF, fp2)) != NULL) {			// bits are left in buffer
		// printf("%d\n", EOF);
			//bufferin = atoi(buf);
			//printf("%d\n", bufferin);
		bufferin = fgetc(fp2);
		if(bufferin == EOF) {
			garbage_bits += 1;
			if(garbage_bits > Code_value_bits - 2) {	// Return arbitrary bits
				printf("Bad input file\n");				// after eof, but check
				exit(-1);								// for too many such
			}
		}
		//i++;
		// }
		bits_in_go = 8;
	}
	t = bufferin & 1;		// Return the next bit from
	bufferin >>= 1;		// the bottom of the byte
	bits_in_go -= 1;
	return t;
}


/* Arithmetic encoding algorithm */

static void bit_plus_follow();		// Routine that follows


/* Current state of the encoding */

static code_value low, high;	// Ends of the current code region
static long bits_to_follow;		// Number of opposite bits to output after the next bit


/* Start encoding a stream of symbols */

void start_encoding() {
	low = 0;	// Full code range
	high = Top_value;
	bits_to_follow = 0;		// No bits to follow next
}


/* Encode a symbol */

void encode_symbol(int symbol, int cum_freq[]) {		// Symbol to encode and Cumulative symbol frequencies
	long double range;		// Size of the current code region
	range = (long double)(high - low) + 1;
	high = low + ((range * cum_freq[symbol - 1]) / cum_freq[0]) - 1;		// Narrow the code region
	low = low + (range * cum_freq[symbol]) / cum_freq[0];				// to that allotted to this symbol
	for(;;) {		// Loop to output bits
		if(high < Half) {		// Output 0 if in low half
			bit_plus_follow(0);
		}
		else if(low >= Half) {		// Output 1 if in high half
			bit_plus_follow(1);
			low -= Half;
			high -= Half;		// Subtract offset to top
		}
		else if((low >= First_qtr) && (high < Third_qtr)) {	// Output an opposite bit later if in middle half
			bits_to_follow += 1;
			low -= First_qtr;
			high -= First_qtr;		// Subtract offset to middle
		}
		else		// Otherwise exit loop
			break;
		low = 2 * low;
		high = 2 * high + 1;		// Scale up code range
	}
}


/* Finish encoding the stream */

void done_encoding() {				// Output two bits that
	bits_to_follow += 1;		// select the quarter that
	if(low < First_qtr)			// the current code range contains
		bit_plus_follow(0);
	else
		bit_plus_follow(1);
}


/* Output bits plus following opposite bits */

static void bit_plus_follow(int bit) {
	output_bit(bit);		// Output the bit
	while(bits_to_follow > 0) {
		output_bit(!bit);		// Output bits_to_follow
		bits_to_follow -= 1;	// opposite bits. Set
	}							// bits_to_follow to zero
}


/* Arithmetic decoding algorithm */

/* Current state of the decoding */

static code_value value;		// Currently-seen code value
static code_value low, high;	// Ends of current code region


/* Start decoding a stream of symbols */

void start_decoding() {
	int i;
	value = 0;
	for(i = 1; i <= Code_value_bits; i++) {		// Input bits to fill the
		value = 2 * value + input_bit();		// code value
	}
	low = 0;			// Full code range
	high = Top_value;
}


/* Decode the next symbol */

int decode_symbol(int cum_freq[]) {		// Cumulative symbol frequencies
	long double range;			// Size of current code region
	int cum, symbol;			// Cumulative frequency calculated and Symbol decoded
	range = (long double)(high - low) + 1;
	cum = (((long double)(value - low) + 1) * cum_freq[0] - 1) / range;		// Find cum freq for value
	for(symbol = 1; cum_freq[symbol] > cum; symbol++);		// Then find symbol
	high = low + (range * cum_freq[symbol - 1]) / cum_freq[0] - 1;		// Narrow the code region to that
	low = low + (range * cum_freq[symbol]) / cum_freq[0];				// allotted to this symbol
	for(;;) {					// Lopp to get rid of bits
		if(high < Half) {}		// Expand low half
		else if(low >= Half) {	// Expand high half
			value -= Half;
			low -= Half;		//Subtract offset to top
			high -= Half;
		}
		else if((low >= First_qtr) && (high < Third_qtr)) {		// Expand middle half
			value -= First_qtr;
			low -= First_qtr;		// Subtract offset to middle
			high -= First_qtr;
		}
		else
			break;					// Otherwise exit loop
		low = 2 * low;
		high = 2 * high + 1;				// Scale up code range
		value = 2 * value + input_bit();	// Move in next input bit
	}
	return symbol;
}


void encode(int *str) {
	start_model();
	fp1 = fopen("output.txt", "w");

	

	start_outputing_bits();
	start_encoding();
	//printf("%d\n", (sizeof(str)/sizeof(int)));
	int i = 0;
	for(i = 0; i < n; i++) {
		// int ch, symbol;
		// ch = str[i];
		printf("%d\n", str[i]);
		// symbol = ch;
		if(str[i] == EOF)
			break;
		(int)str[i];
		encode_symbol(str[i], cum_freq);
		update_model(str[i]);
		// i++;
	}

	/*for(;;) {
		int ch, symbol;
		ch = getc(stdin);
		if(ch == EOF)
			break;
		symbol = char_to_index[ch];
		encode_symbol(symbol, cum_freq);
		update_model(symbol);
	}*/
	encode_symbol(EOF_symbol, cum_freq);
	done_encoding();
	done_outputing_bits();
	fclose(fp1);
}


void decode() {
	FILE *fp3 = fopen("output1.txt", "w");
	fp2 = fopen("output.txt", "r");
	int *str = (int *)malloc(BUF * sizeof(int));
	/*char *str, *temp;
   	str = (char*)(malloc(BUF * sizeof(char)));
   	temp = (char*)(malloc(BUF * sizeof(char)));
   	// FILE *fp = fopen(filename, "r");
   	if(fp != NULL)
      	while(fgets(temp, BUF, fp2) != NULL)
         	strcat(str, temp);*/

	start_model();
	start_inputing_bits();
	start_decoding();
	int i = 0;
	for(;;) {
		// int ch, symbol;
		str[i] = decode_symbol(cum_freq);
		if(str[i] == EOF_symbol)
			break;
		// ch = symbol;
		fprintf(fp3, "%d\n", str[i]);
		update_model(str[i]);
		i++;
	}
	fclose(fp2);
	fclose(fp3);
}

int *readIntFile(char *filename) {
	char *temp;
	int *str;
   str = (int *)(malloc(BUF * sizeof(int)));
   temp = (char *)(malloc(BUF * sizeof(char)));
   FILE *fp = fopen(filename, "r");
   int i = 0;
   if(fp != NULL) {
      while(fgets(temp, BUF, fp) != NULL) {
        	//printf("%s\n", temp);
        	str[i] = atoi(temp);
        	//printf("%d\n", str[i]);
        	i++;
    	}
   }
   n = i;
   fclose(fp);
   return str;
}

char *readFile(char *filename) {
   char *str, *temp;
   str = (char*)(malloc(BUF * sizeof(char)));
   temp = (char*)(malloc(BUF * sizeof(char)));
   FILE *fp = fopen(filename, "r");
   if(fp != NULL)
      	while(fgets(temp, BUF, fp) != NULL) {
        	strcat(str, temp);
    	}
   fclose(fp);
   return str;
}

/* Main program for encoding */

int main(int argc, char **argv) {
	int *str = readIntFile(argv[1]);
	printf("%ld\n", sizeof(int));
	encode(str);
	decode();
	exit(0);
}