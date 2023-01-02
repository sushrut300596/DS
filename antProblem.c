// Ant Problem Group Assignment

// Zero Group(mod 4 = 0)

// 21112001 Nikhil Aghav
// 21112024 Sushrut Shendre
// 21112031 Sumit Mahanwar
// 21112043 Ansh Gupta
// 21112044 Arpan Gupta
// 21112046 Adesh Khandait

// Instruction to run the program :
// Root Node or Array's first element is always 'E'

/* Binary Tree of following char array is :

								E
							   / \
							  A   S
							 /   / \
							A   A   A
						   / \ /     \
						  S  S A      S
						 /
						A
*/


#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
	char item;
	struct Node *left;
	struct Node *right;
	struct Node *parent;
} Node;

typedef struct Tree {
	Node *root;
} Tree;

char a[] = {'E', 'A', 'S', 'A', '\0', 'A', 'A', 'S', 'S', '\0', '\0', 'A', '\0', '\0', 'S', '\0', '\0', 'A'};

Node *buildTree(int n) {
	Node *temp = NULL;
	if((a[n] != '\0') && (n < sizeof(a))) {
		temp = malloc(sizeof(struct Node));
		temp->left = buildTree(2*n + 1);
		temp->item = a[n];
		temp->right = buildTree(2*n + 2);
		if(temp->left != NULL) {
			temp->left->parent = temp;
		}
		if(temp->right != NULL) {
			temp->right->parent = temp;
		}
	}
	return temp;
}

int maxHeight(Node *x) {
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
}

/*void preorder(Node *x) {
	if(x != NULL) {
		printf("%c ", x->item);
		preorder(x->left);
		preorder(x->right);
	}
}*/

void survive(Node *x, int *count, int *scount, Tree *t) {
	Node *temp = malloc(sizeof(struct Node));
	if(x != NULL) {
		if(x == t->root) {}
		else if(x->item == 'E') {}
		else if(x->item == 'A') {
			temp = x->parent;
			if(temp == t->root) {
				*count = *count + 1;
			}
			else {
				if(temp->item == 'E')
					temp->item = 'A';
				else if(temp->item == 'A')
					temp->item = 'E';
				else {}
			}
			x->item = 'E';
		}
		else if(x->item == 'S') {
			temp = x->parent;
			if(temp == t->root) {
				*scount = *scount + 1;
			}
			else {
				if(temp->item == 'E')
					temp->item = 'S';
				else if(temp->item == 'A')
					temp->item = 'S';
				else
					temp->item = 'E';
			}
			x->item = 'E';
		}
		survive(x->left, count, scount, t);
		survive(x->right, count, scount, t);
	}
}

int main() {
	Tree *t = malloc(sizeof(struct Tree));
	t->root = NULL;
	int *count = malloc(sizeof(int)), *scount = malloc(sizeof(int));
	int i;
	*count = 0;
	*scount = 0;
	t->root = buildTree(0);
	// preorder(t->root);
	for(i = 0; i <= maxHeight(t->root); i++)
		survive(t->root, count, scount, t);
	printf("Survived Super Ants : %d\nSurvived Normal Ants : %d\n", *scount, *count);
}