#include<stdio.h>
#include<stdlib.h>
#include<string.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// Part 3

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define max_value 20
#define max_char 256
#define max_bits 20

unsigned int L, R , V, bits_outstanding;
FILE *fp,*fp2;
unsigned int buffer,bits_to_go;

int *convert_to_bin2(int n)
{
    int *bnum;
    bnum = (int *)malloc((max_bits+1)*sizeof(int));
  
    int i = 0;
    while (n > 0) {
        bnum[i + 1] = n % 2;
        n = n / 2;
        i++;
    }
    bnum[0] = i ;
    return bnum;
}

void output_bit(int bit)
{
    buffer |= bit;
    buffer << 1;
    bits_to_go++;
    if(bits_to_go == 32)
    {
        fprintf(fp,"%d",buffer);
        buffer = 0;
        bits_to_go = 0;
    }
}

void done_outputing_bits()
{
	fprintf(fp,"%d",buffer << (32 - bits_to_go ));
}

void encode_transmit2(int num,int max)
{
    int i,j,*bin;
    bin = convert_to_bin2(num);

    i = bin[0];
    j = i - 1 ;
    int bit = 0;
    while (i < max)
    {
        fputc(bit + '0',fp);
        //fprintf(fp,"0");
        i++;
    }
    while (j >=0)
    {
        fputc( (bin[j+1] + '0'),fp);
        //fprintf(fp,"%d",bin[j+1]);
        j--;
    }
    
}


void bit_plus_follow(int bit)
{
    int bit2;
    bit2 = !bit;
    output_bit(bit);
    //fputc(bit,fp);
    //fprintf(fp,"%d",bit);
    while (bits_outstanding > 0)
    {
        output_bit(bit2);
        //fprintf(fp,"%d",bit2);
        //fputc(bit2,fp);
        bits_outstanding -= 1;
    }
    
}

void encode_symbol(int l,int h,int slen)
{
    int i,j,k,t; 
    L = L + R*l/slen;
    R =  R*h/slen - R*l/slen;
    j = 1<<(max_value-1);
    k = 1<<(max_value-2);
    t = k * 3;
    while (R <= k)
    {
        if((L + R ) <= j)
        { 
            bit_plus_follow(0);
        }
        else if( j <= L)
        {
            bit_plus_follow(1);
            L = L - j;
        }
        else if((L >= k) && ((L + R) <= t))
        {
            bits_outstanding += 1;
            L = L - k;
        }
        else
        {
            break;
        }
        L = 2*L;
        R = 2*R + 1;
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

    encode_transmit2(slen,max_bits);
    encode_transmit2(row,max_bits);

    for(i=1;i<256;i++)
    {
        encode_transmit2(1+pc[i], max_bits);
        pc[i] = pc[i-1] + pc[i];
    }

    L = 0, R = 1<<(max_value-1);
    bits_outstanding = 0;
    buffer = 0;
    bits_to_go = 0;
    int l,h,s;

    for(i=0;i<slen;i++)
    {
        s = acp[i];
        l = pc[s-1];
        h = pc[s];
        encode_symbol(l,h,slen);
    }
    
    bits_outstanding += 1;
    if(L < (1<<(max_value -2)))
        bit_plus_follow(0);
    else
        bit_plus_follow(1);
    
    encode_transmit2(L,max_value); 
    done_outputing_bits();
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int get_one_bit()
{
    char ch;
    int bit;
    ch = fgetc(fp2);
    bit = ch - '0';
    return bit;
}

long double convert_to_dec2(char *bnum,int n)
{
    long double j,bin = 0;
    long double power = 1;
    int i;
    for(i=(n-1);i>=0;i--)
    {
        bin = bin + ((bnum[i] - '0') * power);
        power = power * 2;
    }
    return bin;
}

int receive_decode(int n)
{
    int i,j,num;
    int temp;
    char *bnum;
    bnum = (char *)malloc((n+1) * sizeof(char));
    for(i=0;i<n;i++)
    {
        bnum[i] = fgetc(fp2);
    }
    temp = (int)convert_to_dec2(bnum,n);;

    return temp;
}

int get_one_integer(int num)
{
    return receive_decode(num);
}

int decode_target(int t)
{
    return (((V - L + 1) * t)- 1)/R;
}

void decode_symbol(int l,int h, int slen)
{
    int i,j,k,t;
    L = L + R*l/slen;
    R = R*h/slen - R*l/slen;
    j = 1<<(max_value-1);
    k = 1<<(max_value-2);
    t = k*3;
    while (R <= k)
    {
        if((L + R ) <= j) 
            {}
        else if( j <= L)
        {  
            L = L - j;
            V = V - j;
        }
        else if((L >= k) && ((L + R)<= t))
        {
            bits_outstanding += 1;
            L = L - k;
            V = V - k;
        }
        else
        {
            break;
        }
        L = 2*L;
        R = 2*R + 1;
        V = 2*V + get_one_bit();
    }
}

int *arithmetic_decode_sequence()
{
    int i,slen,n,temp,target,s,row;
    slen = receive_decode(max_bits);
    row = receive_decode(max_bits);

    int *pc,*acp;
    pc = (int *)malloc(max_char*sizeof(int));
    acp = (int *)malloc((slen+2)*sizeof(int));
    acp[0] = slen;
    acp[1] = row;
    n = max_char;


    for(i=0;i<n;i++)
        pc[i] = 0;

    for(i=1;i<n;i++)
    {
        temp = receive_decode(max_bits);
        pc[i] = temp - 1;
        pc[i] = pc[i-1] + pc[i];
    }

    L = 0, R = 1<<(max_value-1);
    V = get_one_integer(max_value);

    int l,h,count;

    for(i=0;i<slen;i++)
    {
        s = 0;
        target = decode_target(slen); 
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

