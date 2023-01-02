#include<stdio.h>
#include <stdlib.h>

/*
 * ************* Maximum Points: 05 ***************** 
 *
 * ********** PENALTY: -7 (negative 7) ************** 
 *  In following cases you will get penalty
 *  (a) Not submitted, 
 *  (b) Copied, 
 *  (c) Late submission
 *  (d) Submission is not as per the instructions
 *
 * ***************** Instructions *******************
 * (1) Implement ALL methods without any changes in input arguments. You are
 *     NOT allowed to add or remove or change any arguments of method.
 * (2) DO NOT CHANGE any names of structure, functions or arguments.
 * (3) You can add helper methods / new methods. Add comments to describe its purpose.
 * (4) You can make assumptions if something is not clear. Make sure you write 
 *     your assumptions wherever required as comments. If you make assumptions but 
       HAVE NOT mentioned then it may lead to loss of points.
 * (5) While submitting the code DO NOT KEEP any code in main method. Also DO 
       NOT CHANGE filename while submitting the code.
 */


/* README
   This implementation of integer sequence uses static array. 
   Meaning array is exactly of size len. No buffer space.
   Every remove or insert will require you to initiate and 
   copy into NEW array of size len-1 or len+1 respectively.
*/

typedef struct ArraySeq {
  int *items;  // Array of length len
  int len;     // length of sequence
} ArraySeq;

////////////////////////////////////////////////////


/// You can add helper methods here

// Helper method to print array
void print(struct ArraySeq arr)
{
  int i;
  for (i = 0; i < arr.len; i++)
  {
    printf("%d ", arr.items[i]);
  }
  printf("\n");
}


////////////////////////////////////////////////////


// Read 'len' number of integers and create ArraySeq from it.
// Return pointer to ArraySeq.

ArraySeq *build(int len) {
  //-----------------------------
  //        Implement me
  //-----------------------------
  struct ArraySeq *arr = malloc(sizeof(struct ArraySeq));
  int i;
  arr->len = len;
  arr->items = malloc(len);
  for(i = 0; i < arr->len; i++)
  {
    scanf("%d", &arr->items[i]);
  }
  return arr;
}

// Replaces i th item in sequence with item 'x' 
void set_at(int i, int x, ArraySeq *s) {
  //-----------------------------
  //        Implement me
  //-----------------------------
  s->items[i] = x;
}

int get_at(int i, ArraySeq s) {
  //-----------------------------
  //        Implement me
  //-----------------------------
  return s.items[i];  
}

int length(ArraySeq s) {
  //-----------------------------
  //        Implement me
  //-----------------------------
  return s.len;      
}

// insert 'x' at i th position in sequence s.

void insert_at(int i, int x, ArraySeq *s) {
  //-----------------------------
  //        Implement me
  //----------------------------- 
  s->len = s->len + 1;
  int arr[s->len], j, flag = 0;
  for(j = 0; j < s->len; j++)
  {
    if(j == i)
    {
      arr[j] = x;
      j++;
      flag = 1;
    }
    if(flag == 0)
    {
      arr[j] = s->items[j];
    }
    else
    {
      arr[j] = s->items[j - 1];
    }
  } 
  s->items[s->len];
  for(j = 0; j < s->len; j++)
    s->items[j] = arr[j];
}

// removes the ith element and returns the deleted integer.
// Make sure array is copied into another array of len-1 size.
int delete_at(int i, ArraySeq *s) {
  //-----------------------------
  //        Implement me
  //----------------------------- 
  s->len = s->len - 1;
  int temp = s->items[i];
  int arr[s->len], j, flag = 0;
  for(j = 0; j < s->len; j++)
  {
    if(j == i)
    {
      arr[j] = s->items[j + 1];
      flag = 1;
    }
    if(flag == 0)
    {
      arr[j] = s->items[j];
    }
    else
    {
      arr[j] = s->items[j + 1];
    }
  }
  s->items[s->len];
  for(j = 0; j < s->len; j++)
    s->items[j] = arr[j];
  return temp;
}

int main(void) {
}
