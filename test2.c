#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define BUF 2000000

int leq1(int a1, int a2, int b1, int b2) {
	if(a1 < b1 || a1 == b1 && a2 <= b2)
		return 1;
	else
		return 0;
}
int leq2(int a1, int a2, int a3, int b1, int b2, int b3) {
	if(a1 < b1 || a1 == b1 && leq1(a2,a3, b2,b3))
		return 1;
	else
		return 0;
}

int GetI(int n0, int t, int *SA12) {
	return(SA12[t] < n0 ? SA12[t] * 3 + 1 : (SA12[t] - n0) * 3 + 2);
}

// stably sort a[0..n-1] to b[0..n-1] with keys in 0..K from r
void radixPass(int *a, int *b, int *r, int n, int K) {
	int *c = (int *)malloc(sizeof(int) * (K + 1));
	for (int i = 0; i <= K; i++)
		c[i] = 0;
	for (int i = 0; i < n; i++)
		c[r[a[i]]]++;
	for (int i = 0, sum = 0; i <= K; i++) {
		int t = c[i];
		c[i] = sum;
		sum += t;
	} // exclusive prefix sums
	for (int i = 0; i < n; i++)
		b[c[r[a[i]]]++] = a[i]; // sort
}

// find the suffix array SA of T[0..n-1] in {1..K}^n
// requires T[n]=T[n+1]=T[n+2]=0, n>=2
void suffixArray(int *T, int *SA, int n, int K) {
	
	int n0 = (n + 2) / 3, n1 = (n + 1) / 3, n2 = n / 3, n02 = n0 + n2;
	int *R = (int *)malloc(sizeof(int) * (n02 + 3));
	R[n02] = R[n02 + 1] = R[n02 + 2] = 0;
	int *SA12 = (int *)malloc(sizeof(int) * (n02 + 3));
	SA12[n02] = SA12[n02 + 1] = SA12[n02 + 2] = 0;
	int *R0 = (int *)malloc(sizeof(int) * n0);
	int *SA0 = (int *)malloc(sizeof(int) * n0);

	// Step 1: Construct sample
	// generate positions of mod 1 and mod 2 suffixes
	// the "+(n0-n1)" adds a dummy mod 1 suffix if n%3 == 1
	for(int i = 0, j = 0; i < n + (n0 - n1); i++)
		if(i % 3 != 0)
			R[j++] = i;

	// Step 2: Sort sample suffixes
	// LSB radix sort the mod 1 and mod 2 triples
	radixPass(R , SA12, T+2, n02, K);
	radixPass(SA12, R , T+1, n02, K);
	radixPass(R , SA12, T , n02, K);


	// find lexicographic names of triples and
	// write them to correct places in R
	int name = 0, c0 = -1, c1 = -1, c2 = -1;
	for(int i = 0; i < n02; i++) {
		if (T[SA12[i]] != c0 || T[SA12[i]+1] != c1 || T[SA12[i]+2] != c2) {
			name++;
			c0 = T[SA12[i]];
			c1 = T[SA12[i]+1];
			c2 = T[SA12[i]+2];
		}
		if(SA12[i] % 3 == 1) {
			R[SA12[i]/3] = name;
		} // write to R1
		else {
			R[SA12[i]/3 + n0] = name;
		} // write to R2
	}
	
	// recurse if names are not yet unique
	if (name < n02) {
		suffixArray(R, SA12, n02, name);
		// store unique names in R using the suffix array
		for (int i = 0; i < n02; i++)
			R[SA12[i]] = i + 1;
	}
	else // generate the suffix array of R directly
		for (int i = 0; i < n02; i++) 
			SA12[R[i] - 1] = i;
	

	// Step 2: Sort nonsample suffixes
	// stably sort the mod 0 suffixes from SA12 by their first character
	for (int i = 0, j = 0; i < n02; i++)
		if (SA12[i] < n0)
			R0[j++] = 3*SA12[i];
	radixPass(R0, SA0, T, n0, K);
	

	// Step 3: Merge
	// merge sorted SA0 suffixes and sorted SA12 suffixes
	for (int p = 0, t = n0 - n1, k = 0; k < n; k++) {
		
		int i = GetI(n0, t, SA12); // pos of current offset 12 suffix
		int j = SA0[p]; // pos of current offset 0 suffix
		if ((SA12[t] < n0 ? leq1(T[i], R[SA12[t] + n0], T[j], R[j/3]) : leq2(T[i],T[i+1],R[SA12[t]-n0+1], T[j],T[j+1],R[j/3+n0])) == 1)
		{
			// suffix from SA12 is smaller
			SA[k] = i; t++;
			if (t == n02) // done --- only SA0 suffixes left
				for (k++; p < n0; p++, k++)
					SA[k] = SA0[p];
		}
		else { // suffix from SA0 is smaller
			SA[k] = j;
			p++;
			if (p == n0) // done --- only SA12 suffixes left
				for (k++; t < n02; t++, k++)
					SA[k] = GetI(n0, t, SA12);
		}
	}
}

char *copyString(char str[], int A[], int size) {
	int i;
	char *newStr;
	newStr = (char*)(malloc(size*sizeof(char)));
	for (i = 0; i < size; i++) {
		if(A[i] != 0)
			newStr[i] = str[A[i]-1];
		else
			newStr[i] = str[size-1];
	}
	newStr[size] = '\0';
	return newStr;
}

int getIndex(int A[], int size) {
	int i;
	for (i = 0; i < size; i++) {
		if (A[i] == 0) {
			break;
		}
	}
	return i;
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

char *encode(char *str, int *k, int n) {
	
	int *T = (int *)malloc(sizeof(int) * n);
	int *SA = (int *)malloc(sizeof(int) * n);
	for(int i = 0; i < n; i++)
		T[i] = (int)str[i];
	
	suffixArray(T, SA, n, 255);
	
	*k = getIndex(SA, n);
    
   char *scpStr = copyString(str, SA, n);
	return scpStr;
}

char *decode(char *scpStr, int k, int n) {
	int i;
	int K[256];
	int C[n];
	int M[256];
	char *str = (char *)(malloc(n*sizeof(char)));
	for(i = 0; i < 256; i++) {
		K[i] = 0;
	}

	for(i = 0; i < n; i++) {
		C[i] = K[(int)scpStr[i]];
		K[(int)scpStr[i]] = K[(int)scpStr[i]] + 1;
	}

	int sum = 0;
	for(i = 0; i < 256; i++) {
		M[i] = sum;
		sum = sum + K[i];
	}

	int j = k;
	for(i = (n - 1); i >= 0; i--) {
		str[i] = scpStr[j];
		j = C[j] + M[(int)scpStr[j]];
	}
	str[n] = '\0';
	return str;
}

int main(int argc, char **argv) {
	
	
	time_t begin = time(NULL);

	// char *str = "bananabananabananabanana$";

	char *str = readFile(argv[1]);
   strcat(str, "$");
   int n = strlen(str);

   // printf("%d\n", strlen(str));

   int *k = (int *)(malloc(sizeof(int)));
	char *scpStr = encode(str, k, n);

	// printf("%d\n", strlen(scpStr));

	char *str1;
	str1 = decode(scpStr, *k, n);

	// printf("%d\n", strlen(str1));

	printf("%d\n", strcmp(str, str1));
	
	time_t end = time(NULL);

	printf("%ld\n", end-begin);

}