#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <inttypes.h>
#define BSIZE 1<<15


/*
 *****   R   *****
 *****   E   *****
 *****   A   *****
 *****   D   *****
 */

char buffer[BSIZE];
long bpos = 0L, bsize = 0L;
long readLong()
{
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
 *****                  *****
 *****    Structs       *****
 *****    Definition    *****
 *****                  *****
 */

typedef struct Edge
{
    long int source, destination;
    int cntwght;
}edge;
typedef edge * edgeptr;

typedef struct vrtx {
    long int arg1val,arg2val;
    struct vrtx * next;
}vrtx;
typedef vrtx * vrtxptr;

typedef struct Subsequence
{
    long int ancestor,varrnk,vrtx;
}subseq;
typedef  subseq * subseqptr;

typedef struct Graph
{
    struct Edge* edge;
}graphos;
typedef graphos * grptr;

typedef int  * intptr;
typedef char * charptr;


/*
 *****                *****
 *****    Global      *****
 *****    Variables   *****
 *****                *****
 */

long int V,E;
intptr dcval,downlimit,ancestor;
charptr marked;
long int res1 = 0,res2 = 0;
int time = 0;
vrtxptr *adj;
grptr Gr;

/*
 *****                *****
 *****  Functions     *****
 *****  Declaration   *****
 *****                *****
 */

void ToolForPath(int,long int);
void CreatePath(long int);
void AddToTheGraph(long int,long int);
void FunSearchForEdges(grptr);
void DoAllTheWork();
void ReadInput();
void Union(subseq *totalsub,long int,long int);
int  FunCompareNodes(const void*,const void*);
long int find(subseqptr,long int);

/*
 *****  M  *****
 *****  A  *****
 *****  I  *****
 *****  N  *****
 */

int main() {
    DoAllTheWork();
    printf("%ld\n%ld\n%ld\n", res1, res2, E - res1 - res2);
    return 0;
}

long int find(subseqptr totalsub, long int i){
    long int res;
    if (totalsub[i].ancestor == i) ;
    else{
        totalsub[i].ancestor = find(totalsub, totalsub[i].ancestor);
    }
    res = totalsub[i].ancestor;
    return res;
}

void Union(subseq *totalsub,long int arg1,long int arg2){
    long int atree, btree;
    atree = find(totalsub, arg1);
    btree = find(totalsub, arg2);
    
    if (totalsub[atree].varrnk < totalsub[btree].varrnk){
        totalsub[btree].vrtx += totalsub[atree].vrtx;
        totalsub[atree].ancestor = btree;
    }
    else if (totalsub[atree].varrnk >= totalsub[btree].varrnk){
        if(totalsub[atree].varrnk >totalsub[btree].varrnk){
            totalsub[btree].ancestor = atree;
            totalsub[atree].vrtx += totalsub[btree].vrtx;
        }
        else{
            totalsub[btree].ancestor = atree;
            totalsub[atree].varrnk++;
            totalsub[atree].vrtx += totalsub[btree].vrtx;
        }
        
    }
}

void DoAllTheWork(){
    ReadInput();
    Gr = (grptr)malloc(sizeof(graphos));
    Gr->edge = (edgeptr)malloc(E*sizeof(edge));
    int i = 0;
    while(i < E ){
        Gr->edge[i].source = readLong();
        Gr->edge[i].destination = readLong();
        Gr->edge[i].cntwght = readLong();
        ++i;
    }
    FunSearchForEdges(Gr);
    return;
}

void ReadInput(){
    V = readLong();
    E = readLong();
    return;
}

int FunCompareNodes(const void* A, const void* B){
    return (((const edgeptr)A)->cntwght) - (((const edgeptr)B)->cntwght);
}


void ToolForPath(int u, long int globalcounter) {
    vrtxptr p = adj[u];;
    marked[u] = 1;
    time++;
    downlimit[u] = dcval[u] = time;
    long int c;
    while (p != NULL) {
        c = p->arg1val;
        if (marked[c] == 0) {
            ancestor[c] = u;
            ToolForPath(c, globalcounter);
            
            if (downlimit[u] > downlimit[c]) downlimit[u] = downlimit [c];
            else;
            
            if (downlimit[c] > dcval[u]){
                if(p->arg2val == 1 ) res1++;
                else ;
            }
            else ;
            
        }
        else if (c != ancestor[u]) {
            if (dcval[c] < downlimit[u]) downlimit[u]  = dcval[c];
            else ;
        }
        p = p->next;
    }
    
}

void CreatePath(long int globalcounter) {
    long int f=globalcounter+1;
    marked = (charptr) malloc(f * sizeof(char) );
    dcval = (intptr) malloc(f * sizeof(int) );
    downlimit = (intptr) malloc( f * sizeof(int) );
    ancestor = (intptr) malloc( f * sizeof(int) );
    int i = 0;
    while(i < globalcounter){
        ancestor[i] = marked[i] = 0;
        i++;
    }
    
    i=1;
    while(i < globalcounter){
        if (marked[i] == 0) {
            if(adj[i] != NULL){
                ToolForPath(i,globalcounter);
                i++;
            }
            else i++;
        }
        else i++;
    }
    return;
}

void AddToTheGraph(long int x, long int u) {
    if (adj[x] != NULL){
        vrtxptr p;
        p = adj[x];
        while(p->next != NULL){
            if(p->arg1val == u) break;
            else p = p->next;
        }
        if (p->arg1val != u){
            vrtxptr newnode;
            newnode=(vrtxptr) malloc( sizeof(vrtx));
            p->next=newnode;
            p = p->next;
            p->next = NULL;
            p->arg2val = 1;
            p->arg1val = u;
        }
        else p->arg2val++;
    }
    else{
        vrtxptr newnode1;
        newnode1=(vrtxptr) malloc( sizeof(vrtx));
        adj[x] = newnode1;
        adj[x]->next = NULL;
        adj[x]->arg2val = 1;
        adj[x]->arg1val = u;
    }
    
}

void FunSearchForEdges(grptr graph) {
    long int x,y,j,globalcounter;
    int weightcur, weightnew;
    edge next_edge ;
    
    qsort(graph->edge, E, sizeof(graph->edge[0]), FunCompareNodes);
    subseqptr totalsub = (subseqptr) malloc( (V+1) * sizeof(subseq) );
    
    long int i=1;
    while( i <= V){
        if(i == 1){
            totalsub[i].ancestor = totalsub[i].vrtx = 1 ;
            totalsub[i].varrnk = 0;
        }
        else{
            totalsub[i].ancestor = i;
            totalsub[i++].varrnk = 0;
            totalsub[--i].vrtx = 1 ;
        }
        i++;
    }
    
    i = 0;
    long int begin = 0;
    while (i < E)
    {
        adj = (vrtxptr*) malloc( (V+1) * sizeof(vrtxptr) );
        next_edge = graph->edge[i];
        x = find(totalsub, next_edge.source);
        y = find(totalsub, next_edge.destination);
        if (x <= y) globalcounter = y;
        else globalcounter = x;
        long int j = 1;
        while(j <= V){
            adj[j++] = NULL;
        }
        weightcur = next_edge.cntwght;
        weightnew = weightcur;
        while (i < E)
        {
            next_edge = graph->edge[i];
            x = find(totalsub, next_edge.source);
            y = find(totalsub, next_edge.destination);
            weightcur = next_edge.cntwght;
            if (weightnew == weightcur);
            else break;
            if (x == y) res2++;
            else{
                AddToTheGraph(x,y);
                AddToTheGraph(y,x);
                if (x > globalcounter) globalcounter = x;
                else;
                if (y > globalcounter) globalcounter = y;
                else;
            }
            i++;
        }
        CreatePath(globalcounter);
        free(adj);
        j = begin;
        while(j < i){
            next_edge = graph->edge[j];
            x = find(totalsub, next_edge.source);
            y = find(totalsub, next_edge.destination);
            if (x == y) ++j;
            else{
                j++;
                Union(totalsub, x, y);
            }
        }
        begin = i;
    }
}




