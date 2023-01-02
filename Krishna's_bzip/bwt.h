#include<stdio.h>
#include<stdlib.h>
#include"bitio.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// Part 3

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define max_char 257

unsigned long half = 1<<(max_bits - 1 );
unsigned long f_qtr = 1<<(max_bits - 2);
#define t_qtr  3 * f_qtr

unsigned long L, R, H, V;

void encode_symbol(int l,int h,int slen)
{ 
    R = H - L + 1;
    H = L + (R * h)/slen - 1;
    L = L + (R * l)/slen;
    while (1)
    {
        if( H < half)
        { 
            bit_plus_follow(0);
        }
        else if( half <= L)
        {
            bit_plus_follow(1);
            L = L - half;
            H = H - half;
        }
        else if((L >= f_qtr) && (H < t_qtr))
        {
            bits_outstanding += 1;
            L = L - f_qtr;
            H = H - f_qtr;
        }
        else
        {
            break;
        }
        L = 2*L;
        H = 2 * H + 1;
    }
}

void arithmetic_encode_sequence(int *acp,int slen,int row)
{
    int *pc,i,j;
    pc = (int *)malloc(max_char*sizeof(int));

    for(i=0;i<max_char;i++)
        pc[i] = 0;

    for(i=0;i<slen;i++)
        pc[acp[i]]++;

    start_encoding();
    encode_transmit(slen,max_bits);
    encode_transmit(row,max_bits);

    for(i=1;i<max_char;i++)
    {
        encode_transmit(1+pc[i], max_bits);
        pc[i] = pc[i-1] + pc[i];
    }

    L = 0;
    H = (1<<max_bits)-1;
    bits_outstanding = 0;
    int l,h,s;

    for(i=0;i<slen;i++)
    {
        s = acp[i];
        l = pc[s-1];
        h = pc[s];
        encode_symbol(l,h,slen);
    }
    
    bits_outstanding += 1;
    if(L < (1<<(max_bits -2)))
        bit_plus_follow(0);
    else
        bit_plus_follow(1);
    
    encode_transmit(L,max_bits); 
    done_outputing_bits();
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void decode_symbol(int l,int h, int slen)
{
    R = H - L + 1;
    H = L + (R * h)/slen - 1;
    L = L + (R * l)/slen;
    while (1)
    {
        if(H  < half) 
        {
        }
        else if( half <= L)
        {  
            L = L - half;
            V = V - half;
            H = H - half;
        }
        else if((L >= f_qtr) && (H < t_qtr))
        {
            bits_outstanding += 1;
            L = L - f_qtr;
            V = V - f_qtr;
            H = H - f_qtr;
        }
        else
        {
            break;
        }
        L = 2*L;
        H = 2*H + 1;
        V = 2*V + input_bit();
    }
}

int *arithmetic_decode_sequence()
{
    int i,target,s;
    int slen,row,temp;
    
    start_decoding();
    slen = receive_decode(max_bits);
    row = receive_decode(max_bits);

    int *acp;
    acp = (int *)malloc((slen+2)*sizeof(int));
    acp[0] = slen;
    acp[1] = row;

    int *pc;
    pc = (int *)malloc(max_char*sizeof(int));

    for(i=0;i<max_char;i++)
        pc[i] = 0;

    for(i=1;i<max_char;i++)
    {
        temp = receive_decode(max_bits);
        pc[i] = temp - 1;
        pc[i] = pc[i-1] + pc[i];
    }

    L = 0;
    H = (1<<max_bits)-1;
    V = receive_decode(max_bits);

    int l,h,count;

    for(i=0;i<slen;i++)
    {
        s = 0;
        target = (((V - L + 1) * slen)- 1)/(H - L + 1); 
        while (1)
        {
            if(target < pc[s] )
                break;
            s++;
        }
        l = pc[s-1];
        h = pc[s];
        decode_symbol(l,h,slen);
        acp[i+2] = s;
    }
    return acp;
}

