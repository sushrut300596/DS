#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
	int item;
	struct Node *left;
	struct Node *right;
} Node;

typedef struct Set {
	Node *root;
} Set;

void insert(int e, Set *s) {
	Node *curr = s->root;
	if(curr == NULL) {
		Node *node = malloc(sizeof(struct Node));
		node->left = NULL;
		node->right = NULL;
		node->item = e;
		s->root = node;
	}
	else {
		Node *prev = NULL;
		while(curr != NULL) {
			if(curr->item == e) {
				printf("Element already present.\n");
				break;
			}
			else if (curr->item > e){
				prev = curr;
				curr = curr->left;
			}
			else {
				prev = curr;
				curr = curr->right;
			}
		}
		if (curr == NULL){
			Node *node = malloc(sizeof(struct Node));
			node->left = NULL;
			node->right = NULL;
			node->item = e;
			if (prev->item > e)
				prev->left = node;
			else
				prev->right = node;
		}
	}
}

void query(int a, int b, Node *curr, int *count) {
	if(curr != NULL) {
		if(curr->item > a && curr->item < b) {
			*count = *count + 1;
			query(a, b, curr->left, count);
			query(a, b, curr->right, count);
		}
		else if(curr->item <= a) {
			if(curr->item == a){
				*count = *count + 1;
			}
			query(a, b, curr->right, count);
		}
		else if(curr->item >= b) {
			if(curr->item == b){
				*count = *count + 1;
			}
			query(a, b, curr->left, count);
		}
	}
}

void traverse (Node *node) {
	if(node != NULL) {
		traverse(node->left);
		printf("%d ", node->item);
		traverse(node->right);
	}
}

void main() {
	Set *s = malloc(sizeof(struct Set));
	s->root = NULL;
	insert(5, s);
	insert(3, s);
	insert(4, s);
	insert(2, s);
	insert(1, s);
	insert(8, s);
	insert(9, s);
	insert(6, s);
	insert(7, s);
	// traverse(s->root);
	int *count = malloc(sizeof(int));
	*count = 0;
	query(4, 8, s->root, count);
	printf("%d\n", *count);
}