// this program takes random nos as inputs
// to make 2 programs take the same random values you have to seed srand(seed) with same value in both programs
#include<stdio.h>
#include<stdlib.h>
#include <time.h>
int last = 0,maxconfig[1000],r,num[200],last2, comparisons = 0, noChilds = 0, singlet = 0, doublets = 0, heapSize = 0, maxHeapSize, rule1 = 0, rule2 = 0, flag=1;
struct node				//hash table
{
	int jump;
	int config;
	int shifts[200];
	struct node *next;
};
struct heap
{
	int jump,config,shifts[200];
};
void heapsort(int a[]);
void minheapify(int a[],int i);
void buildminheap(int a[]);
struct node *tab[1000];
void topk(int num[],int n,int k);
int extractmax(struct heap h[],int count,int n);
void buildmaxheap(struct heap h[]);
void insert(int child[],int pos,int con);
int duplicate(int child[],int jump);
int same(int a[],int b[]);
void maxheapify(struct heap h[],int i);
int sum(struct heap a);
int left(int i);
int right(int i);
void initheap(struct heap h[]);
int nCr(int n1, int n2);
int fact(int num);
void _init_();

int main(void)
{
	int n,i,k,j;
	FILE *f1;
	double cpu_time_used;
	clock_t start, end;
	srand(time(NULL));
	printf("\n Enter n and r: ");
	scanf("%d %d",&n,&r);

	//calculating k = nCr
	k = nCr(n,r);
	
	f1 = fopen("data.txt", "w");
	for(j=0;j<100;j++){
		_init_();
		for(i=0;i<n;i++)
		{
			num[i]=rand()%100 + 1;
		}
		start = clock();						//time starts
		last2=n-1;
		heapsort(num);
		topk(num,n,k);
		end = clock();
	    	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		/*printf("\n The time taken is %f",cpu_time_used);
		printf("\n Total number of comparisons: %d\n", comparisons);
		printf("\n Max Size of Heap: %d\n", maxHeapSize);
		printf("\n Total No of nodes: %d\n", k);
		printf("\n No of zero child nodes: %d\n", noChilds);
		printf("\n No of one child node: %d\n", singlet);
		printf("\n No of two child nodes: %d\n", doublets);*/
		
		fprintf(f1,"\n ---------------------------------- Data Set #%d--------------------------------\n",j+1);		
		
		fprintf(f1,"\n The sorted nos. are ");
		for(i=0;i<n;i++)
			fprintf(f1," %d ",num[i]);
				
		fprintf(f1,"\n Total number of comparisons: %d\n", comparisons);
		fprintf(f1,"\n Max Size of Heap: %d\n", maxHeapSize);
		fprintf(f1,"\n Total No of nodes: %d\n", k);
		fprintf(f1,"\n No of zero child nodes: %d\n", noChilds);
		fprintf(f1,"\n No of one child node: %d\n", singlet);
		fprintf(f1,"\n No of two child nodes: %d\n", doublets);
		fprintf(f1,"\n --------------------------------------------------------------------------------\n");		
		
	}
	fclose(f1);
	return 0;
}
void _init_(){
	int i=0;	
	last = 0,last2, comparisons = 0, noChilds = 0, singlet = 0, doublets = 0, heapSize = 0, maxHeapSize=0, rule1 = 0, rule2 = 0, flag=1;
	for(i=0;i<1000;i++)
			maxconfig[i]=0;
		
}
void topk(int num[],int n,int k)
{
	struct heap h[1000];
	int count=1;
	initheap(h);
	while(flag)
	{
		if(extractmax(h,count,n)==-1){		//extract max element from heap,displays it,inserts children
		    flag=0;
		}
		if(maxHeapSize < heapSize){
			maxHeapSize=heapSize;
		}
		count++;
	}
		
		
	return;
}
void initheap(struct heap h[])
{
	int i;
	tab[0]=(struct node *)malloc(sizeof(struct node));
	
	h[0].jump=0;
	h[0].config=1;
	for(i=0;i<r;i++)
	{
		h[0].shifts[i]=0;
		tab[0]->shifts[i]=0;
	}
	tab[0]->jump=0;
	tab[0]->config=1;
	maxconfig[0]=1;
	tab[0]->next=NULL;
	heapSize++;
	maxHeapSize = heapSize;
	return;
}
int extractmax(struct heap h[],int count,int n)
{
	struct heap max;
	int child[200],x,i,j,l,shiftSum=0;
	if(last<0)
	{
		printf("\n Heap Underflow");
		return;
	}
	
	/*display the max element at h[0]
	printf("\n %d. %d-%d",count,h[0].jump,h[0].config);
	printf("\n Shifts=");
	
	for(i=0;i<r;i++)
		printf(" %d ",h[0].shifts[i]);
	printf("\n Combination=");
	for(i=0;i<r;i++)
	{
		printf(" %d ",num[i+h[0].shifts[i]]);
	}
	printf("\n Sum=%d \n \n",sum(h[0]));*/
	
	for(i=0;i<r;i++){
		shiftSum += h[0].shifts[i];
	}
	if((shiftSum/r) == r){
	    //printf("hi");
	    return -1;
	}
	
	max=h[0];
	//adjust the heap
	if(last!=0)
	{
		h[0]=h[last];
		
	}
	last--;
	heapSize--;
	maxheapify(h,0);
	
	//generate and insert the children
	for(i=r-1;i>0;i--)
	{
		
		if(max.shifts[i]-max.shifts[i-1] > 0)		
		{
			if(max.shifts[i]-max.shifts[i-1] == 1){
    			//child.jump=max.jump+1;
    			for(j=0;j<r;j++)
    			{
    				if(j==i-1)
    				{
    					child[j]=max.shifts[j]+1;
    				}
    				else
    				{
    					child[j]=max.shifts[j];
    				}
    			}
    			
    			last++;
    			h[last].jump=max.jump+1;
    			h[last].config=maxconfig[max.jump+1]+1;
    			maxconfig[max.jump+1]=maxconfig[max.jump+1]+1;
    			for(l=0;l<r;l++)
    			{
    				h[last].shifts[l]=child[l];
    			}
    			heapSize++;
    			rule2=1;
    			buildmaxheap(h);
    			insert(child,max.jump+1,maxconfig[max.jump+1]);
		    }
		    break;
		}

	}
	if(max.shifts[r-1]+1<=n-r)
	{
		for(j=0;j<r;j++)
		{
			if(j==r-1)
			{
				child[j]=max.shifts[j]+1;
			}
			else
			{
				child[j]=max.shifts[j];
			}
		}
		last++;
		h[last].jump=max.jump+1;
		h[last].config=maxconfig[max.jump+1]+1;
		maxconfig[max.jump+1]=maxconfig[max.jump+1]+1;
		for(l=0;l<r;l++)
		{
			h[last].shifts[l]=child[l];
		}
		heapSize++;
		rule1=1;
		buildmaxheap(h);
		insert(child,max.jump+1,maxconfig[max.jump+1]);
	}
	if(rule1 == rule2){
	    if(rule1==1){
	        doublets++;
	        rule1=0;
	        rule2=0;
	    }
	    else if(rule1==0){
	        noChilds++;
	    }
	}
	else{
	    singlet++;
	    if(rule1==1){
	        rule1=0;
	    }
	    else{
	        rule2=0;
	    }
	}
	
	return 0;
}
void buildmaxheap(struct heap h[])
{
	int i;
	
	if(last%2==1)
		i=last/2;
	else
		i=(last/2)-1;
	for(;i>=0;i--)
		maxheapify(h,i);
}
void insert(int child[],int pos,int con)
{
	struct node *temp,*tmp=tab[pos];
	int i;
	if(con==1)
	{
		tab[pos]=(struct node *)malloc(sizeof(struct node));
		tab[pos]->jump=pos;
		tab[pos]->config=con;
		for(i=0;i<r;i++)
		{
			tab[pos]->shifts[i]=child[i];
		}
		tab[pos]->next=NULL;
	}
	else
	{
		temp=(struct node *)malloc(sizeof(struct node));
		temp->jump=pos;
		temp->config=con;
		for(i=0;i<r;i++)
		{
			temp->shifts[i]=child[i];
		}
		temp->next=NULL;
		while(tmp->next!=NULL)
			tmp=tmp->next;
		tmp->next=temp;
	}
}

