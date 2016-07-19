#include<stdio.h>
#include<stdlib.h>
#include "heap.h"
int number=0;
int len=0;
int len2=0;
int len_attr=0;
int *ch,*y,*attr,*maxAttr,*diff_table,*freeAttr;//*value

int x=0;
node* q[50];
int front=-1;
int rear=-1;
typedef struct pair{
    char c[40];
    int val;
}Player;

Player *array;

void fromFile()
{
    FILE *bat,*bow,*wic,*all;
    int p,k;
    FILE **attr_txt = malloc(x* sizeof(FILE* ));
    char **fn = (char **)malloc(x * sizeof(char *));
    for (p=0; p<x; p++)
         fn[p] = (char *)malloc(15 * sizeof(char));

   // int nBat=41,cBat=3,nBow=59,cBow=2,nWic=24,cWic=1,nAll=39,cAll=2;
    int i,j=0,plno=0;//plno= player no,ie index of array of players
    array=(Player*)calloc(len,sizeof(Player));
    for(k=0;k<x;k++)
    {
        printf("enter name of file containing attribute %d",k+1);
        gets(fn[k]);
        attr_txt[k]=fopen(fn[k],"r");
        i=0;
        do{
            fscanf(attr_txt[k],"%d",&array[plno].val);
            fgets(array[plno].c,40,bat);
            plno++;
        }while(++i<y[j++]);
    }
        for (i=0;i<len;i++)
            printf("\n%d,%s",array[i].val,array[i].c);

}

int isempty()
{
    if(front==-1&&rear==-1)
        return 1;
    return 0;
}

void enqueue(node* ptr)
{
        if(isempty()==1)
        {
            front=rear=0;
            q[rear]=ptr;
        }
        else
            q[++rear]=ptr;
}

node* dequeue()
{
    int temp=front;
        if(front==rear)
        {
            front=rear=-1;
            return q[temp];

        }
        else
        {
            front++;
            return q[temp];
        }


}

void quickSort(int p,int r)
{
int k,i=p,j=p,temp;
if(p!=r)
	{
	for(k=p;k<r;k++)
	{
		if(array[k].val>array[r].val)
		{

				temp=array[k].val;
				array[k].val=array[i].val;
				array[i].val=temp;

			i++;
		}
		j++;
	}
	temp=array[r].val;
	array[r].val=array[i].val;
	array[i].val=temp;
	if(i!=p)
		quickSort(p,i-1);
	if(i!=r)
		quickSort(i+1,r);
	}
}

void dt_gen(int si,int ei)
{
    int i;
    for(i=si;i<ei;i++)
    {
        diff_table[i]=array[i].val-array[i+1].val;
    }
}

node* createNode(node *parent,int br)
{
    int i;
    node* n=(node*)malloc(1*sizeof(node));
    n->no=++number;
    n->firstBreak=br;
    n->arr=(int*)malloc(len*sizeof(int));
	n->val=parent->val;
    for(i=0;i<len;i++)
    {
        n->arr[i]=parent->arr[i];
    }
    return n;
}

node* generate_initial()
{
    int i,j,k=0,p=0,si;
    int *z,*initial;
	int val=0;
    node* n;
    printf("\n enter number of attributes");
    scanf("%d",&x);
    y=(int*)malloc(x*sizeof(int));
    z=(int*)malloc(x*sizeof(int));
    attr=(int*)malloc((x+1)*sizeof(int));
    maxAttr=(int*)malloc(x*sizeof(int));
    freeAttr=(int*)malloc(x*sizeof(int));
    ch=(int*)calloc(x,sizeof(int));
    for(i=0;i<x;i++)
    {
        printf("\n enter total number of elements having attribute %d and number of required elements from attribute %d\n",i+1,i+1);
        scanf("%d",&y[i]);
        scanf("%d",&z[i]);
        len=len+y[i];
        len2=len2+z[i];
        freeAttr[i]=y[i]-z[i];
        //len_attr=len_attr+z[i];

    }
    initial=(int*)malloc(len*sizeof(int));
    //value=(int*)malloc(len*sizeof(int));
    /*for(i=0;i<x;i++)
    {
        //printf("\n enter %d values of attribute %d",y[i],i+1);
        for(j=0;j<y[i];j++)
            value[p++]=rand()%10+1;
    }*/
    fromFile();
    diff_table=(int*)malloc((len-1)*sizeof(int));
    printf("\nbefore sorting");
    for(i=0;i<len;i++)
        printf("%d ",array[i].val);
    for(i=0;i<x;i++)
    {
       si=0;
       for(j =0;j<i;j++)
       {
               si=si+y[j];
       }
       quickSort(si,si+y[i]-1);
       dt_gen(si,si+y[i]-1);
    }
    printf("\n after sorting");
    for(i=0;i<len;i++)
        printf("%d ",array[i].val);
    printf("\n\ndifference table");
    for(i=0;i<len;i++)
    {
        printf("%d ",diff_table[i]);
    }
    i=0;
    while(k<x)
    {

        if(k==0)
        {
            attr[k]=0;
        }
        else
        {
            attr[k]=attr[k-1]+y[k-1];
        }
        maxAttr[k]=attr[k]+z[k]-1;
        j=0;
        while(j<z[k])
        {
            initial[i++]=1;
            j++;
        }
        while(j<y[k])
        {
            initial[i++]=0;
            j++;
        }
        k++;
    }
    attr[x]=len;
    for(i=0;i<x;i++)
    {
      printf("attr-:%d ",attr[i]);
     printf("maxattr-:%d  \n%d ",maxAttr[i]);
    }
    n=(node*)malloc(1*sizeof(node));
    n->no=0;

    n->arr=(int*)malloc(len*sizeof(int));
    for(i=0;i<len;i++)
    {
         if(initial[i]!=0)
			 val+=array[i].val;
		 n->arr[i]=initial[i];
    }
	n->val=val;
	printf("The value is %d",n->val);
return n;
}

