#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define BUF 10000000

typedef struct SuffixArray {
   int index;
   char *suff;
} SuffixArray;

SuffixArray *fillSuffixArray(char *str) {
   int i, len = strlen(str);
   SuffixArray *suffarr = malloc(sizeof(SuffixArray) * len);
   for(i = 0; i < len; i++) {
      char *temp = (char *)(malloc((len - i) * sizeof(char)));
      suffarr[i].index = i;
      strncpy(temp, &str[i], len - i);
      suffarr[i].suff = temp;
   }

   /*for(i = 0; i < len; i++) {
      printf("%d ", suffarr[i].index);
   }
   printf("\n");
   for(i = 0; i < len; i++)
      printf("%s ", suffarr[i].suff);
   printf("\n");*/

   return suffarr;
}

void main() {
   char *str = "hello$";
   int i, len = strlen(str);
   SuffixArray *suffarr = malloc(sizeof(SuffixArray) * len);
   suffarr = fillSuffixArray(str);
   for(i = 0; i < len; i++)
      printf("%s ", suffarr[i].suff);
   printf("\n");
}