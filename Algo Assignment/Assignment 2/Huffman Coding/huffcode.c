
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

typedef struct MaxHeapTreeNode
{
    char sym;
    int code[30];
    int prob;
    int top;
    struct MaxHeapTreeNode *lchild, *rchild;
}MaxHeapTreeNode;

typedef struct Node
{
    char sym;
    int code[30];
}Node;

void InsertandRemove(MaxHeapTreeNode arr[], int* n, MaxHeapTreeNode top)
{
    int i=2;
    int size = *n;
    for(;i<size-1;i++)
        if(arr[i].prob>top.prob)
            break;
    for(int j=size-1;j>i;j--)
        arr[j] = arr[j-1];
    arr[i] = top;

    for(int i=0;i<size-2;i++)
        arr[i] = arr[i+2];
    *n = size-1;
}

void CreateHeap(MaxHeapTreeNode arr[], int n, MaxHeapTreeNode **root)
{

    while(n > 2)
    {
        MaxHeapTreeNode *top = (MaxHeapTreeNode *)malloc(sizeof(MaxHeapTreeNode));
        MaxHeapTreeNode *lchildnode = NULL, *rchildnode = NULL;
        lchildnode = (MaxHeapTreeNode *)malloc(sizeof(MaxHeapTreeNode));
        rchildnode = (MaxHeapTreeNode *)malloc(sizeof(MaxHeapTreeNode));
        (*lchildnode) = arr[0];
        (*rchildnode) = arr[1];
        top->lchild = lchildnode;
        top->rchild = rchildnode;
        top->prob = lchildnode->prob + rchildnode->prob;
        InsertandRemove(arr, &n, *top);
        *root = top;
    }
}

void traverse(MaxHeapTreeNode *root, int c[], int n)
{
    MaxHeapTreeNode *temp1 = root->lchild;
    if(temp1)
    {
        int size = n;
        c[++size] = 0;
        temp1->top = size;
        for(int i=0;i<=size;i++)
            temp1->code[i] = c[i];
        traverse(temp1, c, n+1);
    }

    MaxHeapTreeNode *temp2 = root->rchild;
    if(temp2)
    {
        int size = n;
        c[++size] = 1;
        temp2->top = size;
        for(int i=0;i<=size;i++)
            temp2->code[i] = c[i];
        traverse(temp2, c, n+1);
    }
}

void sortData(MaxHeapTreeNode arr[], int n)
{
    for(int i=n-1;i>0;i--)
    {
        for(int j=0;j<i;j++)
        {
            if(arr[j].prob>arr[j+1].prob)
            {
                MaxHeapTreeNode temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}

MaxHeapTreeNode* ReadData(char fileread[], int *nos)
{
    MaxHeapTreeNode temp[256];
    for(int i=0;i<256;i++)
        temp[i].prob=0;
    FILE *fp = fopen(fileread, "r");
    char c;
    while((c = fgetc(fp)) != EOF)
    {
        temp[c].sym = c;
        temp[c].prob++;
    }
    for(int i=0;i<256;i++)
        if(temp[i].prob)
            (*nos)++;
    MaxHeapTreeNode *arr = (MaxHeapTreeNode *)malloc(((*nos)+1) * sizeof(MaxHeapTreeNode));
    int j=0;
    for(int i=0;i<256;i++)
    {
        if(temp[i].prob)
        {
            arr[j] = temp[i];
            arr[j].lchild = arr[j].rchild = NULL;
            arr[j].top = -1;
            j++;
        }
    }

    sortData(arr, (*nos));
    fclose(fp);

    return arr;
}

void table(MaxHeapTreeNode *trev, MaxHeapTreeNode codetable[], int *size)
{
    static int i=0;
    if(!trev)
        return;
    if(trev->lchild == NULL && trev->rchild == NULL)
        codetable[i++] = (*trev);

    if(trev->lchild)
        table(trev->lchild, codetable, size);
    if(trev->rchild)
        table(trev->rchild, codetable, size);
    (*size) = i;
}

void encode(MaxHeapTreeNode arr[], char fileread[], char filewrite[], int size)
{
    FILE *fread = fopen(fileread, "r");
    FILE *fwrite = fopen(filewrite, "w");
    char c;
    while((c = fgetc(fread)) != EOF)
    {
        for(int i=0;i<size;i++)
        {
            if(arr[i].sym == c)
            {
                for(int j=0;j<= arr[i].top;j++)
                    fprintf(fwrite, "%d", arr[i].code[j]);
                break;
            }
        }
    }
    fclose(fread);
    fclose(fwrite);
}

void decode(MaxHeapTreeNode *trev, char fileencode[], char filedecode[])
{
    FILE *fp = fopen(fileencode, "r");
    FILE *fwrite = fopen(filedecode, "w");
    char c;
    c = fgetc(fp);
    MaxHeapTreeNode *temp = trev;
    while(c != EOF && c != '\n')
    {
        if(trev->lchild == NULL && trev->rchild == NULL)
        {
            fprintf(fwrite, "%c", trev->sym);
            trev = temp;
        }
        if(c == '0')
            trev = trev->lchild;
        else if(c == '1')
            trev = trev->rchild;
        c =fgetc(fp);
    }
    fclose(fp);
    fclose(fwrite);
}

int compare(char fileread1[], char fileread2[])
{
    int size1=0, size2 = 0;
    char c;
    FILE *fp1 = fopen(fileread1, "r");
    FILE *fp2 = fopen(fileread2, "r");

    c = fgetc(fp1);
    while(c != EOF)
    {
        size1++;
        c = fgetc(fp1);
    }

    c = fgetc(fp2);
    while(c != EOF)
    {
        size2++;
        c = fgetc(fp2);
    }

    fclose(fp1);
    fclose(fp2);

    return size1 - size2;
}

unsigned long int countsize(char filename[])
{
    unsigned long int size = 0;
    FILE *fp = fopen(filename, "r");
    char c;
    while((c = fgetc(fp)) != EOF)
        size++;
    if(filename[0]>=48 && filename[0]<=57)
        size *= 8;
    return size;
}

void operation()
{   
    MaxHeapTreeNode *root;
    int size=0;
    MaxHeapTreeNode *arr = ReadData("1.txt", &size);
    CreateHeap(arr, size+1, &root);
    int code[30];
    traverse(root, code, -1);
     MaxHeapTreeNode codetable[256];
    int arrsize=0;
    table(root, codetable, &arrsize);

    FILE *fp = fopen("plaintext.txt", "r");
    FILE *fpwrite = fopen("result.csv", "w");
    fprintf(fpwrite, "OriginalSize,EncodedSize\n");
    unsigned long int size1=0, size2=0;
    char filetemp[20];
    for(int i=1;i<=20;i++)
    {
        char filename[20];
         fscanf(fp, "%s", filename);
        encode(codetable, filename, "encode.txt", arrsize);
        decode(root, "encode.txt", "decode.txt");

        size1 = countsize(filename);
        size2 = countsize("encode.txt");
        fprintf(fpwrite, "%lu,%lu\n", size1, size2);
    }
    fclose(fp);
    fclose(fpwrite);

    fpwrite = fopen("Table.txt", "w");
    fprintf(fpwrite, "Symbol      Encoded symbol\n");
    for(int i=0;i<arrsize;i++)
    {
        long long unsigned int res = 0;
        for(int j=0;j<=codetable[i].top;j++)
            res = 10*res + codetable[i].code[j];
        fprintf(fpwrite, "%c   %llu\n", codetable[i].sym, res);
    }
    fclose(fpwrite);
}

int main()
{
    operation();
}

