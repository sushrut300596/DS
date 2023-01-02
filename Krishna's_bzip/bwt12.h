#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_SIZE 9000000

void displaystr(char *arr)
{
    printf("%s\n",arr);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Creating suffix array using skew algorithm

int leq(int a1, int a2, int b1, int b2) 
{ 
    return(a1 < b1 || a1 == b1 && a2 <= b2); 
} 

int leq2(int a1, int a2, int a3, int b1, int b2, int b3)
{ 
    return(a1 < b1 || a1 == b1 && leq(a2,a3, b2,b3)); 
} 

void radixPass(int* a, int* b, int* r, int n, int K)
{ 
    
    int *c,t;
    c = (int *)malloc((K+1) * sizeof(int));

    for (int i = 0; i <= K; i++) 
        c[i] = 0; 
    
    for (int i = 0; i < n; i++) 
        c[r[a[i]]]++; 
    
    for (int i = 0, sum = 0; i <= K; i++)
    { 
        t = c[i]; 
        c[i] = sum; 
        sum = sum + t; 
    }
    
    for (int i = 0; i < n; i++) 
        b[c[r[a[i]]]++] = a[i]; 
    
    free(c);
}


void suffixArray(int* T, int* SA, int n, int K)
{
    int n0=(n+2)/3, n1=(n+1)/3, n2=n/3, n02=n0+n2;
    int* R = (int *)malloc((n02+3)*sizeof(int));  
    R[n02]= R[n02+1]= R[n02+2]=0;
    
    int* SA12 = (int *)malloc((n02+3)*sizeof(int)); 
    SA12[n02]=SA12[n02+1]=SA12[n02+2]=0;
    
    int* R0 = (int *)malloc((n0)*sizeof(int));
    int* SA0 = (int *)malloc((n0)*sizeof(int));

    for (int i=0, j=0; i < n+(n0-n1); i++) 
        if (i%3 != 0) 
            R[j++] = i;

    radixPass(R , SA12, T+2, n02, K);
    radixPass(SA12, R , T+1, n02, K);
    radixPass(R , SA12, T , n02, K);

    int name = 0, c0 = -1, c1 = -1, c2 = -1;
    for (int i = 0; i < n02; i++) 
    {
        if (T[SA12[i]] != c0 || T[SA12[i]+1] != c1 || T[SA12[i]+2] != c2)
        { 
            name++; 
            c0 = T[SA12[i]]; 
            c1 = T[SA12[i]+1]; 
            c2 = T[SA12[i]+2]; 
        }
        if (SA12[i] % 3 == 1) 
        { 
            R[SA12[i]/3] = name;
        } 
        else 
        { 
            R[SA12[i]/3 + n0] = name; 
        } 
    }

    if (name < n02) 
    {
        suffixArray(R, SA12, n02, name);
        for (int i = 0; i < n02; i++) 
            R[SA12[i]] = i + 1;
    } 
    else 
        for (int i = 0; i < n02; i++) 
            SA12[R[i] - 1] = i;


    for (int i=0, j=0; i < n02; i++) 
        if (SA12[i] < n0) 
            R0[j++] = 3*SA12[i];
    
    radixPass(R0, SA0, T, n0, K);

    for (int p=0, t=n0-n1, k=0; k < n; k++) 
    {
        #define GetI() (SA12[t] < n0 ? SA12[t] * 3 + 1 : (SA12[t] - n0) * 3 + 2)
        int i = GetI(); 
        int j = SA0[p]; 
        if (SA12[t] < n0 ?  leq(T[i], R[SA12[t] + n0], T[j], R[j/3]) : leq2(T[i],T[i+1],R[SA12[t]-n0+1], T[j],T[j+1],R[j/3+n0]))
        { 
            SA[k] = i; t++;
            if (t == n02) 
                for (k++; p < n0; p++, k++) 
                    SA[k] = SA0[p];
        } 
        else 
        { 
            SA[k] = j; p++;
            if (p == n0) 
                for (k++; t < n02; t++, k++) 
                    SA[k] = GetI();
        }
    }
    
    free(R); 
    free(SA12); 
    free(SA0); 
    free(R0);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Last column to original string using count sort


char *recoverstr(char *lcol,int row,int slen)
{
    int i,j,sum;
    int *count,*pos,*c;
    char *org;
    count = (int *)malloc(256 * sizeof(int));
    org  = (char *)malloc(slen * sizeof(char));
    pos = (int *)malloc(256 * sizeof(int));
    c = (int *)malloc(slen * sizeof(int));

    for(i=0;i<256;i++)
        count[i] = 0;

    for(i=0;i<slen;i++)
    {
        c[i] = count[(int)lcol[i]];
        count[(int)lcol[i]] = count[(int)lcol[i]] + 1;
    }

    sum = 0;
    for(i=0;i<256;i++)
    {
        pos[i] = sum;
        sum = sum + count[i];
    }
    
    j = row;
    for(i=slen-1;i>=0;i--)
    {
        org[i] = lcol[j];
        j = c[j] + pos[(int)lcol[j]];
    }

    free(count);
    free(pos);
    free(c);
    return org;
}

int getdatafile(char *fname,char *str)
{
    int n,size;
    FILE *fp;
    fp = fopen(fname,"r");
    if(fp == NULL)
    {
        printf("Unable to open file\n");
        exit(EXIT_FAILURE);
    }
    
    int j;
    j = 0;
    char ch;
    while (1)
    {
        if((ch = fgetc(fp)) == EOF)
            break;
        str[j] = ch;
        j++;
    }

    n = j;
    str[n] = '$';
    n = n+1;
    return n;
}

int bwtstr(char *str, char *lcol, int slen)
{
    int i;
    int *sa,*t,row;
    sa = (int *)malloc(slen*sizeof(int));
    t = (int *)malloc(slen*sizeof(int));
    
    for(i=0;i<slen;i++)
        t[i] = (int)str[i];
    
    suffixArray(t,sa,slen,255);

    for(i=0;i<slen;i++)
    {
        if(sa[i] == 0)
        {
            lcol[i] = str[slen-1];
            row = i;
            continue;
        }
        lcol[i] = str[sa[i] - 1];
    }
    return row;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// Part 2

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct Node {
  char item;           
  struct Node *next;  
}Node;

typedef struct LinkedList {
  Node *head;   
  int len;     
}LinkedList;

LinkedList *build(int len,char *asc) {
  int i=0;
  LinkedList *linkls = malloc(sizeof(LinkedList));
  Node *fstnode,*temp;
  
  fstnode = (struct Node*)malloc(sizeof(Node));
  fstnode -> item = asc[i];
  fstnode -> next = NULL;
  temp = fstnode;
  
  linkls -> head = fstnode;
  linkls -> len = len;
  
  for(i=1;i<len;i++)
  {
    Node *node = (struct Node*)malloc(sizeof(Node));
    node -> item = asc[i];
    node -> next = NULL;
    temp -> next = node;
    temp = temp -> next;

  }
  return linkls;
}

int strtonum(char str,LinkedList *linkls)
{
    int i,count = 0,found = 0;
    Node *temp,*temp2;
    temp = linkls -> head;

    if(temp -> item == str)
    {
        return 0;
    }
    while (temp -> next != NULL)
    {
        if(temp -> next -> item == str)
        {
            found = 1;
            break;
        }
        count++;
        temp = temp -> next;
    }
    if(found == 0)
    {
        printf("error character not in ascii\n");
        exit(1);
    }
    temp2 = temp -> next;
    temp -> next = temp -> next -> next;
    temp2 -> next = linkls -> head;
    linkls -> head = temp2;
    return count+1;
}

char numtostr(int num,LinkedList *linkls)
{
    int count = 0;
    Node *temp,*temp2;
    char c;
    temp = linkls -> head;

    if(num ==0)
    {
        return temp ->item;
    }
    
    while (count < num-1)
    {
        temp = temp->next;
        count++;
    }
    temp2 = temp->next;
    temp -> next = temp -> next -> next;
    temp2 -> next = linkls -> head;
    linkls -> head = temp2;
    return temp2 -> item; 
}

int *ap_generate(char *str,LinkedList *ls,int slen)
{
    int *ap;
    ap = (int *)malloc(slen*sizeof(int));
    for (int i = 0; i < slen; i++)
    {
        ap[i] = strtonum(str[i],ls) + 1;
    }
    return ap;
}

char *ap_recover(int *str,LinkedList *ls,int slen)
{
    char *ap;
    ap = (char *)malloc(slen*sizeof(char));
    for (int i = 0; i < slen; i++)
    {
        ap[i] = numtostr(str[i] - 1,ls);
    }
    return ap;
}