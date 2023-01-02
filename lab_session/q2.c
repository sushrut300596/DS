#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
	char item;
	struct Node *left;
	struct Node *right;
	struct Node *parent;
} Node;

typedef struct Tree {
	Node *root;
} Tree;

Node *build(char *arr, int start, int end) {
	Node *temp = NULL;
	if(start <= end) {
		int mid = (start + end) / 2;
		temp = malloc(sizeof(struct Node));
		temp->item = arr[start];
		temp->left = build(arr, start + 1, mid);
		temp->right = build(arr, mid + 1, end);
		if(temp->left != NULL)
			temp->left->parent = temp;
		if(temp->right != NULL)
			temp->right->parent = temp;
	}
	return temp;
}

void traverse(Node *node) {
	if(node != NULL) {
		printf("%c", node->item);
		traverse(node->left);
		traverse(node->right);
	}
}

Node *first(Tree *t) {
	return t->root;
}

Node *last(Node *temp) {
	while(temp->left != NULL || temp->right != NULL) {
		if(temp->right != NULL)
			temp = temp->right;
		else
			temp = temp->left;
	}
	return temp;
}

Node *next(Node *node) {
	Node *parent = node->parent;
	if(node->left != NULL) {
		return node->left;
	}
	else if(node->right){
		return node->right;
	}
	else {
		while(parent->right == node) {
			node = parent;
			parent = node->parent;
		}
		return parent->right;
	}
}

Node *previous(Node *node) {
	Node *parent = node->parent;
	if(parent != NULL) {
		if(parent->left == node){
			return parent;
		}
		else {
			if(parent->left != NULL)
				return last(parent->left);
			else
				return parent;
		}
	}
}

int main() {
	Tree *t = malloc(sizeof(struct Tree));
	Node *n = malloc(sizeof(struct Node));
	t->root = NULL;
	char *a = "STRING";
	t->root = build(a, 0, (strlen(a) - 1));
	traverse(t->root);
	printf("\n");
	// n = first(t->root);
	// n = last(t);
	// n = next(t->root);
	n = previous(t->root->left);
	printf("%c", n->item);
	printf("\n");
}