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
 *      NOT allowed to add or remove any arguments of any method.
 * (2) DO NOT CHANGE names of structures, functions or arguments.
 * (3) You can add helper method. Add comments to describe its purpose.
 * (4) You can make assumptions if something is not clear. Make sure you write 
 *     your assumptions wherever required as comments. If you make assumptions but 
 *     HAVE NOT mentioned then it may lead to loss of points.
 * (5) While submitting the code DO NOT KEEP any code in main method. Also DO 
 *     NOT CHANGE filename while submitting the code.
*/

/* README
   This implementation of integer sequence uses linked list.
   Every node has pointer to next node. 
   items are referred as 0,1,2...n-1. (meaning, index start from 0)
*/

typedef struct Node {
  int item;           // item
  struct Node *next;  // Pointer to next node
}Node;

typedef struct LinkedList {
  Node *head;   // Pointer to first node of list
  int len;      // Length of linked list
}LinkedList;


////////////////////////////////////////////////////


/// You can add helper methods here

void print(struct LinkedList ls)
{
  struct Node *node = ls.head;
  while(node != NULL) {
    printf("%d ", node->item);
    node = node->next;
  }
  printf("\n");
}


////////////////////////////////////////////////////



// Read 'len' number of integers and create LinkedList from it.
// Return address of the sequence.

LinkedList *build(int len) {
  //-----------------------------
  //        Implement me
  //-----------------------------
  struct Node *node = NULL, *prev = NULL;
  struct LinkedList *ls = malloc(sizeof(struct LinkedList));
  ls->len = len;
  int i;
  for(i = 0; i < ls->len; i++) {
    node = malloc(sizeof(struct Node));
    scanf("%d", &node->item);
    node->next = NULL;
    if(i == 0)
      ls->head = node;
    else
      prev->next = node;
    prev = node;
  }
  return ls;
}

// Replace 'i'th node of the sequence with new item 'x'.
void set_at(int i, Node *x, LinkedList *s) {
  //-----------------------------
  //        Implement me
  //-----------------------------
  struct Node *node = NULL, *prev = NULL;
  if(i == 0) {
    node = s->head;
    x->next = node->next;
    s->head = x;
  }
  else {
    prev = s->head;
    int j;
    for(j = 0; j < i - 1; j++) {
      prev = prev->next;
    }
    node = prev->next;
    prev->next = x;
    x->next = node->next;
  }
}

// Return i th item
Node *get_at(int i, LinkedList s) {
  //-----------------------------
  //        Implement me
  //-----------------------------
  struct Node *node = NULL;
  node = s.head;
  int j;
  for(j = 0; j < i; j++) {
    node = node->next;
  }
  return node;
}

// Return Length of sequence 
int length(LinkedList s) {
  //-----------------------------
  //        Implement me
  //-----------------------------
  return s.len;
}

// insert 'x' at i th position in sequence s (REMEBER ith ITEM OF SEQUENCE NOT ARRAY).
void insert_at(int i, Node *x, LinkedList *s) {
  //-----------------------------
  //        Implement me
  //-----------------------------
  struct Node *node = NULL, *prev = NULL;
  s->len = s->len + 1;
  if(i == 0) {
    x->next = s->head;
    s->head = x;
  }
  else {
    prev = get_at((i - 1),*s);
    node = prev->next;
    prev->next = x;
    x->next = node;
  }
}

// removes the ith element of the sequence and returns the pointer to deleted item.
Node *delete_at(int i, LinkedList *s) {
  //-----------------------------
  //        Implement me
  //-----------------------------
  struct Node *node = NULL, *prev = NULL;
  if(i == 0) {
    node = s->head;
    s->head = node->next;
  }
  else {
    prev = get_at((i - 1),*s);
    node = prev->next;
    prev->next = node->next;
  }
  node->next = NULL;
  s->len = s->len - 1;
  return node;
}

// insert item 'x' at the start of the sequence.
void insert_first(Node *x, LinkedList *s) {
  //-----------------------------
  //        Implement me
  //-----------------------------
  insert_at(0,x,s);
}
// insert item 'x' at the end of the sequence.
void insert_last(Node *x, LinkedList *s) {
  //-----------------------------
  //        Implement me
  //-----------------------------
  insert_at(s->len,x,s);
}

// delete first item of sequence and return pointer to deleted item.
Node *delete_first(LinkedList *s) {
  //-----------------------------
  //        Implement me
  //-----------------------------
  return (delete_at(0,s));
}

// delete last item of sequence and return pointer to deleted item.
Node *delete_last(LinkedList *s) {
  //-----------------------------
  //        Implement me
  //-----------------------------
  return (delete_at(s->len - 1,s));
}

int main(void) {
}
