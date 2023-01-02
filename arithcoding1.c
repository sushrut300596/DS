#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define max_value 8
#define max_char 256
#define max_bits 8

#define BUF 1000000

int L, R , V, bits_outstanding;
FILE *fp,*fp2;
int n;


long convert_to_bin(int n)
{
    long long bin = 0;
    int rem, i = 1;

    while (n!=0) 
    {
        rem = n % 2;
        n /= 2;
        bin += rem * i;
        i *= 10;
    }
    return bin;
}

void encode_transmit(int num)
{
    int i;
    long bin;
    char *bnum;
    bnum = (char *)malloc(max_bits * sizeof(char));
    bin = convert_to_bin(num);
    sprintf(bnum,"%ld",bin);

    i = strlen(bnum);
    while (i < max_bits )
    {
        fprintf(fp,"0");
        i++;
    }
    fprintf(fp,"%ld",bin);
    
}

void bit_plus_follow(int bit)
{
    int bit2;
    bit2 = !bit;
    //fputc(bit,fp);
    fprintf(fp,"%d",bit);
    while (bits_outstanding > 0)
    {
        fprintf(fp,"%d",bit2);
        //fputc(bit2,fp);
        bits_outstanding -= 1;
    }
    
}

void encode_symbol(int l,int h,int slen)
{
    int i,j,k;
    L = L + R*l/slen;
    R = R*h/slen - R*l/slen;
    j = 1<<(max_value-1);
    k = 1<<(max_value-2);
    while (R <= k)
    {
        if((L + R ) <= j) 
            bit_plus_follow(0);
        else if( j <= L)
        {
            bit_plus_follow(1);
            L = L - j;
        }
        else
        {
            bits_outstanding += 1;
            L = L - k;
        }
        L = 2*L;
        R = 2*R;
    }
}

void arithmetic_encode_sequence(int *acp,int slen)
{
    int *pc,i,j;
    pc = (int *)malloc(max_char*sizeof(int));

    for(i=0;i<max_char;i++)
        pc[i] = 0;

    for(i=0;i<slen;i++)
        pc[acp[i]]++;

    encode_transmit(slen);
    encode_transmit(max_char-1);

    for(i=1;i<256;i++)
    {
        encode_transmit(1+pc[i]);
        pc[i] = pc[i-1] + pc[i];
    }
    L = 0, R = 1<<(max_value-1);
    bits_outstanding = 0;
    int l,h,s;

    for(i=0;i<slen;i++)
    {
        s = acp[i];
        l = pc[s-1];
        h = pc[s];
        encode_symbol(l,h,slen);
    }
    encode_transmit(L); //as one integer b bits long as using bit plus bollow
}

int get_one_bit()
{
    char ch;
    int bit;
    ch = fgetc(fp2);
    bit = ch - '0';
    return bit;
}

int convert_to_dec(long n)
{
    int dec = 0, i = 0, rem,pow;

    while (n!=0)
    {
        rem = n % 10;
        n /= 10;
        pow = 1<<i;
        dec += rem * pow;
        ++i;
    }
    return dec;
}

int receive_decode(int n)
{
    int i,j,num;
    long temp;
    char *bnum;
    bnum = (char *)malloc((n+1) * sizeof(char));
    for(i=0;i<n;i++)
    {
        bnum[i] = fgetc(fp2);
    }
    temp =(long) atoi(bnum);
    num = convert_to_dec(temp);

    return num;
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
    int i,j,k;
    L = L + R*l/slen;
    R = R*h/slen - R*l/slen;
    j = 1<<(max_value-1);
    k = 1<<(max_value-2);
    while (R <= k)
    {
        if((L + R ) <= j) 
            {}
        else if( j <= L)
        {
            
            L = L - j;
            V = V - j;
        }
        else
        {
            bits_outstanding += 1;
            L = L - k;
            V = V - k;
        }
        L = 2*L;
        R = 2*R;
        V = 2*V + get_one_bit();
    }
}

int *arithmetic_decode_sequence()
{
    int i,slen,n,temp,target,s;
    slen = receive_decode(max_value);
    n = receive_decode(max_value);
    n += 1;

    int *pc,*acp;
    pc = (int *)malloc(max_char*sizeof(int));
    acp = (int *)malloc(slen*sizeof(int));

    for(i=0;i<n;i++)
        pc[i] = 0;

    for(i=1;i<n;i++)
    {
        temp = receive_decode(max_value);
        pc[i] = temp - 1;
        pc[i] = pc[i-1] + pc[i];
    }

    L = 0, R = 1<<(max_value-1);
    V = get_one_integer(max_value);
    int l,h;

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
        acp[i] = s;
    }
    return acp;
}

int *readIntFile(char *filename) {
    char *temp;
    int *str;
    str = (int *)(malloc(1000 * BUF * sizeof(int)));
    temp = (char *)(malloc(BUF * sizeof(char)));
    FILE *fp = fopen(filename, "r");
    int i = 0;
    if(fp != NULL) {
        while(fgets(temp, BUF, fp) != NULL) {
            //printf("%s\n", temp);
            str[i] = atoi(temp);
            //printf("%d\n", str[i]);
            i++;
        }
    }
    n = i;
    fclose(fp);
    return str;
}

int main(int argc, char **argv)
{
    //int arr[]={97,98,97,97,97,101,97,97,97,97}, n;
    // int arr[]={100}, n;
    int *str = readIntFile(argv[1]);
    fp = fopen("output.txt","w");
    arithmetic_encode_sequence(str,n);
    fclose(fp);

    int *acp;
    fp2 = fopen("output.txt","r");
    acp = arithmetic_decode_sequence();
    fclose(fp2);
    for(int i=0;i<n;i++)
        printf("%d\n",acp[i]);
}