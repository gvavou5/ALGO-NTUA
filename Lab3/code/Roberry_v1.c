#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#define BSIZE 1<<15


/*
 *
 Read The Input
 *
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
 *
 Definition of structs with the usefull typedefinitions
 *
 */

typedef struct Node{
        long int a,b,c,indx;
}tnode;
typedef tnode * tnodeptr;


typedef struct dfs_struct{
    int edge, vertex;
    struct dfs_struct *next;
}dfs_struct;
typedef dfs_struct * dfs_struct_ptr;
dfs_struct_ptr Graph[50000];


/*
 *
 Global Variables  ( N_max = 50.000 , M_max = 5000.000 )
 *
 */

int Visitor[50000],Parent[50000],Father[50000],DownLimit[50000],Array[50000],MustMatrix[500000],Location[50000][2];
int when;
long int mustval=0;

/*
 *
 DFS Function
 *
 */

void DFS(int s)
{
    int index1,index2;
    Visitor[s]=1;
    Array[s] = DownLimit[s] = ++when;
    
    dfs_struct_ptr dfsptr1;
    dfsptr1=Graph[s];

    while(dfsptr1!=NULL){
        index2= Location[dfsptr1->vertex][0];
        index1 = dfsptr1->edge;
        if(!Visitor[index2]){
            Father[index2] = s;
            DFS(index2);
            if (DownLimit[s] > DownLimit[index2]) DownLimit[s] = DownLimit[index2];
            if ((DownLimit[index2] > Array[s]) && (MustMatrix[index1])) mustval++;
        }
        else if (index2 != Father[s]){
            if (DownLimit[s] <=  DownLimit[index2]) ;
            else DownLimit[s] = DownLimit[index2];
        }
        dfsptr1=dfsptr1->next;
    }
    
}


/*
 *
 Functions for DFS
 *
 */

void insert(int v1,int v2, int Index)
{
    dfs_struct_ptr dfsptr1,dfsptr2,dfsptr3;
    
    dfsptr2=(dfs_struct_ptr)malloc(sizeof(dfs_struct));
    dfsptr3=(dfs_struct_ptr)malloc(sizeof(dfs_struct));
    
    dfsptr2->next = dfsptr3->next=NULL;
    dfsptr3->vertex=v1;
    dfsptr2->vertex = v2;
    dfsptr2->edge = dfsptr3->edge = Index;
    
    if(Graph[Location[v1][0]] != NULL)
    {
        dfsptr1=Graph[Location[v1][0]];
        while(dfsptr1->next != NULL) dfsptr1=dfsptr1->next;
        dfsptr1->next=dfsptr2;
    }
    else{
        Visitor[Location[v1][0]] = 0;
        Graph[Location[v1][0]]=dfsptr2;
    }
  
    
    if(Graph[Location[v2][0]] != NULL)
    {
        dfsptr1=Graph[Location[v2][0]];
        while(dfsptr1->next != NULL) dfsptr1=dfsptr1->next;
        dfsptr1->next=dfsptr3;
    }
    else{
        Visitor[Location[v1][0]] = 0;
        Graph[Location[v2][0]]=dfsptr3;
    }
}


int FunCompareNodes(const void *A, const void *B)
{
    int x = ((const tnodeptr)A)->c;
    int y = ((const tnodeptr)B)->c;
    return (x - y);
}

int MakeCompares(const void *A, const void *B)
{       
        int result, temp;
        long int tmp;
        tnodeptr t1 = (tnodeptr)A;
        tnodeptr t2 = (tnodeptr)B;
        
        if ((t1->a) > (t1->a)){
            tmp = t1->a;
            t1->a = t1->b;
            t1->b = tmp;
        }
    
        if ((t2->a) > (t2->b)){
            tmp = t2->a;
            t2->a = t2->b;
            t2->b = tmp;
        }
        result = t1->a > t2->a ;
        temp   = (t1->a == t2->a)&&(t1->b > t2->b);
        result = result || temp;
        return (result);
}


