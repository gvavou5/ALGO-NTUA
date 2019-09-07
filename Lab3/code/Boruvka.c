#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

#define BSIZE 1<<15
char buffer[BSIZE];
long bpos = 0L, bsize = 0L;
long readLong();
//we are going to implement Boruvka's algorithm for minimum spanning tree
//we add the stations as routes with cost the sum of the i,j's station's construct cost
void fixGraph(int , int);
//definition of necessary structs
struct Edge
{
	int src,dest,weight;
	int flag;
	int visited;
	int modsrc,moddest,costsrc,costdest;
};

struct Graph
{
	int V,E;
	struct Edge* edge;
};

//a struct to represent a subset for union find
struct subset
{
	int parent;
	int rank;
};

struct Graph* createGraph(int V,int E)
{
	struct Graph* graph = (struct Graph*)malloc(sizeof(struct Graph));
	graph->V=V;
	graph->E=E;
	graph->edge= (struct Edge*)malloc(E*sizeof(struct Edge));
	return graph;
};

//definition of necessary functions
int find(struct subset subsets[],int );
void Union(struct subset subsets[],int ,int );
void boruvkaMST(struct Graph *);
int N,K,M;
int B[10001];
int A[10001][10001];
struct Graph* graph;
int MSTweight;
int tempor=0;
//long int index[10001];
int counter;


int main(int argc, char *argv[]){
	//reading my input
	N=readLong();
	K=readLong();
	M=readLong();
	
	graph = createGraph(N,M+K*K);
	int i,j;
	/*for (i=0;i<K;i++){
		index=readLong();
		B[index]=readLong();
	}*/
	/*for (i=0;i<M;i++){
		index=readLong();
		index2=readLong();
		A[index][index2]=readLong();
		A[index2][index]=A[index][index2]; //a[i][j]=a[j][i] ->symmetric
	}*/
	//i am done with the reading
	int index[10001];
	//read the planet transporters
	for (i=0;i<K;i++){
		index[i]=readLong();
		B[i]=readLong();
	}
	//read the edges
	for(i=0;i<M;i++){
		graph->edge[i].src=readLong();
		graph->edge[i].dest=readLong();
		graph->edge[i].weight=readLong();
		graph->edge[i].flag=0;
		graph->edge[i].visited=0;
		graph->edge[i].modsrc=0;
                graph->edge[i].moddest=0;
                graph->edge[i].costsrc=0;
                graph->edge[i].costdest=0;

	}
	counter=M;
	//create the edges with the necessary costs
	for(i=0;i<K;i++){
		for (j=i+1;j<K;j++){
			graph->edge[counter].src=index[i];
			graph->edge[counter].dest=index[j];
			graph->edge[counter].weight=B[i]+B[j];
			graph->edge[counter].flag=1;
			graph->edge[counter].visited=0;
			graph->edge[counter].modsrc=0;
			graph->edge[counter].moddest=0;
			graph->edge[counter].costsrc=B[i];
			graph->edge[counter].costdest=B[j];
			counter++;
		}
	}
	/*
	graph->edge[M].src=1;
	graph->edge[M].dest=6;
	graph->edge[M].weight=8;
	*/
	

	boruvkaMST(graph);
/*	tempor=MSTweight;
	while (1){
		MSTweight=0;
		boruvkaMST(graph);
		if (MSTweight >= tempor)
			break;
		else
			tempor=MSTweight;
	
	}
*/	printf("%d\n",MSTweight);
	/* reading is correct
	for (i=1;i<N;i++){
		for (j=1;j<N;j++){
			printf("%d ",A[i][j]);
		}
		printf("\n");
	}
	*/
return 0;
}

