/*************************************************General Logic*********************************************************
/                                                                                                                      /
/                                       Author: Abhimanyu Banerjee                                                     /
/                                                                                                                      /
/Trivial cases: Print the zero-eth node outside of sorting nodes function                                              /
/                                                                                                                      /
/Data Storage needed: 1) Array of nodes to represent each level(rank) [until rank is determined each such array        /
/                        element will hold all the children of the previous level/rank].                               /
/                     2) Integer array to store the number of nodes at each level/rank at any instant                  /
/                                                                                                                      /
/Algorithm: Step 1: Start from either end of the list of children and compare two of the nodes in order[compareChilds] /
/                   The loser gets pushed to the winner's lower level along with any of its own children and at the    /
/                   same time generate its child and add it to the level below its current level.Repeat until the      / 
/                   noOfChilds[currLevel] == 1, ie there is only one node on the current level.                        /
/           Step 2: If the node whose rank was determined in the previous level has any children left to be generated  /
/                   ->generate them. Otherwise, increase the level/rank variable and go to the next level(go to step 1)/
/                                                                                                                      /
/Observation: 1)Greater the standard deviation, fewer comparisons(not sure ...need to plot)
/**********************************************************************************************************************/



#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include <math.h>
int last=0,maxconfig[1000],r,num[200],last2, currLevel=0, n, bigNum=10000, nodes=0, comparisons=0, connect=0;
//creating the noOfChilds matrix
int noOfChilds[10000];
struct node//a node structure
{
	int jump;
	int config;
	int level;
	int shifts[200];
	int shift_diff[200];
	int full;
	int pid;
	int id;
	struct node *next;
};

struct heap
{
	int jump,config,shifts[200],shift_diff[200];
};

void heapsort(int a[]);
void minheapify(int a[],int i);
void buildminheap(int a[]);

struct node **matr;
void sortNodes(int limit);
int generateChild(struct node *a,int level);
void compareChilds(int currLevel);
int sum(struct node *a);
int left(int i);
int right(int i);
void iniRanker();
float sd(int n);
void bubbleDown(int pid, int level);
int nCr(int n1, int n2);
int factorial(int num);

