#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

/*
 *
  Read
 *
 */

#define BSIZE 1<<15
char buffer[BSIZE];
long bpos = 0L, bsize = 0L;
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


/*
 *
 Global Parameters ara neccessary for this problem
 *
 */

int N,K,M;
int cntr,Result;
int PlanetSt[10009];

/*
 *
 Definition of the structs
 *
 */

typedef struct Edge
{
    int source,destination,weight;      // source, destination nodes and the weight
    int visitedsrc,visiteddest;         // these variables shows if source and/or destination have been modified
    int costS,costD;                    // cost of source and destination nodes
    int alarm, flag;                    // boolean variables ==> flag tells if a node has been flag before
}Edge;
typedef Edge * Edgeptr;

typedef struct Graph
{
    int V,E;
    Edgeptr edge;
}Graph;
typedef Graph * Graphptr;
Graphptr graph;


typedef struct Subsequence
{
    // this struct is used by BoruvkaAlgorithmMst function
    int parent, rank;
}Subsequence;
typedef Subsequence * subseqptr;

/*
 *
 function GraphEq makes the right changes to the Graph if there is a need
 *
 */


void GraphEq(int source,int destination){
    int i=M;
    while(i < cntr ){
        if ((graph->edge[i].source == source && graph->edge[i].destination == destination) || (graph->edge[i].source == destination && graph->edge[i].destination == source) ){
            graph->edge[i].visitedsrc = graph->edge[i].visiteddest = 1;
            graph->edge[i].weight = 0;
            i++;
            //continue;
        }
        else if (graph->edge[i].visitedsrc == 0 && graph->edge[i].source == source){
            graph->edge[i].weight -= graph->edge[i].costS;
            graph->edge[i].visitedsrc=1;
            i++;
            //continue;
        }
        else if (graph->edge[i].destination == source && graph->edge[i].visiteddest == 0){
            graph->edge[i].weight -= graph->edge[i].costD;
            graph->edge[i].visiteddest=1;
            i++;
            //continue;
        }
        else if (graph->edge[i].visitedsrc == 0 && graph->edge[i].source == destination ){
            graph->edge[i].weight -= graph->edge[i].costS;
            graph->edge[i].visitedsrc=1;
            i++;
            //continue;
        }
        else if (graph->edge[i].destination == destination && graph->edge[i].visiteddest == 0){
            graph->edge[i].weight -= graph->edge[i].costD;
            graph->edge[i].visiteddest=1;
            i++;
            //continue;
        }
        else{
            i++;
            //continue;
        }
    }
}

/*
 *
 function createGraph creates the graph that we want
 *
 */

Graphptr createGraph(int V,int E)
{
	Graphptr graph = (Graphptr)malloc(sizeof(Graph));
	graph->V=V; graph->E=E;
	graph->edge= (Edgeptr)malloc(E*sizeof(Edge));
	return graph;
};

/*
 *
 BoruvkaAlgorithmMst function as also functions Unions and Find found ready at the web
 and i only add a few lines of code at BoruvkaAlgorithmMst function to give always the right result
 *
 */


int Find(struct Subsequence totalsub[], int index)
{
    int result;
    if (totalsub[index].parent == index);
    else  totalsub[index].parent = Find(totalsub, totalsub[index].parent);
    
    result = totalsub[index].parent;
    return result;
}



void Unions(Subsequence totalsub[], int a, int b)
{
    int atree, btree;
    atree = Find(totalsub, a);
    btree = Find(totalsub, b);
    if (totalsub[atree].rank < totalsub[btree].rank) totalsub[atree].parent = btree;
    else{
        if(totalsub[atree].rank > totalsub[btree].rank) totalsub[btree].parent = atree;
        else{
            totalsub[atree].rank++;
            totalsub[btree].parent = atree;
        }
    }
    return;
}