void maxheapify(struct heap h[],int i)
{
	struct heap temp;
	int l,r,largest;
	l=left(i);
	r=right(i);
	if(l<=last && sum(h[l])>sum(h[i]))
	{
		comparisons++;
		largest=l;
	}
	else
		largest=i;
	if(r<=last && sum(h[r])>sum(h[largest]))
	{
		comparisons++;
		largest=r;
	}
	if(largest!=i)
	{
		//exchange(h,i,largest);
		temp=h[i];
		h[i]=h[largest];
		h[largest]=temp;
		maxheapify(h,largest);
	}
	return;
}
int sum(struct heap a)
{
	int i,s=0;
	for(i=0;i<r;i++)
		s=s+num[i+a.shifts[i]];
	return s;
}
int left(int i)
{
	return 2*i+1;
}
int right(int i)
{
	return 2*i+2;
}
void minheapify(int a[],int i)
{
	int l,r,smallest,temp;
	l=left(i);
	r=right(i);
	if(l<=last2 && a[l]<a[i])
		smallest=l;
	else
		smallest=i;
	if(r<=last2 && a[r]<a[smallest])
		smallest=r;
	if(smallest!=i)
	{
		temp=a[i];
		a[i]=a[smallest];
		a[smallest]=temp;
		minheapify(a,smallest);
	}
	return;
}

void buildminheap(int a[])
{
	int i;
	if(last2%2==0)
		i=(last2/2)-1;
	else
		i=last2/2;
	for(;i>=0;i--)
		minheapify(a,i);
}
void heapsort(int a[])
{
	int i,temp;
	buildminheap(a);
	for(i=last2;i>=1;i--)
	{
		temp=a[0];
		a[0]=a[i];
		a[i]=temp;
		last2=last2-1;
		minheapify(a,0);
	}
}
int nCr(int x, int y){
    int numr = 1, denom = 1, i = 0, term = 0;
    for(i=x;i>(x-y);i--){
        numr = numr * i;
    }
    denom = fact(y);
    term = (int)(numr/denom);
    return term;
}
int fact(int num){
    int j=1,i=0;
    for(i=num;i>0;i--){
        j = j * i;
    }
    return j;
}