int main(void){
	int i=0,k=0,j=0,term=0;
	double cpu_time_used;
	clock_t start, end;
	srand(time(NULL));
	printf("\n Enter n and r: ");
	scanf("%d %d",&n,&r);
	
	//calculating k = nCr
	k = nCr(n,r);
	
	//init the noOfChilds matrix
	for(i=0;i<bigNum;i++){
	    if(i>=k){
	        noOfChilds[i]=-1;
	    }
	    else{
	        noOfChilds[i]=0;
	    }
	}
	
	//creating the rank matrix
	matr = (struct node**)malloc(k*sizeof(struct node*));
	for(i=0;i<k;i++){
	    matr[i] = NULL;
	}
	
	//filling the number array
	for(i=0;i<n;i++)
	{
		num[i]=rand()%100;
	}
	start = clock();						//time starts
	last2=n-1;
	heapsort(num);
	for(i=0;i<1000;i++)
		maxconfig[i]=0;
		
	printf("\n Node %d: 0 - 0",++nodes);
	printf("\n Shifts= 0 0 0");
	printf("\n Combination=");
	term = 0;
	for(i=0;i<r;i++){
		printf(" %d ",num[i]);
		term += num[i];
	}
	printf("\n Sum=%d \n \n",term);
	
	//nCrSorting
	iniRanker();
	sortNodes(k-1);
	
	end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf("\n The time taken is %f\n",cpu_time_used);
	printf("\n The sorted nos. are ");
	for(i=0;i<n;i++){
		printf(" %d ",num[i]);
	}
	printf("\n Standard deviation of the numbers is: %f ", sd(n));
	printf("\nTotal number of comparisons needed: %d\n",comparisons);
	return 0;
}
float sd(int n){
    //calculate standard deviation of the numbers in num array
    int  i;
    float average, variance, std_deviation, sum = 0, sum1 = 0;
    for (i = 0; i < n; i++)
    {
        sum = sum + num[i];
    }
    average = sum / (float)n;
    /*  Compute  variance  and standard deviation  */
    for (i = 0; i < n; i++)
    {
        sum1 = sum1 + pow((num[i] - average), 2);
    }
    variance = sum1 / (float)n;
    std_deviation = sqrt(variance);
    return std_deviation;
}
void iniRanker(){
	int i;
	
	//creating the level-1 node
	struct node *newNode;
	newNode = (struct node*)malloc(sizeof(struct node));
	newNode->jump=1;
	newNode->config=1;
	for(i=0;i<r;i++)
	{
		newNode->shifts[i]=0;
		if(i==r-2){
		    newNode->shift_diff[i] = 1;
		}
		else{
		    newNode->shift_diff[i] = 0;
		}
	}
	newNode->shifts[r-1]=1;
	newNode->level=0;
	maxconfig[0]=1;
	
	//means no child has been generated yet, becomes 1 when all children have been generated
	newNode->full=-1; 
    
    //generating parentId(trivial case). All other times, parentId is copied from nodeId of parent
    newNode->pid = connect;    
    
    //generating nodeId	
    newNode->id = ++connect;
	
	//storing the level-1 node in the rank matrix
	matr[0] = newNode;
	
	//initialising the noOfChilds array accordingly
	noOfChilds[0]++;
}
void sortNodes(int limit){
	int noOfChild = 1, sum1=0,sum2=0,i=0,k=0,j=0;
	while(currLevel < limit){
		//check if current level has only one Node
		if(noOfChilds[currLevel]==1){
		    /*print the current ranked node
    		printf("\n Node %d: %d - %d",++nodes,matr[currLevel]->jump, matr[currLevel]->config);
    		printf("\n Shifts=");
    		for(i=0;i<r;i++){
    			printf(" %d ",matr[currLevel]->shifts[i]);
    		}
    		printf("\n Combination=");
    		for(i=0;i<r;i++){
    			printf(" %d ",num[i+matr[currLevel]->shifts[i]]);
    		}
    		printf("\n Sum=%d \n \n",sum(matr[currLevel]));
    		//break;*/
		    
		    //generate all possible children
		    if(matr[currLevel]->full!=1){
		        generateChild(matr[currLevel], matr[currLevel]->level);
		        matr[currLevel]->full=1; 
		    }
		    currLevel++;
		    
		}
		else{
		    //compare first two nodes in current level
		    compareChilds(currLevel);
		}
	}
}
void compareChilds(int num){
    //the smaller sum is the winner
    int sum1=0, sum2=0;
    struct node *ptr=NULL, *postptr=NULL, *ptr1=NULL;
    ptr = matr[num];
    postptr = ptr->next;
    ptr1=matr[num+1];
    while(ptr1!=NULL && ptr1->next!=NULL){
        ptr1=ptr1->next;
    }
    sum1=sum(ptr);
    sum2=sum(postptr);
    if(sum1 <= sum2){
        //ptr is loser => demoted
        if(ptr1==NULL){
            matr[num+1] = ptr;
        }
        else{
            ptr1->next = ptr;
        }
        ptr->next = NULL;
        ptr->level++;
        matr[num]=postptr;
        //reduce no of nodes on current level
        noOfChilds[num]--;
        noOfChilds[num+1]++;
        
        //increase the comparisons counter
        comparisons++;
        
        //demote the losing subtree by bubbling down
        bubbleDown(ptr->id, ptr->level);
    }
    else{
        //postptr is loser =>demoted
        ptr->next = postptr->next;
        if(ptr1==NULL){
            matr[num+1] = postptr;
        }
        else{
            ptr1->next = postptr;
        }
        postptr->next = NULL;
        postptr->level ++;
        //reduce no of nodes on current level
        noOfChilds[num]--;
        noOfChilds[num+1]++;
        
        //increase the comparisons counter
        comparisons++;
        
        //demote the losing subtree by bubbling down
        bubbleDown(postptr->id, postptr->level);
    }
}
void bubbleDown(int pid, int level){
    //to recursively push down the child sub-tree due to new status(rank/level) of sub-tree's root
    struct node *ptr = NULL, *ptr1 = NULL;
    ptr = matr[level];
    while(ptr!=NULL && ptr->next!=NULL){
        if(ptr->pid == pid){
            //child found => demote to next level
            ptr1 = matr[level+1];
            while(ptr1!=NULL && ptr1->next!=NULL){ //a loop to get to the end of the list(if any) on that level
                ptr1=ptr1->next;
            }
            if(ptr1==NULL){
                matr[level+1]=ptr;
            }
            else{
                ptr1->next = ptr;
            }
            //increase no of nodes on new level
            noOfChilds[level+1]++;
            
            ptr->level = level + 1;
            bubbleDown(ptr->id, ptr->level);
        }
        ptr=ptr->next;
    }
}
int generateChild(struct node *a,int level){
	int i =0,pos=0,j=0;
	struct node *ptr=NULL;
	//generate and insert the children
	for(i=r-2;i>=0;i--){
		if(a->shift_diff[i] > 0){
        	//found the start of the rightmost block of 1's
        	if(a->shift_diff[i]==1){
        	    //rule 2:If the right most block of 1 has a single 0 to its left, then move the 1 immediately to the left of that 0 to the right by one position.
                struct node *child2;
        		child2 = (struct node*)malloc(sizeof(struct node));
        		for(j=0;j<r;j++){
        			child2->shifts[j]=a->shifts[j];
        			if(j<r-1){
        			    child2->shift_diff[j]=a->shift_diff[j];
        			}
        		}					
        		child2->jump=a->jump+1;
        		child2->config=maxconfig[a->jump+1]+1;
        		maxconfig[a->jump+1]=maxconfig[a->jump+1]+1;	
        		child2->shifts[i]++;
        		child2->shift_diff[i]--;
        		if((i-1)>=0){
        			child2->shift_diff[i-1]++;
        		}
        		child2->level = level+1;
        		child2->pid = matr[level]->id;
        		child2->id = ++connect;
                ptr=matr[level+1];
                if(ptr==NULL){
                    matr[level+1]=child2;
                }
                else{
                    while(ptr->next!=NULL){
        			    ptr=ptr->next;
        			}
        			ptr->next=child2;
                }
                noOfChilds[child2->level]++;
            }
            break;
		}
	    
	}
	if(a->shifts[r-1]<(n-r)){
	    //rule 1:Move the right most 1 by 1 position if space is available to the right
	    struct node *child1;
		child1 = (struct node *)malloc(sizeof(struct node));
		for(j=0;j<r;j++){
			if(j<r-1){
				child1->shift_diff[j]=a->shift_diff[j];
			}
			child1->shifts[j]=a->shifts[j];
		}
		child1->jump=a->jump+1;
		child1->config=maxconfig[a->jump+1]+1;
		maxconfig[a->jump+1]=maxconfig[a->jump+1]+1;	
		child1->shifts[r-1]++;
		child1->shift_diff[r-2]++;			
		child1->level = level+1;
		child1->pid = matr[level]->id;
		child1->id = rand()%bigNum + 1;
		ptr=matr[level+1];
		if(ptr==NULL){
            matr[level+1]=child1;
        }
		else{
            while(ptr->next!=NULL){
			    ptr=ptr->next;
			}
			ptr->next=child1;
        }
        noOfChilds[child1->level]++;
	}
}
int sum(struct node *a){
	int i,s=0;
	for(i=0;i<r;i++)
		s=s+num[i+a->shifts[i]];
	return s;
}

int left(int i){
	return 2*i+1;
}
int right(int i){
	return 2*i+2;
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
void minheapify(int a[],int i){
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

void buildminheap(int a[]){
	int i;
	if(last2%2==0)
		i=(last2/2)-1;
	else
		i=last2/2;
	for(;i>=0;i--)
		minheapify(a,i);
}
void heapsort(int a[]){
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