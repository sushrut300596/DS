#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define BUF 10000000

typedef struct Node {
    char item;
    struct Node *next;
} Node;

typedef struct LinkedList {
    Node *head;
    int len;
} LinkedList;

char *buildArray() {
    int i;
    char *a = (char *)malloc(256 * sizeof(char));
    for(i = 0; i < 256; i++)
        a[i] = (char)i;
    return a;
}

LinkedList *build(int len) {
    char *a = (char *)malloc(256 * sizeof(char));
    LinkedList *ls = malloc(sizeof(struct LinkedList));
    Node *node = NULL, *prev = NULL;
    ls->len = len;
    int i;
    a = buildArray();
    for(i = 0; i < len; i++) {
        node = malloc(sizeof(struct Node));
        node->item = a[i];
        node->next = NULL;
        if(i == 0)
            ls->head = node;
        else
            prev->next = node;
        prev = node;
    }
    return ls;
}

int accessPatternHelper(char ch, LinkedList *ls) {
    int count = 0;
    Node *current = ls->head;
    Node *prev;
    if(current->item == ch)
        return 0;
    while(current->next != NULL) {
        if(current->next->item == ch)
            break;
        count++;
        current = current->next;
    }
    prev = current->next;
    current->next = current->next->next;
    prev->next = ls->head;
    ls->head = prev;
    return count + 1;
}

int *accessPattern(char *str, LinkedList *ls, int n) {
    int i;
    int *pattern = (int *)malloc(BUF * sizeof(int));
    for(i = 0; i < n; i++)
        pattern[i] = accessPatternHelper(str[i], ls);
    return pattern;
}

char findChar(int a, LinkedList *ls) {
    Node *current = ls->head;
    Node *prev;
    int count = 0;
    if(a == 0) {
        return current->item;
    }
    while(count < (a - 1)) {
        current = current->next;
        count++;
    }
    prev = current->next;
    current->next = current->next->next;
    prev->next = ls->head;
    ls->head = prev;
    return prev->item;
}

char *accessToString(int *pattern, LinkedList *ls, int n) {
    int i;
    char *str = (char *)malloc(n * sizeof(char));
    for(i = 0; i < n; i++)
        str[i] = findChar(pattern[i], ls);
    str[n] = '\0';
    return str;
}

char *readFile(char *filename) {
   char *str, *temp;
   str = (char*)(malloc(BUF * sizeof(char)));
   temp = (char*)(malloc(BUF * sizeof(char)));
   FILE *fp = fopen(filename, "r");
   if(fp != NULL)
      while(fgets(temp, BUF, fp) != NULL)
         strcat(str, temp);
   fclose(fp);
   return str;
}

int main(int argc, char **argv) {

    time_t begin = time(NULL);
    int i;
    LinkedList *ls = malloc(sizeof(struct LinkedList));
    ls = build(256);
    FILE *fp1 = fopen(argv[3], "w");
    /*struct Node *node = ls->head;
    while(node != NULL) {
        printf("%c ", node->item);
        node = node->next;
    }
    printf("\n");*/

    FILE *fp = fopen(argv[2], "w");
    char *str = readFile(argv[1]);
    int n = strlen(str);
    int *pattern = (int *)malloc(BUF * sizeof(int));
    pattern = accessPattern(str, ls, n);
    for(i = 0; i < n; i++)
        fprintf(fp, "%d\n",pattern[i]);
    ls = build(256);
    char *str1 = accessToString(pattern, ls, n);

    fprintf(fp1, "%s\n", str1);
    
    fclose(fp);

    time_t end = time(NULL);
    printf("Time taken in seconds : %ld\n", (end - begin));
}