void boruvkaMST(struct Graph* graph)
{
	int V = graph->V;
	int E = graph->E;
	struct Edge* edge = graph->edge;
	struct subset* subsets = (struct subset*)malloc(V*sizeof(struct subset));//creating our initial forests
	
	int* cheapest = (int*)malloc(V*sizeof(int));
	
	//Create V subsets with single elements at the beggining
	int v;
	int i;
	for (v=0;v<V;++v)
	{
		subsets[v].parent=v;
		subsets[v].rank=0;
		cheapest[v]=-1;
	}
	
	int numTrees=V;
	//int MSTweight=0; //<-this is what we are looking for
	int flag=0;
	while (numTrees > 1)
	{
		for(i=0;i<E;i++)
		{
			int set1=find(subsets,edge[i].src);
			int set2=find(subsets,edge[i].dest);
			if (set1==set2)
			{
				continue;
			}
			else
			{	
				if(cheapest[set1] == -1 || edge[cheapest[set1]].weight >= edge[i].weight)
					cheapest[set1]=i;
				
				if(cheapest[set2] == -1 || edge[cheapest[set2]].weight > edge[i].weight)
					cheapest[set2]=i;
			}
		}
		
		for(i=0;i<V;i++)
		{
			if (cheapest[i]!=-1)
			{
				int set1=find(subsets,edge[cheapest[i]].src);
				int set2=find(subsets,edge[cheapest[i]].dest);
				
				if (set1==set2)
				{
					continue;
				}


				if (edge[cheapest[i]].flag == 0){ //an h akmh anhkei se aplo dromo metaforas
					MSTweight += edge[cheapest[i]].weight;
				}
				else if (edge[cheapest[i]].visited == 0){ // an anhkei se akmh stathmou thlemetaforas ftiakse ksana ta varh tou grafoy
					MSTweight += edge[cheapest[i]].weight;
					edge[cheapest[i]].visited=1;
					edge[cheapest[i]].weight=0;
					edge[cheapest[i]].modsrc=1;
					edge[cheapest[i]].moddest=1;
					fixGraph (edge[cheapest[i]].src,edge[cheapest[i]].dest);
				}
				else if (edge[cheapest[i]].visited ==1){ //an thn exeis episkeftei hdh de 8elei ftiaksimo
					MSTweight += edge[cheapest[i]].weight;
				}


				//if (flag==1){
				//	graph->edge[cheapest[i]].weight=INT_MAX; //wste na mh lhf8ei ypopshn h amkh ayth sthn epomenh klhsh gia epilogh sthn isothta
				//}
				Union(subsets,set1,set2);
				numTrees--;
			}
		}
		for (v=0;v<V;++v)
		{
		cheapest[v]=-1;
		}
		flag=0;
	}
	
	//printf("%d\n",MSTweight);
	return;
}

void fixGraph(int src,int dest){

	int i;
	for (i=M;i<counter ;i++){
		if ((graph->edge[i].src == src && graph->edge[i].dest==dest) || (graph->edge[i].src == dest && graph->edge[i].dest == src) ){
			graph->edge[i].modsrc=1;
			graph->edge[i].moddest=1;
			graph->edge[i].weight=0;
		}
		else if (graph->edge[i].src == src && graph->edge[i].modsrc==0 ){
			graph->edge[i].weight-=graph->edge[i].costsrc;//graph->edge[i].costdest
			graph->edge[i].modsrc=1;
		}
		else if (graph->edge[i].dest == src && graph->edge[i].moddest==0){
			graph->edge[i].weight-=graph->edge[i].costdest;
			graph->edge[i].moddest=1;
		}
		else if (graph->edge[i].src == dest && graph->edge[i].modsrc==0){
			graph->edge[i].weight-=graph->edge[i].costsrc;
			graph->edge[i].modsrc=1;
		}
		else if (graph->edge[i].dest == dest && graph->edge[i].moddest==0){
                        graph->edge[i].weight-=graph->edge[i].costdest;
                        graph->edge[i].moddest=1;
                }
	}

}



void Union(struct subset subsets[], int x, int y)
{
    int xroot = find(subsets, x);
    int yroot = find(subsets, y);
 
    // Attach smaller rank tree under root of high
    // rank tree (Union by Rank)
    if (subsets[xroot].rank < subsets[yroot].rank)
        subsets[xroot].parent = yroot;
    else if (subsets[xroot].rank > subsets[yroot].rank)
        subsets[yroot].parent = xroot;
 
    // If ranks are same, then make one as root and
    // increment its rank by one
    else
    {
        subsets[yroot].parent = xroot;
        subsets[xroot].rank++;
    }
}

int find(struct subset subsets[], int i)
{
    // find root and make root as parent of i
    // (path compression)
    if (subsets[i].parent != i)
      subsets[i].parent =find(subsets, subsets[i].parent);
 
    return subsets[i].parent;
}


long readLong() {
        long d = 0L, x = 0L;
        char c;
        while (1)  {
                if (bpos >= bsize) {
                        bpos = 0;
                        if (feof(stdin)) return x;
                        bsize = fread(buffer, 1, BSIZE, stdin);
                }
                c = buffer[bpos++];
                if (c >= '0' && c <= '9') { x = x*10 + (c-'0'); d = 1; }
                else if (d == 1) return x;
        }
        return -1;
}	