void BoruvkaAlgorithmMst(Graphptr graph){
    // i found this algorithm at web i only add a few lines of code.

    typedef int * intptr;
    int V = graph->V;
    int newindex=0,i=0,alarm=0,treecounter=V;
    
    subseqptr totalsub = (subseqptr)malloc(V*sizeof(Subsequence));
    intptr mincost = (intptr)malloc(V*sizeof(int));
    Edgeptr edge = graph->edge;
    
    int E = graph->E,group1,group2;

    while(newindex < V){
        totalsub[newindex].parent=newindex;
        mincost[newindex++]=-1;
        totalsub[--newindex].rank=0;
        newindex++;
    }
//    printf("FLOP\n");

    while (treecounter > 1)
    {
        i=0;
        printf("%d , %d\n", i, E);
        while(i < E){
            int flag1=0, flag2=0;
            group1=Find(totalsub,edge[i].source);
            group2=Find(totalsub,edge[i].destination);
            if (group1 == group2){
                i++;
                continue;
                printf("45\n");

            }
            else{
                if(mincost[group1] == -1){
                    printf("1111\n");
                    flag1 = 1;
                    mincost[group1]=i;
                }
                printf("1\n");
                
                if(edge[mincost[group1]].weight > edge[i].weight && flag1 == 0) mincost[group1]=i;
                else if(edge[mincost[group1]].weight == edge[i].weight && flag1 == 0) mincost[group1]=i;
                
                printf("2\n");

                if(mincost[group2] == -1 ){
                    printf("here\n");
                    flag2 = 1;
                    mincost[group2]=i;
                }
                else;
                
                if(edge[mincost[group2]].weight > edge[i].weight && flag2 == 0) mincost[group2]=i;
                else{
                    i++;
                    continue;
                }
                printf("3\n");

                i++;
            }
        }
        printf("FLOP2\n");

        
        i=0;
        while(i < V){
            if (mincost[i] != -1){
                group1 = Find(totalsub,edge[mincost[i]].source);
                group2 = Find(totalsub,edge[mincost[i]].destination);
                
                if (group1 == group2){
                    i++;
                    continue;
                }
                else ;
                
                if (edge[mincost[i]].alarm == 0){
                    // den anhkei se akmh stathmou thelmetaforas
                    Result += edge[mincost[i]].weight;
                }
                else if(edge[mincost[i]].flag == 1){
                    // den anhkei se akmh stathmou thelmetaforas
                    Result += edge[mincost[i]].weight;
                }
                else if (edge[mincost[i]].flag == 0){
                    // anhkei se akmh stathmou thelmetaforas
                    Result += edge[mincost[i]].weight;
                    edge[mincost[i]].weight=0;
                    edge[mincost[i]].flag = edge[mincost[i]].visitedsrc =  edge[mincost[i]].visiteddest = 1;
                    GraphEq (edge[mincost[i]].source,edge[mincost[i]].destination);
                }
                else ;
                Unions(totalsub,group1,group2);
                i++;
                treecounter--;
            }
            else{
                i++;
                continue;
            }
        }
        
        newindex = 0;
        alarm=0;
        while(newindex < V){
            mincost[newindex++] = -1;
        }
    }
    return;
}





/*
 *
    Main()
 *
 */

int main(int argc, char *argv[]){
    int i=0,j,PrtArr[10001];
    N=readLong(); K=readLong(); M=readLong();
	graph = createGraph(N,M+K*K+10); // 10 is added to be sure that seg fault will never happen
    cntr=M;
    
    while(i < K){
        PrtArr[i]=readLong();
        PlanetSt[i++]=readLong();
    }
    
    i=0;
    while(i < M){
        // read the edges and initialize
        graph->edge[i].source=readLong();
        graph->edge[i].destination=readLong();
        graph->edge[i].weight=readLong();
        graph->edge[i].alarm = graph->edge[i].flag = graph->edge[i].visitedsrc = graph->edge[i].visiteddest = graph->edge[i].costS = graph->edge[i].costD = 0;
        ++i;
    }
    //printf("Hi\n");
    i=0;
    while(i < K){
        // put the right costs
        j = i+1 ;
        while(j < K){
            graph->edge[cntr].alarm=1;
            graph->edge[cntr].source=PrtArr[i]; graph->edge[cntr].destination=PrtArr[j];
            graph->edge[cntr].costS=PlanetSt[i]; graph->edge[cntr].costD=PlanetSt[j];
            graph->edge[cntr].flag=graph->edge[cntr].visitedsrc = graph->edge[cntr].visiteddest=0;
            graph->edge[cntr].weight=PlanetSt[i]+PlanetSt[j++];
            cntr++;
        }
        ++i;
    }
   // printf("Ho\n");
	// Use BoruvkaMST Algorithm
	BoruvkaAlgorithmMst(graph);
    
    printf("%d\n",Result);
    return 0;
}

