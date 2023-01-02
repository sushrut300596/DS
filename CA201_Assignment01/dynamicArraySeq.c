#include<stdio.h>
#include<stdlib.h>


/*
 * ************* Maximum Points: 10 ***************** 
 *
 * ********** PENALTY: -15 (negative 15) ************** 
 *  In following cases you will get penalty
 *  (a) Not submitted, 
 *  (b) Copied, 
 *  (c) Late submission
 *  (d) Submission is not as per the instructions
 *
 * ***************** Instructions *******************
 * (1) Implement ALL methods without any changes in input arguments. You are
 *     NOT allowed to add or remove any arguments of method.
 * (2) DO NOT CHANGE any names of structure, functions or arguments etc.
 * (3) You can add helper methods. Add comments to describe its purpose.
 * (4) You can make assumptions if something is not clear. Make sure you write 
 *     your assumptions wherever required as comments. If you make assumptions but 
 *     HAVE NOT mentioned then it may lead to loss of points.
 * (5) While submitting the code DO NOT KEEP any code in main method. Also DO 
 *     NOT CHANGE filename while submitting the code.
*/


/* README
   This implementation of integer sequence uses dynamic array.
   Here we have empty space before and after the sequence.
   We dont need to initiate a new array every time. We can use empty
   slots for new insertions. 
*/

typedef struct Item {
  int k;
}Item;

typedef struct DynamicArraySeq {
  Item *items; // Array of items
  int s;       // Start Index of sequence
  int e;       // End Index of sequence
  int size;    // Total size of Array
} DynamicArraySeq;

////////////////////////////////////////////////////


/// You can add helper methods here

// Helper method to print array
void print(struct DynamicArraySeq arr)
{
  int i;
  for (i = arr.s; i <= arr.e; i++)
  {
    printf("%d ", arr.items[i].k);
  }
  printf("\n");
}

void growSize(DynamicArraySeq *s) {
  struct DynamicArraySeq *temp = malloc(sizeof(struct DynamicArraySeq));
  temp->size = s->size * 2;
  temp->items = malloc(temp->size * sizeof(struct Item));
  temp->s = (int)((temp->size - s->size)/2);
  temp->e = temp->s + s->size - 1;
  int i, j;
  for(i = temp->s, j = s->s; i <= temp->e, j <= s->e; i++, j++)
  {
    temp->items[i].k = s->items[j].k;
  }
  s->size = temp->size;
  s = temp;
}

////////////////////////////////////////////////////


// Read 'len' number of items and create DynamicArraySeq from it.
// Return address of the sequence.

DynamicArraySeq *build(int len) {
  //-----------------------------
  //        Implement me
  //-----------------------------
  struct DynamicArraySeq *arr = malloc(sizeof(struct DynamicArraySeq));
  arr->size = 4;
  arr->items = malloc(arr->size * sizeof(struct Item));
  arr->s = (int)((arr->size - len)/2);
  arr->e = arr->s + len - 1;
  int i;
  for(i = arr->s; i <= arr->e; i++)
  {
    scanf("%d", &arr->items[i].k);
  }
  return arr;
}

// Replace 'i'th item of the sequence with new item 'x'.
void set_at(int i, Item x, DynamicArraySeq *s) {
  //-----------------------------
  //        Implement me
  //-----------------------------
  s->items[s->s + i] = x;
}

// Return i th item
Item get_at(int i, DynamicArraySeq s) {
  //-----------------------------
  //        Implement me
  //-----------------------------
  return s.items[s.s + i];
}

// Return Length of sequence 
int length(DynamicArraySeq s) {
  //-----------------------------
  //        Implement me
  //-----------------------------
  return ((s.e - s.s) + 1);
}

// insert 'x' at i th position in sequence s (REMEBER ith ITEM OF SEQUENCE NOT ARRAY).
void insert_at(int i, Item x, DynamicArraySeq *s) {
  //-----------------------------
  //        Implement me
  //-----------------------------
  // Assumption
  // If there is no blank space to enter a number in array then showing an error.
  if(s->s == 0 && s->size == (s->e + 1))
  {
    growSize(s);
  }
  else
  {
    int j;
    if((s->size - 1) != s->e)
    {
      for(j = s->e; j >= (s->s + i); j--)
      {
        s->items[j + 1] = s->items[j];
        if(j == (s->s + i))
        {
          s->items[j] = x;
        }
      }
      s->e = s->e + 1;
    }
    else
    {
      for(j = s->s; j <= (s->s + i - 1); j++)
      {
        s->items[j - 1] = s->items[j];
        if(j == (s->s + i - 1))
        {
          s->items[j] = x;
        }
      }
      s->s = s->s - 1;
    }
  }
}

// removes the ith element of the sequence and returns deleted item.
// if empty part of array is bigger in size compare to sequence length then
// move sequence to smaller size array.
Item delete_at(int i, DynamicArraySeq *s) {
  //-----------------------------
  //        Implement me
  //-----------------------------
  struct Item item;
  item = s->items[s->s + i];
  int j;
  for(j = s->s + i; j < s->e; j++)
  {
    s->items[j] = s->items[j + 1];
  }
  s->e = s->e - 1;
  return item;
}

// insert item 'x' at the start of the sequence.
void insert_first(Item x, DynamicArraySeq *s) {
  //-----------------------------
  //        Implement me
  //-----------------------------
  printf("%d %d\n", s->s, s->e);
  if(s->size == (s->e - s->s + 1)) {
    growSize(s);
  }
  s->s = s->s - 1;
  s->items[s->s] = x;
}
// insert item 'x' at the end of the sequence.
void insert_last(Item x, DynamicArraySeq *s) {
  //-----------------------------
  //        Implement me
  //-----------------------------
  if(s->size == (s->e - s->s + 1))
  {
    growSize(s);
  }
  s->e = s->e + 1;
  s->items[s->e] = x;
}

// delete first item of sequence and returns deleted item.
Item delete_first(DynamicArraySeq *s) {
  //-----------------------------
  //        Implement me
  //-----------------------------
  struct Item item;
  item = s->items[s->s];
  s->s = s->s + 1;
  return item;
}

// delete last item of sequence and returns deleted item.
Item delete_last(DynamicArraySeq *s) {
  //-----------------------------
  //        Implement me
  //-----------------------------
  struct Item item;
  item = s->items[s->e];
  s->e = s->e - 1;
  return item;
}

int main(void) {
  struct DynamicArraySeq *s = malloc(sizeof(struct DynamicArraySeq));
  struct Item *x = malloc(sizeof(struct Item));
  struct Item *x1 = malloc(sizeof(struct Item));
  x->k = 12;
  x1->k = 20;
  s = build(2);
  insert_first(*x, s);
  insert_last(*x, s);
  insert_last(*x1, s); 
  insert_last(*x1, s);
  x->k = 13;
  insert_first(*x, s);
  x->k = 14;
  insert_first(*x, s);
  insert_first(*x, s);
  insert_last(*x1, s);
  print(*s);
  printf("%d\n", s->size);
}
