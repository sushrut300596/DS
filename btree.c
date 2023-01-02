#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
	int item;
	struct Node *left;
	struct Node *right;
	struct Node *parent;
} Node;

typedef struct Tree {
	int height;
	Node *root;
} Tree;

Node *first(Node *x) {
	if(!x)
		return NULL;
	if(x->left)
		return first
}