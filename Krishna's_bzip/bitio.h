#include<stdio.h>
#include<stdlib.h>

#define max_bits 30

FILE *fp,*fp2;
unsigned int bits_in_buf,bits_outstanding,fsize;
unsigned char buffer;

void start_encoding()
{
    buffer = 0;
    bits_in_buf = 0;
}

void start_decoding()
{
    buffer = 0;
    bits_in_buf = 0;
    fseek(fp2,0L,SEEK_END);
    fsize = ftell(fp2);
    fseek(fp2,0L,SEEK_SET);
}

void output_bit(int bit)
{
    buffer = buffer >> 1;
    if(bit)
        buffer |= 0x80;
    bits_in_buf++;
    if(bits_in_buf == 8)
    {
        fwrite(&buffer,sizeof(char),1,fp);
        bits_in_buf = 0;
    }
}

void done_outputing_bits()
{
    unsigned char num = 0;
    num = buffer >> bits_in_buf;
    if(bits_in_buf != 0)
        fwrite(&num,sizeof(char),1,fp);
}

void bit_plus_follow(int bit)
{
    int bit2;
    bit2 = !bit;
    output_bit(bit);
    while (bits_outstanding > 0)
    {
        output_bit(bit2);
        bits_outstanding -= 1;
    }
    
}

int input_bit()
{
    int bit, is_eof = 0;

    if(is_eof)
        return 0;
    
    if(bits_in_buf == 0)
    {
        fread(&buffer,sizeof(char),1,fp2);
        if(fsize == 0)
        {
            is_eof = 1;
            return 0;
        }
        bits_in_buf = 8;
        fsize--;
    }
    bit = buffer & 1;
    buffer >>= 1;
    bits_in_buf -= 1;
    return bit;
}

int *convert_to_bin(int n)
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

int convert_to_dec(int *bnum,int n)
{
    int bin = 0;
    int i,pow = 1;
    for(i=(n-1);i>=0;i--)
    {
        bin = bin + (bnum[i]) * pow;
        pow = pow << 1;
    }
    return bin;
}

void encode_transmit(int num,int max)
{
    int i,j,*bin;
    bin = convert_to_bin(num);

    i = bin[0];
    j = i - 1 ;
    int bit = 0;
    while (i < max)
    {
        output_bit(0);
        i++;
    }
    while (j >=0)
    {
        output_bit(bin[j+1]);
        j--;
    }
    
}

int receive_decode(int n)
{
    int i,j,num;
    int temp;
    int *bnum;
    bnum = (int *)malloc((n+1) * sizeof(int));
    for(i=0;i<n;i++)
    {
        bnum[i] = input_bit();
    }
    temp = convert_to_dec(bnum,n);
    free(bnum);

    return temp;
}
