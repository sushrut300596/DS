#include<stdio.h>
#include<ctype.h>
#include<stdbool.h>
#include<stdlib.h>

void push(int ,char *);
void check(int , char *,char *);
void pop(char *);

int sp=0;
int line_no=1;
static int new_line_index;
bool error_found=false;

int main(int argc , char *argv[])
{
 FILE *fp;
 int open_braces=0;
 int close_braces=0;

 if(argc>1)
  {
   fp=fopen(argv[1],"r");
   if(fp==NULL)
    {
     printf("File \"%s\" not Found!\n",argv[1]);
     exit(-1);
    }
  }
 else
   fp=fopen("example02.c","r");
 int ch;
 int i=0;
 int count=0;
 int isEqual=0;

 while((ch=getc(fp))!=EOF) 
 {
  count++;
 }

 fseek(fp,0,SEEK_SET);
 char file_data[count-1];
 char arr[count-1];

 int j=0;
 int k=0;

 while(1)
 {
  if(feof(fp))
    break;
  else
    {
     ch=fgetc(fp);
     if(ch=='{')
       open_braces++;
     if(ch=='}')
       close_braces++;
     file_data[i]=ch;
     if(open_braces==close_braces && open_braces>1)
       break;
    }
  i++;
 }
 file_data[i++]='\0';
 //printf("a\n");
 // printf("%s\n",file_data);
 //printf("b\n");
 i=0;
 //printf("count: %d\n",count); 
 while(file_data[i]!='\0')
 {
   //printf("i: %d\n",i);
   if(file_data[i]=='\n')
    {
     line_no++;
     //printf("line_no: %d\n",line_no);
     new_line_index=i;
     //printf("new_line_index: %d\n",new_line_index);
     //printf("i: %d\n",i);
    }
   else if(file_data[i]=='{')
    {
     push(file_data[i],arr);
     //printf("%s",arr);
    }

   else if(file_data[i]=='}')
    {
     pop(arr);
    }
   else if(isalpha((ch=file_data[i])) && file_data[i-1]==' ')
    {
     int j=i;
     int flag1=0;
     while(file_data[j]!='\n')
      {
       if(file_data[j]=='=')
        {
         check(ch,arr,file_data);
         flag1=1;
        }
        j--;
      }
     if(flag1)
      { 
       i++;
       continue;
      }
     j=i;
     while(file_data[j]!='\n')
      {
       if(file_data[j]=='=')
        {
         check(ch,arr,file_data);
         flag1=1;
        }
        j++;
      }
     if(flag1)
      {
        i++;
        continue;
      }

     if(file_data[i+1]==';')
      {
       int j=i;
       int flag=0;

       while(file_data[j]!='\n')
        {
         if(file_data[j]=='=')
          { 
           flag=1;
           break;
          }
         j--;
        }
       if(!flag)
         push(ch,arr);
      }
     else if(isEqual = 1 && file_data[i-1]==' ' && file_data[i+1]==' ' || file_data[i+1]==';')
      {
       check(ch,arr,file_data);
       isEqual=0;
      } 
     else 
      {
        while(isspace(file_data[i]) || file_data[i]=='\t')
         
        if(file_data[i]==';')
         {
           int j=i;
           int flag=0;

           while(arr[j]!='\n')
           {
             if(arr[j]=='=')
             { 
               flag=1;
               break;
             }
             j--;
           }
           if(!flag)
             push(ch,arr);
         }
        else if(file_data[i]=='=')
        {
         check(ch,arr,file_data);
         isEqual=1;
        }
      }
    }
    i++;
  }

 //printf("Loop end");
 if(!error_found)
   printf("Valid\n");

 return 0;
}

void push(int ch,char *arr)
{
  arr[sp++]=ch;
  arr[sp]='\0';
  //printf("push: %s\n",arr);
}

void pop(char *arr)
{
  while(arr[sp]!='{')
    sp--;
 // sp--;  
  arr[sp]='\0';
  //printf("pop: %s\n",arr);
}

void check(int ch , char *arr,char *file_data)
{
 int found=0;
 int i=0;
 int j=0;
 
 printf("checking character: %c",ch);
 while(i<sp)
  {  
   if(arr[i]==ch)
    {
     found=1;
     break;
    }
   i++;
  }
 i=1;
 if(!found)
  {
   //printf("new_line_index: %d",new_line_index);
   if(!error_found)
     printf("Errors.\n");
   error_found=true;
   printf("Line:%d \"",line_no); 
   while(file_data[new_line_index+i]!=';')
    {
     printf("%c",file_data[new_line_index+i]);
     i++;
    }
   printf("\" ==> ");
   printf("%c is not defined",ch);
   printf("\n");
  }
}
