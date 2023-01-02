#include <stdio.h>
#include <stdlib.h>

#define CODE_VALUE 16
#define MAX_VALUE ((1 << CODE_VALUE) - 1)
#define MAX_FREQ 16383

#define FIRST_QTR (MAX_VALUE / 4 + 1)
#define HALF (2 * FIRST_QTR)
#define THIRD_QTR (3 * FIRST_QTR)

#define NO_OF_CHARS 256
#define EOF_SYMBOL (NO_OF_CHARS + 1)
#define NO_OF_SYMBOLS (NO_OF_CHARS + 1)


unsigned char index_to_char [NO_OF_SYMBOLS];
int char_to_index [NO_OF_CHARS];
int cum_freq [NO_OF_SYMBOLS + 1];
int freq [NO_OF_SYMBOLS + 1];


