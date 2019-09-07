#include<stdio.h>
#include<stdlib.h>
#define BSIZE 1<<15

struct node{
        long int a;
        long int b;
        long int c;
        long int index;
};
typedef struct node node_t;

typedef struct DFS_node
{
    struct DFS_node *next;
    int vertex;
    int edge_index;
}DFS_node;

DFS_node *G[50000];    //heads of linked list
int visited[50000];
int whereIs[50000][2];
int find(int);
int isNecessary[500000];
int uni(int,int);
long readLong();
long int Necessary=0;
int parent[50000];
char buffer[BSIZE];
long bpos = 0L, bsize = 0L;
int disc[50000];
int time, low[50000];
int father[50000];
void DFS(int i)
{
    int j;
    int ind;
    DFS_node *p;
    //printf("@@@@@@@@@@@@@@@@@@DFS ORDER: %d\n",i);
    p=G[i];
    visited[i]=1;
    time++;
    disc[i] = time;
    low[i] = time;
    while(p!=NULL)
    {
        j= whereIs[p->vertex][0];
        ind = p->edge_index;
        if(!visited[j]){
            father[j] = i;
            DFS(j);
            if (low[i] > low[j]) low[i] = low[j];
            //if (low[j] > disc[i]) Necessary++;
            if ((low[j] > disc[i])&& (isNecessary[ind])) Necessary++;
        }
        else if (j != father[i]){
            if (low[i] > low[j]) low[i] = low[j];
        }
        p=p->next;
    }
}

void insert(int vi,int vj, int index1)
{
    DFS_node *p,*q, *q1;
    q=(DFS_node*)malloc(sizeof(DFS_node));
    q->vertex=vj;
    q->edge_index = index1;
    q->next=NULL;   
    
    if(G[whereIs[vi][0]]==NULL){
        G[whereIs[vi][0]]=q;
        visited[whereIs[vi][0]] = 0;
    }
    else
    {   
        p=G[whereIs[vi][0]];
        while(p->next!=NULL){
            p=p->next;
        }
        p->next=q;
    }
    q1=(DFS_node*)malloc(sizeof(DFS_node));
    q1->vertex=vi;
    q1->edge_index = index1;
    q1->next=NULL;

    if(G[whereIs[vj][0]]==NULL){
        G[whereIs[vj][0]]=q1;
        visited[whereIs[vi][0]] = 0;
    }
    else
    {
        p=G[whereIs[vj][0]];
        while(p->next!=NULL)
            p=p->next;
        p->next=q1;
    }

}



int cmp_node(const void *a, const void *b)
{
        return ((const node_t*)a)->c-((const node_t*)b)->c;
}

int cmp_node2(const void *a, const void *b)
{       
        long int temp;
        node_t *t1 = (node_t*)a;    
        node_t *t2 = (node_t*)b; 
        
        if ((t1->a)>(t1->b)){
            temp = t1->a;
            t1->a = t1->b;
            t1->b = temp;
        }
        if ((t2->a)>(t2->b)){
            temp = t2->a;
            t2->a = t2->b;
            t2->b = temp;   
        }  
        return ((t1->a > t2->a) || ((t1->a == t2->a)&&(t1->b > t2->b)));
}


