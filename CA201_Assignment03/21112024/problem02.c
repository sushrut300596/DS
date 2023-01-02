// Name : Sushrut Shendre
// ROll No. : 21112024

// Problem_02

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct Node {
	char item;
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

int search(char src[], char str[]) {
   int i, j, firstOcc;
   i = 0, j = 0;
   while (src[i] != '\0') {
      while (src[i] != str[0] && src[i] != '\0')
         i++;
      if (src[i] == '\0')
         return (-1);
      firstOcc = i;
      while (src[i] == str[j] && src[i] != '\0' && str[j] != '\0') {
         i++;
         j++;
      }
      if (str[j] == '\0')
         return (firstOcc);
      if (src[i] == '\0')
         return (-1);
      i = firstOcc + 1;
      j = 0;
   }
}

int find(char *str, char ch) {
	int i, flag = -1;
	for(i = 0; i < strlen(str); i++) {
		if(str[i] == ch) {
			flag = i;
			break;
		}
	}
	return flag;
}

void checkLine(char *filename) {
	Stack *s = malloc(sizeof(struct Stack));
	int count = 1, ignore = 0;
	char *str = malloc(1024);
	FILE *fp = fopen(filename, "r");
	if(fp != NULL) {
		int flag = 1;
		while(fgets(str, 1024, fp) != NULL) {
			if(strstr(str, "/*")) {
				ignore = 1;
			}
			if(ignore == 0) {
				if(strchr(str, '{')) {
					Node *node1 = malloc(sizeof(struct Node));
					node1->item = '{'; 
					push(node1, s);
				}
				if(strchr(str, '}')) {
					Node *temp = NULL;
					temp = pop(s);
					while(temp->item != '{') {
						temp = pop(s);
					}
				}
				if(find(str, '=') != -1) {
					int pos = find(str, '=');
					char var1, var2;
					int i = pos - 1;
					while(str[i] == ' ') {
						i--;
					}
					var1 = str[i];
					i = pos + 1;
					while(str[i] == ' ') {
						i++;
					}
					var2 = str[i];
					Node *temp = s->top;
					int flag1 = 1, flag2 = 1;
					while(temp != NULL) {
						if(temp->item == var1) {
							flag1 = 0;
						}
						if(temp->item == var2 || isdigit(var2)) {
							flag2 = 0;
						}
						temp = temp->next;
					}
					if(flag1 == 1) {
						if(flag == 1)
							printf("Errors.\n");
						printf("Line:%d \"%c = %c\" ===> %c is not defined\n", count, var1, var2, var1);
						flag = 0;
					}
					if(flag2 == 1) {
						if(flag == 1)
							printf("Errors.\n");
						printf("Line:%d \"%c = %c\" ===> %c is not defined\n", count, var1, var2, var2);
						flag = 0;
					}
				}
				if(search(str, "int") != -1) {
					int pos = search(str, "int");
					pos += 3;
					int i = pos;
					while(str[i] == ' ') {
						i++;
					}
					Node *temp = malloc(sizeof(struct Node));
					temp->item = str[i];
					temp->next = NULL;
					push(temp, s);
				}
			}
			if(strstr(str, "*/"))
				ignore = 0;
			count++;	
		}
		if(flag == 1) {
			printf("valid\n");
		}
	}
}


int main(int argc, char **argv) {
	checkLine(argv[1]);
}