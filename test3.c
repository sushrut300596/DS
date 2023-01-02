#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF 1000000

// Merges two subarrays of arr[].
// First subarray is arr[l..m]
// Second subarray is arr[m+1..r]
void merge(int arr[], char str[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;
 
    /* create temp arrays */
    int L[n1], R[n2];
 
    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];
 
    /* Merge the temp arrays back into arr[l..r]*/
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = l; // Initial index of merged subarray
    while (i < n1 && j < n2) {
    	/* Compare Based On String */
        if (str[L[i]] < str[R[j]]) {
            arr[k] = L[i];
            i++;
        }
        /* Compare Based On String */
        else if (str[L[i]] > str[R[j]]) {
            arr[k] = R[j];
            j++;
        }
        /* Check Next Character when both Character of 
		String is Same */
        else {
        	int x = L[i], y = R[j], size = strlen(str);
        	char *left = (char *) malloc(size), *right = (char *) malloc(size);
        	memcpy(left, &str[x], size - x - 1);
        	memcpy(right, &str[y], size - y - 1);
        	if (strcmp(left, right) > 0) {
	            arr[k] = L[i];
	            i++;
	        }
	        else {
	            arr[k] = R[j];
	            j++;
	        }
        	/*int count = 0;
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
	        }*/
		}
        k++;
    }
 
    /* Copy the remaining elements of L[], if there
    are any */
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
 
    /* Copy the remaining elements of R[], if there
    are any */
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}
 
/* l is for left index and r is right index of the
sub-array of arr to be sorted */
void mergeSort(int arr[], char str[], int l, int r)
{
    if (l < r) {
        // Same as (l+r)/2, but avoids overflow for
        // large l and h
        int m = l + (r - l) / 2;
 
        // Sort first and second halves
        mergeSort(arr, str, l, m);
        mergeSort(arr, str, m + 1, r);
 
        merge(arr, str, l, m, r);
    }
}
 
/* UTILITY FUNCTIONS */
/* Function to copy string */
void copyString(char str[], int A[], char newStr[], int size)
{
	int i;
	for (i = 0; i < size; i++) {
		if(A[i] != 0)
			newStr[i] = str[A[i]-1];
		else
			newStr[i] = str[size-1];
	}
	newStr[size] = '\0';
}

/* UTILITY FUNCTIONS */
/* Function to Fill Array From 0 to Size-1 */
void arrayFill(int A[], int size)
{
	int i;
	for (i = 0; i < size; i++) {
		A[i] = i;
	}
}

/* UTILITY FUNCTIONS */
/* Function to Print Array */
int getIndex(int A[], int size)
{
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
 
/* Main Function */
int main(int argc, char **argv)
{
	char *str = readFile(argv[1]);
    int arr_size = strlen(str), index;
    strcat(str, "$");
	int arr[arr_size];
	char scpStr[arr_size];
	arrayFill(arr, arr_size);
	printf("Given String :\n%s\n\n", str);
 
    mergeSort(arr, str, 0, arr_size - 1);
    
    copyString(str, arr, scpStr, arr_size);
    index = getIndex(arr, arr_size);
    
    printf("Index :\t%d\n\n", index);
    printf("SCP string :\n%s\n", scpStr);
    return 0;
}