int Find(int i)
{
    if(i != Parent[i]){
        Parent[i] = Find(Parent[i]);
    }
    return Parent[i];
}


int Union(int i,int j)
{
    if(i!=j){
        Parent[j]=i;
        return 1;
    }
    else return 0;
}


/*
 * 
 MAIN
 *
 */

int main(int argc, char *argv[])
{
    long int N,M,result3;
    scanf("%ld %ld", &N, &M);

    tnodeptr tptr1,tptr2;
    long int i=0, italnever=0;
    long int graph_counter = 1,j;
    int x1,x2,count=0,team[M];
    team[0] = 0;

    tptr1 = (tnodeptr) malloc(M*sizeof(tnode));
    tptr2 = (tnodeptr) malloc(M*sizeof(tnode));
    
    while (i<N){
        Location[i][0] = Location[i][1] = Father[i] = -1;
        Parent[i] = i;
        ++i;
    }
    
    i=0;
    while(i < M){
        tptr1[i].a = readLong(); tptr1[i].b = readLong(); tptr1[i].c = readLong();
        tptr1[i].indx = i;
        MustMatrix[i++] = 1;
    }
    
    qsort(tptr1, M, sizeof(tnode),FunCompareNodes);
    i=1;
    while (i<M){
        if (tptr1[i-1].c != tptr1[i].c) team[graph_counter++] = i++;
        else {
            i++;
            continue;
        }
    }
    
    
    team[graph_counter] = M;
    j=0;
    while (j < graph_counter){
        i = team[j];
        while(i < team[j+1]){
            x1 = Find(tptr1[i].a); x2 = Find(tptr1[i].b);
            if (x1 == x2){
                MustMatrix[tptr1[i].indx] = -1;
                italnever++;
            }
            else{
                tptr2[count].a = Parent[tptr1[i].a];
                tptr2[count].b = Parent[tptr1[i].b];
                tptr2[count].c = tptr1[i].c;
                tptr2[count++].indx = tptr1[i].indx;
            }
            i++;
        }
        
        qsort(tptr2, count, sizeof(tnode),MakeCompares);
        int positon_graph = 0;
        if (count == 0) ;
        else{
            Location[tptr2[0].a][0] = positon_graph++;
            Location[tptr2[0].a][1] = Location[tptr2[0].b][1] = j;
            Location[tptr2[0].b][0] = positon_graph++;
            i = 0;
            insert(tptr2[0].a , tptr2[0].b, tptr2[0].indx);
        }
        
        i=1;
        while(i < count){
            if ((tptr2[i].a != tptr2[i-1].a) || (tptr2[i].b != tptr2[i-1].b)){
                if (Location[tptr2[i].a][1] != j){
                    Location[tptr2[i].a][0] = positon_graph++;
                    Location[tptr2[i].a][1] = j;
                }
                if (Location[tptr2[i].b][1] != j){
                    Location[tptr2[i].b][0] = positon_graph++;
                    Location[tptr2[i].b][1] = j;
                }
                insert(tptr2[i].a , tptr2[i].b, tptr2[i].indx);
            }
            else{
                MustMatrix[tptr2[i].indx] = 0;
                if(MustMatrix[tptr2[i-1].indx] != 1){
                    i++;
                    continue;
                }
                else MustMatrix[tptr2[i-1].indx] = 0;
            }
            i++;
        }
        
        count = 0;
        i  = 0;
        while(i < positon_graph-1){
            if (!Visitor[i]) DFS(i++);
            else {
                i++;
                continue;
            }
        }
        
        i=0;
        while(i < positon_graph){
            free(Graph[i]->next);
            Graph[i] = NULL;
            Visitor[i++] = 0;
            Father[--i] = -1;
            i++;
        }
        
        i = team[j];
        while(i < team[j+1]){
            x1 = Find(tptr1[i].a);
            x2 = Find(tptr1[i++].b);
            x1 = Union(x1,x2);
        }
       
        j++;
    }
    
    result3 = M - mustval - italnever;
    printf("%ld\n%ld\n%ld\n",mustval, italnever, result3);
    
    return 0;
}



