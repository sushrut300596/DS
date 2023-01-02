#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"bwt.h"
#include"bwt12.h"
#include"enc.h"
#include"dec.h"
#include"compress.h"


int main(int argc, char **argv)
{

    //////////////////////////////////////////////////////////////////////////
    //part 1
    int slen,row;
    if(argc != 2)
    {
        printf("Enter file name of input file in terminal\n");
        exit(EXIT_FAILURE);
    }
    
    char *fname,*str;
    fname = (char *)malloc(200*sizeof(char));
    str = (char *)malloc(MAX_SIZE*sizeof(char));

    
    fname = argv[1];
    slen = getdatafile(fname,str);

    char *lcol;
    lcol = (char *)malloc(slen*sizeof(char)); // Last column of input

    row = bwtstr(str,lcol,slen); // row of original string
    
    
    printf("slen = %d\n",slen);
    printf("row = %d\n",row);
    printf("part 1 done\n");
    //lcol = str;
    //////////////////////////////////////////////////////////////////////////
    //part 2

    int i,j,*numarr;
    numarr = (int *)malloc(slen*sizeof(int));
    char asc[256];
    for(i=0;i<256;i++)
    {
        asc[i] = (char)i;
    }
    LinkedList *asci,*l1,*l2;
    l1 = build(256,asc);
    l2 = build(256,asc);
    for(i=0; i<slen; i++)
    {
        numarr[i] = strtonum(lcol[i],l1) + 1;
    }


    printf("%d\n",numarr[0]);
    printf("part 2 done\n");
    //////////////////////////////////////////////////////////////////////////
    //part 3
    // FILE *new;
    // new = fopen("access.txt","r");
    // int *numarr;
    // numarr = malloc()

    // fp = fopen("finaloutput.txt","w");
    // arithmetic_encode_sequence(numarr,slen,row);
    // fclose(fp);
    // printf("part 3 done\n");
    fp = fopen("finaloutput.txt","w");
    fp1 = fp;
    start_model();
    encode_start();
    encode(numarr,slen);

    //////////////////////////////////////////////////////////////////////////
    //part 3 reverse
    int *acp;
    fp2 = fopen("finaloutput.txt","r");
    acp = arithmetic_decode_sequence();
    fclose(fp2);
    slen = acp[0];
    row = acp[1];
    
    printf("slen = %d\n",slen);
    printf("row = %d\n",row);
    printf("part 3 rev done\n");
    //////////////////////////////////////////////////////////////////////////
    //part 2 reverse
    char *lastcol;
    lastcol = (char *)malloc(slen*sizeof(char));
    for(i=0; i<slen; i++)
    {
        lastcol[i] = numtostr(acp[i+2] - 1,l2);
    }

    printf("part 2 rev done\n");
    //////////////////////////////////////////////////////////////////////////
    //part 1 reverse
    char *recvorg;
    recvorg = recoverstr(lcol,row,slen); //recovered string

    FILE *recv;
    recv = fopen("finalrecv.txt","w");
    for(i=0;i<slen-1;i++)
        fprintf(recv,"%c",recvorg[i]);
    
    fclose(recv);
    printf("part 1 rev done\n");

}