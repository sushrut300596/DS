#include <stdio.h>
#include <string.h>
#include <stdlib.h>


static int compare(const void *a, const void *b)
{
    const char **str_a = (const char **)a;
    const char **str_b = (const char **)b;
    return strcmp(*str_a, *str_b);
}
  
void sort(char *arr[], int n)
{
    qsort(arr, n, sizeof(char*), compare);
}

char **cyclicPermutation(char *str) {
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
        strcpy(mat[i], str);
        //mat[i][len] = '\0';
    }
    return mat;
}

int main() {
    char str[] = "hello";
    int len = strlen(str);
    int i;
    char **str1;
    str1 = cyclicPermutation(str);
    for(i = 0; i < len; i++) {
        printf("%s\n", str1[i]);
    }
    sort(str1, len);
    for (i = 0; i < len; i++)
        printf("%s\n", str1[i]);
}