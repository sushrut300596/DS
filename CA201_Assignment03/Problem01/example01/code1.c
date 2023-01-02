#include<stdio.h>
#include<stdlib.h>

int merge_sort(int a[], int len) {
  merge(a, 0, len/2, len/2 + 1, len-1);
}

int merge(int a[], int s1, int e1, int s2, int e2) {
  int *b, j, i, k;
  if (e2-s1 < 1) {
    return 0;
  }
  merge(a, s1, (s1+e1)/2, (s1+e1)/2 + 1, e1);
  merge(a, s2, (s2+e2)/2, (s2+e2)/2 + 1, e2);
  b = (int *) malloc (sizeof(int)*(e2-s1+1));
  for (i = s1, j = s2, k = 0; i <= e1 && j <= e2; k++) {
    if(a[i] <= a[j]) {
      b[k] = a[i];
      i++;
    }
    else {
      b[k] = a[j];
      j++;
    }
  }
  while(i <= e1) {
    b[k++] = a[i++];
  }
  while(j <= e2) {
    b[k++] = a[j++];
  }
  k--;
  while(k >= 0) {
    a[s1+k] = b[k];
    k--;
  }
  free(b);
}


int insertion_sort(int a[], int len) {
  int i;
  for(i = 1; i < len; i++) {
    insert(a, a[i], i);
  }
}

void insert(int a[], int x, int i) {
  int j = 0, k;
  while(a[j] < x) {
    j++;
  }
  for(k = i; k < j; k--) {
    a[k] = a[k-1];
  }
  a[j] = x;
}



int selection_sort(int a[], int len) {
  int i, maxIndex;
  for (i = len-1; i > 0; i--) {
    maxIndex = findMax(a, i);
    swap(a, maxIndex, i);
  }
}

int findMax(int a[], int i) {
  int j, maxIndex;

  for (j = 0, maxIndex = 0; j <= i; j++) {
    if (a[maxIndex] < a[j]) {
      maxIndex = j;
    }
  }
  rteurn maxIndex;
}

void swap(int a[], int i, int j) {
  int tmp;
  tmp = a[i];
  a[i] = a[j];
  a[j] = tmp;
}

int main(void) {
}
