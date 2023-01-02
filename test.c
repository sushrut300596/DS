#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct Node {
    char data;
    struct Node *next;
} Node;

typedef struct LinkedList {
    Node *head;
    int len;
} LinkedList;

void display(char *str,int len)
{
    int i=0;
    printf("\n recoved string is:");
    
    for(i=0;i<len;i++)
    {
    printf("%c",str[i]);
    }
    
    printf("\n");
}


LinkedList *build(int len)
 {
    char *a = (char *)(malloc(256*sizeof(char)));
    
    LinkedList *links = malloc(sizeof(struct LinkedList));
    Node *node = NULL, *prev = NULL;
    links->len = len;
    
    int i = 0;
    while(i < len) {
        node = (Node*)malloc(sizeof(struct Node));
        node->data =(char)i;
        node->next = NULL;
        
        if(i == 0)
            links->head = node;
            
        else
            prev->next = node;
        prev = node;
        i++;
    }
    return links;
}

int *accessCode(char *str, LinkedList *links) {
    int i;
    int len = strlen(str);
    int *code = (int*)(malloc(len*sizeof(int)));
    Node *prev;    
    for(i = 0; i < len; i++) {
        int count = 0;
        Node *curr = links->head;
        if(curr->data == str[i])
        {
            code[i] = 0;
            continue;
        }
        while(curr->next != NULL){
            if(curr->next->data == str[i]){
                break;
            }
            curr = curr->next;
            count++;
        }
        code[i] = count + 1;
        prev = curr->next;
        curr->next = curr->next->next;
        prev->next = links->head;
        links->head = prev;
    }
    return code;
}

char *accessString(int *str, LinkedList *links, int len) {
    int i, count = 0;

    char *string = (char*)(malloc(len*sizeof(char)));
    Node *curr = links->head;
    Node *prev;    
    for(i = 0; i < len; i++) {
        count = 0;
        int j = 0;
        Node *curr = links->head;
        if(str[i] == 0)
        {
            string[i] = curr -> data;
            continue;
        }
        while(count < str[i] - 1){
            count++;
            curr = curr -> next;
        }
        string[i] = curr -> next -> data;
        prev = curr->next;
        curr->next = curr->next->next;
        prev->next = links->head;
        links->head = prev;
    }
    return string;
}


int main() {
    int i;
    LinkedList *links = malloc(sizeof(struct LinkedList));
    links = build(256);
   
    printf("Entered string is: \n");
    char *str = "nnbaaa";
    printf("%s\n", str);
    int len = strlen(str);
    int *code = (int*)malloc(strlen(str));
    code = accessCode(str, links);
    
    printf("code of string is:\n ");
    for(i = 0; i < strlen(str); i++) {
        printf("%d\n ", code[i]);
    }
    
    links = build(256);
    char *stringrev = (char*)malloc(strlen(str));
    stringrev = accessString(code, links, strlen(str));
    display(stringrev,len);
}