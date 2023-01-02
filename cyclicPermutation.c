#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static int compare(const void *a, const void *b)
{
    return strcmp(*(char**)a, *(char**)b);
}
  
void sort(char **arr, int n)
{
    qsort(arr, n, sizeof(char*), compare);
}

char *cyclicPermutation(char *str) {
	char temp;
	int i, j, len = strlen(str);
	char **mat = malloc(len*len);
	for(i = 0; i < len; i++) {
		temp = str[len - 1];
		for(j = len - 1; j > 0; j--) {
			str[j] = str[j - 1];
		}
		str[0] = temp;
		mat[i] = malloc(len);
		for(j = 0; j < len; j++) {
			mat[i][j] = str[j];
		}
		mat[i][len] = '\0';
	}
	sort(mat, len);
	char *scpStr = malloc(len);
	for(i = 0; i < len; i++){
		scpStr[i] = mat[i][len-1];
	}
	return scpStr;
}

char **scpSequence(char *scpStr) {
	int len = strlen(scpStr);
	char **mat = malloc(len*len);
	int i, j;
	for(i = 0; i < len; i++) {
		mat[i] = malloc(len);
		for (j = 0; j < len; j++) {
			mat[i][j] = 'a';
		}
	}
	for (j = len-1; j >= 0; j--) {
		for (i = 0; i < len; i++) {
			mat[i][j] = scpStr[i];
		}
		sort(mat, len);
	}
	return mat;
}

int main() {
	char str[] = "vanana";
	int len = strlen(str);
	int i;
	char *str1;
	str1 = cyclicPermutation(str);
	printf("%s\n", str1);
	printf("\n");
	char **sequence = scpSequence(str1);
	for(i = 0; i < len; i++) {
		printf("%s\n", sequence[i]);
	}
}