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

/*int maxHeight(Node *x) {
	if(x == NULL)
		return -1;
	else {
		int lHeight = maxHeight(x->left);
		int rHeight = maxHeight(x->right);
		if(lHeight > rHeight)
			return (lHeight + 1);
		else
			return (rHeight + 1);
	}
}*/

void insert(Node *x, Tree *t) {
	Node *prev = NULL, *temp = malloc(sizeof(struct Node));
	if(t->root == NULL) {
		t->root = x;
	}
	else {
		temp = t->root;
		while(temp != NULL) {
			if(x->item < temp->item) {
				prev = temp;
				temp = temp->left;
			}
			else {
				prev = temp;
				temp = temp->right;
			}
		}
		if(x->item < prev->item) {
			prev->left = x;
			x->parent = prev;
		}
		else {
			prev->right = x;
			x->parent = prev;
		}
	}
}

void traverse(Node *root) {
	if(root != NULL) {
		traverse(root->left);
		printf("%d ", root->item);
		traverse(root->right);
	}
}

Node *buildTree(int a[], int s, int e, Tree *t) {
	int mid;
	Node *temp = malloc(sizeof(struct Node));
	if(s <= e) {
		mid = (s + e) / 2;
		temp->left = buildTree(a, s, mid - 1, t);
		temp->item = a[mid];
		temp->right = buildTree(a, mid + 1, e, t);
		if(temp->left != NULL)
			temp->left->parent = temp;
		if(temp->right != NULL)
			temp->right->parent = temp;
		return temp;
	}
	else
		return NULL;
	
}

Node *buildTree1(int i, int a[], int n, Tree *t) {
	while(i < n) {
		Node *temp = malloc(sizeof(struct Node));
		temp->item = a[i];
		temp->left = NULL;
		temp->right = NULL;
		insert(temp, t);
		i++;
	}
	return t->root;
}

Node *first(Node *rt) {
	if(rt != NULL) {
		if(rt->left)
			return first(rt->left);
		else
			return rt;
	}
}

Node *last(Node *rt) {
	if(rt != NULL) {
		if(rt->right)
			return last(rt->right);
		else
			return rt;
	}
}

Node *next(Node *x) {
	if(x != NULL)
		if(x->right)
			return first(x->right);
		while((x->parent) && (x->parent->right == x))
			x = x->parent;
		return x->parent;
}

Node *prev(Node *x) {
	if(x != NULL) {
		if(x->left)
			return last(x->left);
		while()
	}
}

int main() {
	Tree *t = malloc(sizeof(struct Tree));
	t->root = NULL;
	int a[5] = {10,22,9,45,34};
	t->root = buildTree(a, 0, (sizeof(a)/sizeof(int) - 1), t);
	// t->root = buildTree1(0, a, 5, t);
	traverse(t->root);
	printf("\n");
	Node *x = first(t->root);
	printf("%d\n", x->item);
	Node *x1 = last(t->root);
	printf("%d\n", x1->item);
	Node *x2 = next(t->root);
	printf("%d\n", x2->item);
	// printf("%d\n", maxHeight(t->root));
}