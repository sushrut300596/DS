#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
	int item;
	struct Node *next;
} Node;

typedef struct LinkedList {
	Node *head;
} LinkedList;