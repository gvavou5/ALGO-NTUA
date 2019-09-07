#include<stdio.h>
#include<stdlib.h>
#include <math.h>
#include <limits.h>
#define N 1000000
#define V 10000
#define BSIZE 1<<15

char buffer[BSIZE];
long bpos = 0L, bsize = 0L;

long readLong()
{
    long d = 0L, index1 = 0L;
    char c;
    while (1)  {
        if (bpos >= bsize) {
            bpos = 0;
            if (feof(stdin)) return index1;
            bsize = fread(buffer, 1, BSIZE, stdin);
        }
        c = buffer[bpos++];
        if (c >= '0' && c <= '9') { index1 = index1*10 + (c-'0'); d = 1; }
        else if (d == 1) return index1;
    }
    return -1;
}

struct heapnode {
    int dstcost,index1,index2;
};
typedef struct heapnode heapnode;
typedef heapnode * hptr;

struct field {
    int visited,cost;
};
typedef struct field fieldstr;
typedef fieldstr * grptr;
fieldstr ArrayField[V][V];

struct condpq {
    int dimension;
    hptr heap;
};
typedef struct condpq condpq;
typedef condpq * pptr;

int Array[V][V];

heapnode DeleteHeap(int, hptr);
void InsertHeap(int,heapnode,hptr);
long int DijkstraAlgorithm(pptr,int,int);

int main(int argc, char *argv[]){
    int L,W,flag,i,j;
    scanf("%d %d",&L,&W);
    condpq pntr;
    heapnode varnode;
    
    pntr.heap = (hptr)malloc(sizeof(heapnode)*(N+1));
    do{
        pntr.dimension = 0;
        flag=0;
        i=0;
    }while(0);
    while(i < L){
        j=0;
        while( j < W){
            ArrayField[i][j].visited = 0;
            ArrayField[i][j].cost = readLong();
            if (ArrayField[i][j].cost != 0){
                Array[i][j] = INT_MAX;
            }
            else{
                if(flag == 0){
                    flag++;
                    varnode.index2 = j;
                    varnode.dstcost = 0;
                    varnode.index1 = i;
                    InsertHeap(pntr.dimension,varnode, pntr.heap);
                    ++pntr.dimension;
                }
                Array[i][j] = 0;
            }
            ++j;
        }
        ++i;
    }
    printf("%ld\n",DijkstraAlgorithm(&pntr,L,W));
    return 0;
}

void InsertHeap(int dimension,heapnode nodepar, heapnode* heap) {
    int counter = dimension;
    heapnode temp;
    counter++;
    heap[counter] = nodepar;
    while (heap[counter].dstcost < heap[counter/2].dstcost) {
        if(counter <= 1) continue;
        else{
            temp = heap[counter];
            heap[counter] = heap[counter/2];
            heap[counter/2] = temp;
            counter /= 2;
        }
    }
}

heapnode DeleteHeap(int dimension,heapnode* heap) {
    heapnode returnvalue, temp;
    int help, counter=1;
    returnvalue = heap[1];
    heap[1] = heap[dimension--];
    while(1){
        help = counter*2;
        if (help > dimension) break;
        else if (help < dimension) {
            if (heap[help].dstcost > heap[help+1].dstcost) ++help;
        }
        if (heap[help].dstcost < heap[counter].dstcost) {
            temp = heap[help];
            heap[help] = heap[counter];
            heap[counter] = temp;
            counter = help;
            continue;
        }
        else break;
    }
    return returnvalue;
}



long int DijkstraAlgorithm(condpq *pntr, int L, int W) {
    int i, j, index1, index2;
    heapnode u, varnode;
    while (1) {
        u = DeleteHeap(pntr->dimension,pntr->heap);
        --pntr->dimension;
        index1 = u.index1;
        index2 = u.index2;
        if ((ArrayField[index1][index2].visited != 1) && (ArrayField[index1][index2].cost < 4000)) {
            ArrayField[index1][index2].visited = 1;
            
            i=index1;
            while(i < L){
                if (ArrayField[i][index2].visited == 0) {
                    if (ArrayField[i][index2].cost < 4000 && ArrayField[i][index2].cost == 0) {
                            return Array[index1][index2] + ArrayField[i][index2].cost ;
                    }
                    else if(ArrayField[i][index2].cost < 4000 && ArrayField[i][index2].cost != 0){
                        if (Array[i][index2] > Array[index1][index2] + ArrayField[i][index2].cost){
                            Array[i][index2] = Array[index1][index2] + ArrayField[i][index2].cost;
                            varnode.index2 = index2;
                            varnode.dstcost = Array[i][index2];
                            varnode.index1 = i;
                            InsertHeap(pntr->dimension, varnode, pntr->heap);
                            ++pntr->dimension;
                        }
                    }
                    else break;                 }
                ++i;
            }
            
            i=index1;
            while(i >= 0) {
                if (ArrayField[i][index2].visited == 0) {
                    if (ArrayField[i][index2].cost < 4000 && ArrayField[i][index2].cost == 0) {
                        return Array[index1][index2] + ArrayField[i][index2].cost ;
                    }
                    else if(ArrayField[i][index2].cost < 4000 && ArrayField[i][index2].cost != 0){
                        if (Array[i][index2] > Array[index1][index2] + ArrayField[i][index2].cost){
                            Array[i][index2] = Array[index1][index2] + ArrayField[i][index2].cost;
                            varnode.index2 = index2;
                            varnode.dstcost = Array[i][index2];
                            varnode.index1 = i;
                            InsertHeap(pntr->dimension, varnode, pntr->heap);
                            ++pntr->dimension;
                        }
                    }
                    else break;
                }
                --i;
            }
            
            j=index2;
            while(j < W){
                if (ArrayField[index1][j].visited == 0) {
                    if (ArrayField[index1][j].cost < 4000 && ArrayField[index1][j].cost == 0) {
                            return Array[index1][index2] + ArrayField[index1][j].cost;
                    }
                    else if (ArrayField[index1][j].cost < 4000 && ArrayField[index1][j].cost != 0){
                        if (Array[index1][j] > Array[index1][index2] + ArrayField[index1][j].cost){
                            Array[index1][j] = Array[index1][index2] + ArrayField[index1][j].cost; 
                            varnode.index2 = j;
                            varnode.dstcost = Array[index1][j];
                            varnode.index1 = index1;
                            InsertHeap(pntr->dimension, varnode, pntr->heap);
                            ++pntr->dimension;
                        }
                    }
                    else break;
                }
                ++j;
            }
            
            j=index2;
            while(j >= 0){
                if (ArrayField[index1][j].visited == 0) {
                    if (ArrayField[index1][j].cost < 4000 && ArrayField[index1][j].cost == 0) {
                        return Array[index1][index2] + ArrayField[index1][j].cost;
                    }
                    else if (ArrayField[index1][j].cost < 4000 && ArrayField[index1][j].cost != 0){
                        if (Array[index1][j] > Array[index1][index2] + ArrayField[index1][j].cost){
                            Array[index1][j] = Array[index1][index2] + ArrayField[index1][j].cost;
                            varnode.index2 = j;
                            varnode.dstcost = Array[index1][j];
                            varnode.index1 = index1;
                            InsertHeap(pntr->dimension, varnode, pntr->heap);
                            ++pntr->dimension;
                        }
                    }
                    else break;                }
                --j;
            }
        }
        else continue;
    }
}






