#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"bwt.h"
#include"bwt12.h"

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
    lcol = (char *)malloc(slen*sizeof(char)); 

    row = bwtstr(str,lcol,slen); 
     
    printf("slen = %d\n",slen);
    printf("row = %d\n",row);
    printf("part 1 done\n");
    //////////////////////////////////////////////////////////////////////////
    //part 2
    int i,*ap_num;
    char ascii[256];
    for(int i=0;i<256;i++)
    {
        ascii[i] = (char)i;
    }

    LinkedList *l1,*l2;
    l1 = build(256,ascii);
    l2 = build(256,ascii);

    ap_num = ap_generate(lcol,l1,slen);


    printf("part 2 done\n");
    //////////////////////////////////////////////////////////////////////////
    //part 3

    fp = fopen("finaloutput.txt","wb");
    arithmetic_encode_sequence(ap_num,slen,row);
    fclose(fp);
    printf("part 3 done\n");

    //////////////////////////////////////////////////////////////////////////
    //part 3 reverse
    int *acp;
    fp2 = fopen("finaloutput.txt","rb");
    acp = arithmetic_decode_sequence();
    fclose(fp2);
    slen = acp[0];
    row = acp[1];
    printf("part 3 rev done\n");
    //////////////////////////////////////////////////////////////////////////
    //part 2 reverse
    char *lastcol;

    lastcol = ap_recover(acp+2,l2,slen);

    printf("part 2 rev done\n");
    //////////////////////////////////////////////////////////////////////////
    //part 1 reverse
    char *recvorg;
    recvorg = recoverstr(lastcol,row,slen); 

    FILE *recv;
    recv = fopen("finalrecv.txt","w");
    
    for(i=0;i<slen-1;i++)
        fprintf(recv,"%c",recvorg[i]);
    
    fclose(recv);
    printf("part 1 rev done\n");

    //printf("strcmp = %d\n",strcmp(recvorg,str));

}