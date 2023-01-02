// Name : Sushrut Shendre
// Roll No. : 21112024

// Problem_01

// For run command : ./problem01 filename(which you want reduce) filename(where you want to save reduced file) filename(which you want to recover) filename(where you want to save recovered file)
// e.g., ./problem01 original.txt code1.c code1.c original1.txt

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void trimTrailing(char *str, char end)
{
    int index, i;
    index = -1;
    i = 0;
    while(str[i] != '\0')
    {
        if(str[i] != end)
        {
            index= i;
        }
        i++;
    }
    str[index + 1] = '\0';
}


void reduce(char *filename1, char *filename2) {
	char *str = malloc(1024);
	char *temp = malloc(1024);
	int count = 1;
	int i, j;
	FILE *fp1 = fopen(filename1, "r");
	FILE *fp2 = fopen(filename2, "w");
	FILE *fp3 = fopen("reduce.txt", "w");
	if(fp1 != NULL) {
		while(fgets(str, 1024, fp1) != NULL) {
			if((str[0] == '/' && str[1] == '/') || count == 1) {
				fprintf(fp3, "%s", str);
			}
			else {
				for(i = 0; i < 8; i++) {
					fputc(str[i], fp3);
				}
				fputc('\n', fp3);
				for(i = 8, j = 0; i < strlen(str) - 1; i++, j++) {
					temp[j] = str[i];
				}
				temp[j] = '\0';
				trimTrailing(temp, '/');
				trimTrailing(temp, ' ');
				strcat(temp, "\n");
				fprintf(fp2, "%s", temp); 
			}
			count++;
		}
	}
	fclose(fp1);
	fclose(fp2);
	fclose(fp3);
}

void recover(char *filename1, char *filename2) {
	FILE *fp1 = fopen(filename1, "r");
	FILE *fp2 = fopen(filename2, "w");
	FILE *fp3 = fopen("reduce.txt", "r");
	int count = 1, length = 1, i;
	char *str = malloc(1024), *temp = malloc(1024);
	if(fp1 != NULL && fp3 != NULL) {
		while(fgets(str, 1024, fp3) != NULL) {
			if((strstr(str, "//")) || count == 1) {
				fprintf(fp2, "%s", str);
				if(length == 1)
					length = strlen(str);
			}
			else {
				fgets(temp, 1024, fp1);
				str[strlen(str) - 1] = '\0';
				temp[strlen(temp) - 1] = '\0';
				strcat(str, temp);
				for (i = strlen(str); i < length - 5; i++){
					str[i] = ' ';
				}
				while(i < length - 1){
					str[i++] = '/';
				}
				str[i++] = '\n';
				str[i] = '\0';
				fprintf(fp2, "%s", str);
			}
			// printf("%s", str);
			count++;
		}
	}
	fclose(fp1);
	fclose(fp2);
	fclose(fp3);
}

int main(int argc, char **argv) {
	reduce(argv[1], argv[2]);
	recover(argv[3], argv[4]);
}