void display(node *n)
{
   int i,x=0;
    printf("\n[%d]|[",n->no);
    for(i=0;i<len;i++)
    {
         if(i==attr[x+1])
         {
             x++;
             printf("][");
         }
         printf("%d|",n->arr[i]);
    }
    printf("]\tvalue -> %d",n->val);
}
void mandatory(node* n,int flag)
{
    node *n1;
    int i=0,j=0;
    if(flag){

    display(n);
    printf("\n");
    }
    /*else
    {
        printf("val %d not printed",n->val);
    }*/
    while(n->firstBreak!=i)
    {
        while(j<len&&j<attr[i+1]&&n->arr[j]!=0)
            j++;
        if(j!=len&&j!=attr[i+1])
        {
            n1=createNode(n,i);
            n1->arr[j]=1;
            n1->arr[j-1]=0;
            n1->val-=diff_table[j-1];
            insertNode(n1);
            //display(n1);
        }
        i++;
        j=attr[i];
    }
    if(n->firstBreak!=x)
    {
            if(n->arr[j]==1)
            {
                while(j<len&&j<attr[i+1]&&n->arr[j]!=0)
                    j++;
                if(j!=len&&j!=attr[i+1])
                {
                    n1=createNode(n,i);
                    n1->arr[j]=1;
                    n1->arr[j-1]=0;
                    n1->val-=diff_table[j-1];
                    insertNode(n1);
                    //display(n1);
                }
            }
            while(n->arr[j]==0)
                j++;
            if(((j+1)<len && n->arr[j+1]==0) && ((i+1)==x || (j+1)<attr[i+1] ) )
            {
                n1=createNode(n,i);
                n1->arr[j+1]=1;
                n1->arr[j]=0;
				n1->val-=diff_table[j];
                insertNode(n1);
                //display(n1);
            }
    }
}

void createFirstNodes(int* temp)
{
    int i,attDemo=0,tempVal=0;
    node* n=(node*)malloc(1*sizeof(node));
    n->no=++number;
    n->arr=(int*)malloc(len*sizeof(int));
    for(i=0;i<len;)
    {
        if(i<=temp[attDemo]){
                //printf("\n i=%d te=%d dem=%d",i,temp[attDemo],attDemo);
            n->arr[i]=1;
            tempVal+=array[i].val;
        }
        else
            n->arr[i]=0;
        i++;
        if(i==attr[attDemo+1])
            attDemo++;
    }
    n->val=tempVal;
    n->firstBreak=x;
    heap_arr[heapsize++]=n;

    display(n);
//    printf("\n");
}
void check_sufficiency(int pos,int val)
{
        int i,d;
        int* temp;
        if(pos>=x||val==0)
        {
                if(val==0)
                {
                    temp=(int*)calloc(x,sizeof(int));
                    printf("\n");
                    for(i=0;i<x;i++){
                            if(ch[i]>10)
                                temp[i]=maxAttr[i];
                            else
                                temp[i]=ch[i]+maxAttr[i];
                        //printf("%d::",temp[i]);
                    }
                    createFirstNodes(temp);
                }
            return;
        }
        d=val-freeAttr[pos];

        if(d>0)
            ch[pos]=val-d;
        else
        {
            ch[pos]=val;
            d=0;
        }
        while(d<=val)
        {
            check_sufficiency(pos+1,d);
            d++;
            if(ch[pos]!=0)
                ch[pos]-=1;
        }
}
int main(void)
{
    int flag,i,num,k,d,bgt;

    node*n=generate_initial();
    printf("\n enter total number of members in the team");
    scanf("%d",&num);
    printf("\n\nlen=%d\n",len2);
    check_sufficiency(0,num-len2);

    printf("\n enter budget");
    scanf("%d",&bgt);
    printf("enter the number of combinations that you want to see");
    scanf("%d",&k);

    //enqueue(n);
    //new_mandatory();
    //printf("----------------------------%d-------------------------",heapsize);

    buildmaxheap();
    /*for(i=0;i<3;i++)
    {
        display(heap_arr[i]);
        printf("\n");

    printf("--------------------------top-%d combinations------------",k);

    for(i=0;i<k;)
    {
        flag=0;
        node *n=extractMax();
        if(n->val<=bgt)
        {
            i++;
            printf("\nok");
            flag=1;
            printf("\nok2");
        }
        printf ("\nhello" );
        mandatory(n,flag);
    }
}
