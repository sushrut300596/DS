#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
	int item;
	struct Node *next;
} Node;

typedef struct Stack {
	Node *top;
	int len;
} Stack;

void push(Node *x, Stack *s) {
	x->next = s->top;
	s->top = x;
	s->len++;
}

Node *pop(Stack *s) {
	Node *res = NULL;
	res = s->top;
	s->top = res->next;
	res->next = NULL;
	s->len--;
	return res;
}

void display(Stack s) {
	Node *temp = s.top;
	while(temp != NULL) {
		printf("%d ", temp->item);
		temp = temp->next;
	}
	printf("\n");
}

void display1(Stack *s, Node *temp) {
	if(temp != NULL) {
		display1(s,temp->next);
		printf("%d ", temp->item);
		// display1(s,temp->next);
	}
	// printf("\n");
}

int main() {
	Stack *s = malloc(sizeof(struct Stack));
	s->top = NULL;
	s->len = 0;
	Node *node = malloc(sizeof(struct Node)), *node1 = malloc(sizeof(struct Node)), *node2 = malloc(sizeof(struct Node)), *res;
	node->next = NULL;
	node->item = 10;
	node1->next = NULL;
	node1->item = 20;
	node2->next = NULL;
	node2->item = 30;
	push(node,s);
	push(node1,s);
	push(node2,s);
	display1(s,node2);
	// display(*s);
	// res = pop(s);
	// printf("%d\n", res->item);
	// display(*s);
}