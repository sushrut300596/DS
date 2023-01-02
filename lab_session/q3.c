#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *delete_next (char *string, char *word) {
	char *token = strtok(string, " ");
	char *newString = malloc(100);
	*newString = '\0';
	while( token != NULL ) {
		//printf( "%s\n", token );
		strcat(newString, token);
		strcat(newString, " ");
		if(strcmp(word, token) == 0) {
			token = strtok(NULL, " ");
			token = strtok(NULL, " ");
		}
		else {
			token = strtok(NULL, " ");
		}
	}
	
	//printf("%s\n", newString);
	return newString;
}

int main () {
	char string[100] = "hello world hello pune";
	char word[20] = "hello";
	char *newString = delete_next(string, word);
	printf("%s\n", newString);
}