int main(int argc, char *argv[])
{
    long int N,M,i;
    scanf("%ld %ld", &N, &M);
    node_t *A;
    A = (node_t *) malloc(M*sizeof(node_t));
    
    int group[M];
    long int NeverUsed = 0;

    //for (i=0; i<M; i++) scanf("%ld %ld %ld", &(A[i].a), &(A[i].b), &(A[i].c));
    for (i=0; i<N; i++) {
        whereIs[i][0] = -1;
        whereIs[i][1] = -1;
        father[i] = -1;
        parent[i] = i;
    }

    for (i=0; i<M; i++){
        A[i].a = readLong();
        A[i].b = readLong();
        A[i].c = readLong(); 
        A[i].index = i;
        isNecessary[i] = 1;
    }

    qsort(A, M, sizeof(node_t),cmp_node);
    
    group[0] = 0;
    long int count_g = 1;
    for (i=1; i<M; i++){
        if (A[i].c != A[i-1].c) {
            group[count_g] = i;
            count_g++;
        }    
    }
    group[count_g] = M;

    int u,v,cn=0;
    node_t *B;
    B = (node_t *) malloc(M*sizeof(node_t));
    long int j;
    for (j=0; j<count_g; j++){
        for (i=group[j]; i<group[j+1]; i++){
            u = find(A[i].a);
            v = find(A[i].b);
            if (u!=v){
                //printf("%ld\n",i);
                B[cn].a = parent[A[i].a];
                B[cn].b = parent[A[i].b];
                B[cn].c = A[i].c;
                B[cn].index = A[i].index;
                cn++;
            }
            else{
                isNecessary[A[i].index] = -1; // den tha mpei pote...
                NeverUsed++;
                //printf("Axristi! Akmi %ld %ld %ld || %d %d\n", A[i].a, A[i].b, A[i].c, u, v);
            }
        }
        /* 
        for (i=0; i<cn; i++){
            printf("B: %ld %ld %ld\n" , B[i].a , B[i].b, B[i].c);
        }
        printf("Qsort \n");*/
        qsort(B, cn, sizeof(node_t),cmp_node2);
        /*
        for (i=0; i<cn; i++){
            printf("B: %ld %ld %ld\n" , B[i].a , B[i].b, B[i].c);
        }*/
        //printf("\n\n"); 
        int Gpos = 0;
        if (cn!=0){

        whereIs[B[0].a][0] = Gpos;
        whereIs[B[0].a][1] = j;
        Gpos++;
        whereIs[B[0].b][0] = Gpos;
        whereIs[B[0].b][1] = j;
        Gpos++;
        i = 0;
        insert(B[0].a , B[0].b, B[0].index);
        }
        for (i=1; i<cn; i++){
            //printf("B: %ld %ld %ld\n" , B[i].a , B[i].b, B[i].c);
            if ((B[i].a != B[i-1].a)||(B[i].b != B[i-1].b)){
                if (whereIs[B[i].a][1] != j){
                    whereIs[B[i].a][0] = Gpos;
                    Gpos++;
                    whereIs[B[i].a][1] = j;
                }

                if (whereIs[B[i].b][1] != j){
                    whereIs[B[i].b][0] = Gpos;
                    Gpos++;
                    whereIs[B[i].b][1] = j;
                }
                insert(B[i].a , B[i].b, B[i].index);
            }
            else{
                isNecessary[B[i].index] = 0;
                if (isNecessary[B[i-1].index] == 1){
                    isNecessary[B[i-1].index] = 0;
                }
            }
        }
        cn = 0;
/*
        DFS_node *pp;
          
        for (i=0; i<Gpos; i++){
            //printf("G[%ld ] : %d (%d) ", i,G[i]->vertex, visited[whereIs[G[i]->vertex][0]]);
            pp = G[i];
            while(pp->next!=NULL){
                pp=pp->next;
                //printf("%d (%d)  ", pp->vertex, visited[whereIs[pp->vertex][0]]);
             }
            //printf("\n");
        }
*/
        for(i=0; i<Gpos-1; i++){
            if (!visited[i])
                DFS(i);
        } 
      
        for (i=0; i<Gpos; i++){
            /* ??????????????????? */
            free(G[i]->next);
            G[i] = NULL;
            visited[i] = 0;
            father[i] = -1;
        }
        //printf("Necessary: %d\n", Necessary);
        //Dhmiourgia MST gia to stadio j
        for (i=group[j]; i<group[j+1]; i++){
            u = find(A[i].a);
            v = find(A[i].b);
            u = uni(u,v);
         }
    }
    
    printf("%ld\n%ld\n%ld\n",Necessary, NeverUsed, M-Necessary-NeverUsed);
    return 0;
}

int find(int i){
    if(i != parent[i]) parent[i] = find(parent[i]);
    return parent[i];
}
int uni(int i,int j){
    if(i!=j){
        parent[j]=i;
        return 1;
    }
    return 0;
}

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


