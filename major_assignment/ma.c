/*
	Used mergesort for character sorting in string.

	After sorting gone through each character and have taken just the previous character from original string to construct
	scp string.

	For recovering of string from scp string used count sort in decode function.

	Time complexity is O(n * log n) and in the worst case when string only contains single repetative character
	is O(n^2).

	Time complexity for recovery of string is O(n). 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define BUF 10000000
int L[BUF/2], R[BUF/2];
void merge(int arr[], char str[], int l, int m, int r, int size)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;
 
    // int L[n1], R[n2];

    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];
 
    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2) {
        if (str[L[i]] < str[R[j]]) {
            arr[k] = L[i];
            i++;
        }
        else if (str[L[i]] > str[R[j]]){
            arr[k] = R[j];
            j++;
        }
        else {
        	int x = L[i], y = R[j];
        	int count = 0;
        	while(str[x] == str[y] && count != size) {
        		x++;
        		y++;
        		x = x % size;
        		y = y % size;
        		count++;
			}
			if (str[x] < str[y]) {
	            arr[k] = L[i];
	            i++;
	        }
	        else {
	            arr[k] = R[j];
	            j++;
	        }
		}
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
 
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(int arr[], char str[], int l, int r, int size)
{
    if (l < r) {
        int m = l + (r - l) / 2;

        mergeSort(arr, str, l, m,size);
        mergeSort(arr, str, m + 1, r,size);
 
        merge(arr, str, l, m, r, size);
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

int *arrayFill(int size) {
	int i;
	int *A;
	A = (int*)(malloc(size*sizeof(int)));
	for (i = 0; i < size; i++) {
		A[i] = i;
	}
	return A;
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

void writeFile(char *filename, char *str) {
	FILE *fp = fopen(filename, "w");
	fprintf(fp, "%s\n", str);
	fclose(fp);
}

char *readFile(char *filename) {
	char *str, *temp;
	str = (char*)(malloc(BUF*sizeof(char)));
	temp = (char*)(malloc(BUF*sizeof(char)));
	FILE *fp = fopen(filename, "r");
	if(fp != NULL)
		while(fgets(temp, BUF, fp) != NULL)
			strcat(str, temp);
	fclose(fp);
	return str;
}

char *encode(char *str, int *k) {

	// printf("Original String :\n");
	// printf("%s\n\n", str);
	
	int len = strlen(str);
	int *arr;
	arr = (int*)(malloc(len*sizeof(int)));
	char *scpStr;
	scpStr = (char*)(malloc(len*sizeof(char)));
	arr = arrayFill(len);
	
	mergeSort(arr, str, 0, (len - 1), len);
	
	*k = getIndex(arr, len);
    
    scpStr = copyString(str, arr, len);
	return scpStr;
}

char *decode(char *scpStr, int k) {
	int i, len = strlen(scpStr);
	int *K = (int *)malloc(256 * sizeof(int));
	int *C = (int *)malloc(len * sizeof(int));
	int *M = (int *)malloc(256 * sizeof(int));
	char *str = (char *)(malloc(len*sizeof(char)));
	for(i = 0; i < 256; i++) {
		K[i] = 0;
	}

	for(i = 0; i < len; i++) {
		C[i] = K[(int)scpStr[i]];
		K[(int)scpStr[i]] = K[(int)scpStr[i]] + 1;
	}

	int sum = 0;
	for(i = 0; i < 256; i++) {
		M[i] = sum;
		sum = sum + K[i];
	}

	int j = k;
	for(i = (len - 1); i >= 0; i--) {
		str[i] = scpStr[j];
		j = C[j] + M[(int)scpStr[j]];
	}
	str[len] = '\0';
	return str;
}

int main(int argc, char **argv) {
	time_t begin = time(NULL);

	char *str = readFile(argv[1]);
	int *k = (int *)(malloc(sizeof(int)));
	char *scpStr = encode(str, k);
	
	

    printf("Index :\t%d\n\n", *k);
    

	// printf("Last Column : \n");
	// printf("%s\n\n", scpStr);


	char *str1;
	str1 = decode(scpStr, *k);
	writeFile(argv[2], str1);

	// printf("Recovered String : \n");
	// printf("%s\n\n", str1);

	printf("Matched with original string if value is Zero(0) : %d\n", strcmp(str, str1));

	time_t end = time(NULL);
	printf("Time taken in seconds : %ld\n", (end-begin));
	
	// sleep(3);


}