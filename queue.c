#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
	int item;
	struct Node *next;
} Node;

typedef struct Queue {
	Node *front;
	Node *back;
	int len;
	int size;
} Queue;

void push(Node *x, Queue *q) {
	if(q->len == q->size) {
		printf("Size is full\nElements are not pushed in queue\n");
	}
	else {
		if(q->front == NULL && q->back == NULL) {
			q->front = x;
			q->back = x;
		}
		else {
			q->back->next = x;
			q->back = x;
		}
		q->len++;
	}
}

Node *pop(Queue *q) {
	Node *res = NULL;
	res = q->front;
	q->front = res->next;
	res->next = NULL;
	q->len--;
	return res;
}

Queue *build(int i, int a[], int n, Queue *q) {
	Node *node = malloc(sizeof(struct Node));
	if(i < n) {
		node->item = a[i];
		push(node,q);
		build(i + 1, a, n, q);
	}
}

void display(Queue q) {
	Node *temp;
	temp = q.front;
	if(q.front == NULL && q.back == NULL)
		printf("Queue is empty\n");
	else {
		while(temp != NULL) {
			printf("%d ", temp->item);
			temp = temp->next;
		}
		printf("\n");
	}
}

int main() {
	Queue *q = malloc(sizeof(struct Queue));
	q->front = NULL;
	q->back = NULL;
	q->len = 0;
	q->size = 4;
	int a[5] = {1,2,3,4,5};
	build(0, a, 5, q);
	/*Node *node = malloc(sizeof(struct Node)), *node1 = malloc(sizeof(struct Node)), *res;
	node->next = NULL;
	node->item = 50;
	node1->next = NULL;
	node1->item = 30;
	push(node,q);
	push(node1,q);*/
	display(*q);
	/*res = pop(q);
	printf("%d\n", res->item);
	display(*q);*/
}