#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define BUF 1000000

typedef struct SuffixArray {
   int index;
   char *suff;
} SuffixArray;

int maxRepeatChar(SuffixArray *suffarr, int n) {
   int repeat = 1, i;
   for(i = 0; i < n; i++){
      if (strlen(suffarr[i].suff) > repeat) 
         repeat = strlen(suffarr[i].suff);
   }
   return repeat;
}

void radix_sort (SuffixArray *suffarr, int n){
   SuffixArray bucket[257][257];
   int bucket_cnt[257];
   int i, j, k, r, NOP = 0, divisor = 1, pass, len;

   NOP = maxRepeatChar(suffarr, n);
   
   for (pass = 0; pass < NOP; pass++){
      for (i = 0; i < 256; i++){
         bucket_cnt[i] = 0;
      }
      for (i = 0; i < n; i++){
         len = strlen(suffarr[i].suff);
         if (pass < len)
            r = (int)suffarr[i].suff[len - pass - 1];
         else
            r = (int)suffarr[i].suff[0];
         //bucket[r][bucket_cnt[r]] = suffarr[i].suff;
         //bucket[r][bucket_cnt[r]] = (char *)malloc(n);
         //strcpy(bucket[r][bucket_cnt[r]], suffarr[i].suff);
         //printf("Hello\n");
         bucket[r][bucket_cnt[r]].index = suffarr[i].index;
         bucket[r][bucket_cnt[r]].suff = (char *)malloc(len);
         strcpy(bucket[r][bucket_cnt[r]].suff, suffarr[i].suff);
         bucket_cnt[r] += 1;
      }
      i = 0;
      for (k = 0; k < 256; k++){
         for (j = 0; j < bucket_cnt[k]; j++){
            strcpy(suffarr[i].suff, bucket[k][j].suff);
            suffarr[i].index = bucket[k][j].index;
            i++;
         }
      }
   }
}

SuffixArray *fillSuffixArray(char *str) {
   int i, len = strlen(str);
   SuffixArray *suffarr = malloc(sizeof(SuffixArray) * len);
   for(i = 0; i < len; i++) {
      suffarr[i].suff = (char *)(malloc((len - i) * sizeof(char)));
      suffarr[i].index = i;
      strcpy(suffarr[i].suff, &str[i]);
      //suffarr[i].suff = temp;
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

char *copyString(SuffixArray *suffarr, char *str, int size) {
   int i;
   char *newStr;
   newStr = (char *)(malloc(size * sizeof(char)));
   for (i = 0; i < size; i++) {
      if(suffarr[i].index != 0)
         newStr[i] = str[suffarr[i].index-1];
      else
         newStr[i] = str[size-1];
   }
   newStr[size] = '\0';
   return newStr;
}

char *readFile(char *filename) {
   char *str, *temp;
   str = (char*)(malloc(BUF * sizeof(char)));
   temp = (char*)(malloc(BUF * sizeof(char)));
   FILE *fp = fopen(filename, "r");
   if(fp != NULL)
      while(fgets(temp, BUF, fp) != NULL)
         strcat(str, temp);
   fclose(fp);
   return str;
}

int main (int argc, char **argv){
   time_t begin = time(NULL);
   /*printf ("Enter the number of items to be sorted: ");
   scanf ("%d", &n);*/

   char *str = readFile(argv[1]);
   strcat(str, "$");
   //char *str = "hello$";


   //int *k = (int *)(malloc(sizeof(int)));
   //char *scpStr = encode(str, k);

   //char *str = "banana$";
   int i, n = strlen(str);
   // printf("Hello\n");
   SuffixArray *suffarr;
   suffarr = malloc(sizeof(struct SuffixArray) * BUF);

   suffarr = fillSuffixArray(str);
   // printf("Hello\n");
   for(i = 0; i < n; i++)
      printf("%s ", suffarr[i].suff);
   printf("\n");

   radix_sort (suffarr, n);
   printf ("Sorted items : \n");
   for(i = 0; i < n; i++)
      printf("%s ", suffarr[i].suff);
   printf("\n");
   for(i = 0; i < n; i++) {
      printf("%d ", suffarr[i].index);
   }
   printf("\n");

   char *scpString = copyString(suffarr, str, n);
   printf("\nSCP String :\t%s\n", scpString);

   time_t end = time(NULL);
   printf("Time taken in seconds : %ld\n", (end-begin));
}