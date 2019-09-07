#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>

#define BSIZE 1<<15 // for function readLong

char buffer[BSIZE];
long bpos = 0L, bsize = 0L;

// global parameteres for malloc in function main()
long int N,M,K,index1,index2;
long int **Coins;
long int **FirstMove;
long int **SecondMove;
long int argument = 1;

// struct definition
struct Snode{
    int info;
    int row;
    int column;
    struct Snode * next;
    struct Snode * previous;
};
typedef struct Snode * nodeptr;
nodeptr L[1000];// ton orizw me to megisto megethos gt otan kanw to malloc mou petaei warning
                //kai den kserw an o grader adiaforei me gia ta warnings
//nodeptr *L;
nodeptr list_line;

void IntializeAndMalloc(){
    int i,j;
    Coins = (long int **)malloc(N * sizeof(long int *));
    FirstMove = (long int **)malloc(N * sizeof(long int *));
    SecondMove = (long int **)malloc(N * sizeof(long int *));
    // nodeptr malloc
    // L = (nodeptr)malloc(M*sizeof(struct Snode));
    
    if(Coins == NULL || FirstMove == NULL || SecondMove == NULL ){
        fprintf(stderr, "failed to allocate memory.\n");
        //return -1;; // malloc error
    }
    
    for (i=0; i<N; i++){
        Coins[i] = (long int *)malloc(M* sizeof(long int));
        FirstMove[i] = (long int *)malloc(M* sizeof(long int));
        SecondMove[i] = (long int *)malloc(M* sizeof(long int));
        if(Coins[i] == NULL || FirstMove[i] == NULL || SecondMove[i] == NULL) {
            fprintf(stderr, "failed to allocate memory.\n");
            //return -1;; // malloc error
        }
    }
    return;
}

void NodeInsertion(long int num1,long int num2)
{
    nodeptr headptr;
    //nodeptr pointernew;
    if( L[num2] != NULL)
    {
        int flag=Coins[num1][num2]+SecondMove[num1][num2];
        if (L[num2]->info < flag){
            headptr = (nodeptr)malloc(sizeof(struct Snode ));
            headptr->previous=NULL;
            headptr->next=NULL;
            headptr->row=num1;
            headptr->column=num2;
            headptr->info = Coins[num1][num2]+SecondMove[num1][num2];
            //pointernew = headptr;
            L[num2]=headptr;
            return;
        }
        else{
            headptr=L[num2];
            do{
                if( (headptr->row > num1+K) ) {
                    if(headptr->next!=NULL && headptr->previous!=NULL){
                        headptr->next->previous= headptr->previous;
                        headptr->previous->next= headptr->next;
                        //pointernew = headptr;
                        headptr=headptr->previous;
                        if(argument) continue;
                    }
                    else if(headptr->next == NULL && headptr->previous!=NULL){
                        L[num2]=headptr->previous;
                        headptr->previous->next=NULL;
                        headptr=headptr->previous;
                        //pointernew = headptr;
                        if(argument) continue;
                    }
                    else if(headptr->next!=NULL && headptr->previous == NULL){
                        nodeptr newnode;
                        newnode = (nodeptr)malloc(sizeof(struct Snode ));
                        newnode->previous=NULL;
                        newnode->row=num1;
                        newnode->column=num2;
                        //pointernew = headptr;
                        newnode->info = Coins[num1][num2]+SecondMove[num1][num2];
                        newnode->next=headptr->next;
                        headptr->next->previous=newnode;
                        if(argument) return;
                    }
                    else if (headptr->next==NULL && headptr->previous==NULL){
                        nodeptr newnode;
                        newnode = (nodeptr)malloc(sizeof(struct Snode ));
                        newnode->previous=NULL;
                        newnode->next=NULL;
                        newnode->row=num1;
                        newnode->column=num2;
                        //pointernew = headptr;
                        newnode->info = Coins[num1][num2]+SecondMove[num1][num2];
                        L[num2]=newnode;
                        if(argument) return;
                    }
                }
                else if ( headptr->info < flag ){
                    nodeptr newnode;
                    newnode = (nodeptr)malloc(sizeof(struct Snode ));
                    if (headptr->next!=NULL){
                        newnode->next=headptr->next;
                        newnode->next->previous=newnode;
                    }
                    else{
                        newnode->next=NULL;
                        newnode->previous=NULL;
                        L[num2]=newnode;
                    }
                    newnode->row=num1;
                    newnode->column=num2;
                    newnode->previous=NULL;
                    newnode->info = Coins[num1][num2]+SecondMove[num1][num2];
                    if(argument) return;
                }
                else if (headptr->info == flag){
                    nodeptr newnode;
                    newnode = (nodeptr)malloc(sizeof(struct Snode ));
                    newnode->previous=NULL;
                    newnode->info = Coins[num1][num2]+SecondMove[num1][num2];
                    newnode->row=num1;
                    newnode->column=num2;
                    if (headptr->next==NULL && headptr->previous==NULL){
                        headptr->next=newnode;
                        newnode->previous=headptr;
                        newnode->next=NULL;
                        L[num2]=newnode;
                        if(argument) return;
                    }
                    else if (headptr->next!=NULL && headptr->previous==NULL){
                        newnode->previous=headptr;
                        newnode->next=headptr->next;
                        headptr->next->previous=newnode;
                        headptr->next=newnode;
                        if(argument) return;
                    }
                    else if (headptr->next==NULL && headptr->previous!=NULL){
                        newnode->next=NULL;
                        headptr->next=newnode;
                        newnode->previous=headptr;
                        L[num2]=newnode;
                        if(argument) return;
                    }
                    else if (headptr->next!=NULL && headptr->previous!=NULL){
                        newnode->next=headptr->next;
                        newnode->previous=headptr;
                        headptr->next->previous=newnode;
                        headptr->next=newnode;
                        if(argument) return;
                    }
                }
                else if(headptr->previous==NULL){
                    nodeptr newnode;
                    newnode = (nodeptr)malloc(sizeof(struct Snode ));
                    newnode->previous=NULL;
                    newnode->info = Coins[num1][num2]+SecondMove[num1][num2];
                    newnode->row=num1;
                    //pointernew = headptr;
                    newnode->column=num2;
                    newnode->next=headptr;
                    headptr->previous=newnode;
                    if(argument) return;
                }
                else headptr=headptr->previous;
            }while(headptr!=NULL);
        }
    }
    
    else
    {
        headptr = (nodeptr)malloc(sizeof(struct Snode ));
        headptr->previous=NULL;
        headptr->next=NULL;
        headptr->row=num1;
        headptr->column=num2;
        headptr->info = Coins[num1][num2]+SecondMove[num1][num2];
        L[num2]=headptr;
        if(argument) return;
    }
}

// given input read
long readLong() {
    long d = 0L, num1 = 0L;
    char c;
    while (1)  {
        if (bpos >= bsize) {
            bpos = 0;
            if (feof(stdin)) return num1;
            bsize = fread(buffer, 1, BSIZE, stdin);
        }
        c = buffer[bpos++];
        if (c >= '0' && c <= '9') { num1 = num1*10 + (c-'0'); d = 1; }
        else if (d == 1) return num1;
    }
    return -1;
}

void ListInsertion(long int num1,long int num2)
{
    long int flag=Coins[num1][num2]+SecondMove[num1][num2];
    nodeptr headpt, temp=L[num2];
    //nodeptr pointernew;
    
    if(temp==NULL || temp->info != L[num2]->info) return;
    else{
        do{
            if(list_line != NULL){
                headpt=list_line;
                while(headpt!=NULL){
                    if (headpt->column > num2+K){
                        if (headpt->next!=NULL && headpt->previous!=NULL){
                            headpt->previous->next=headpt->next;
                            headpt->next->previous=headpt->previous;
                            headpt=headpt->previous;
                            if(argument) continue;
                        }
                        else if (headpt->next==NULL && headpt->previous!=NULL){
                            headpt->previous->next=NULL;
                            headpt=headpt->previous;
                            //pointernew = headpt;
                            if(argument) continue;
                        }
                        else if (headpt->next!=NULL && headpt->previous==NULL){
                            nodeptr newnode;
                            newnode = (nodeptr)malloc(sizeof(struct Snode ));
                            newnode->previous=NULL;
                            newnode->row=temp->row;
                            newnode->column=temp->column;
                            newnode->info =temp->info;
                            headpt->next->previous=newnode;
                            newnode->next=headpt->next;
                            temp=temp->previous;
                            //pointernew = headpt;
                            if(argument) break;
                        }
                        else if (headpt->next==NULL && headpt->previous==NULL){
                            nodeptr newnode;
                            newnode = (nodeptr)malloc(sizeof(struct Snode ));
                            newnode->next=NULL;
                            newnode->previous=NULL;
                            newnode->row=temp->row;
                            newnode->column=temp->column;
                            newnode->info =temp->info;
                            list_line=newnode;
                            //pointernew = headpt;
                            temp=temp->previous;
                            if(argument) break;
                        }
                        
                        
                    }
                    else if ( headpt->info < temp->info ){
                        nodeptr newnode;
                        newnode = (nodeptr)malloc(sizeof(struct Snode ));
                        newnode->info =temp->info;
                        newnode->row=temp->row;
                        newnode->column=temp->column;
                        if (headpt->next!=NULL && headpt->previous!=NULL){
                            newnode->next=headpt->next;
                            newnode->previous=headpt->previous;
                            headpt->next->previous=newnode;
                            headpt->previous->next=newnode;
                            temp=temp->previous;
                            if(argument) break;
                        }
                        else if (headpt->next==NULL && headpt->previous!=NULL){
                            headpt->next=newnode;
                            newnode->previous=headpt;
                            newnode->next=NULL;
                            list_line=newnode;
                            temp=temp->previous;
                            if(argument) break;
                        }
                        else if (headpt->next!=NULL && headpt->previous==NULL){
                            headpt->next->previous=newnode;
                            newnode->next=headpt->next;
                            newnode->previous=headpt->previous;
                            temp=temp->previous;
                            if(argument) break;
                        }
                        else if (headpt->next ==NULL && headpt->previous==NULL){
                            headpt->next=newnode;
                            newnode->previous=headpt;
                            newnode->next=NULL;
                            list_line=newnode;
                            temp=temp->previous;
                            if(argument) break;
                        }
                    }
                    else if (headpt->info == temp->info){
                        nodeptr newnode;
                        newnode = (nodeptr)malloc(sizeof(struct Snode ));
                        newnode->info =temp->info;
                        newnode->row=temp->row;
                        newnode->column=temp->column;
                        if (headpt->next!=NULL && headpt->previous!=NULL){
                            newnode->next=headpt->next;
                            newnode->previous=headpt;
                            headpt->next->previous=newnode;
                            headpt->next=newnode;
                            temp=temp->previous;
                            if(argument) break;
                        }
                        else if (headpt->next==NULL && headpt->previous!=NULL){
                            headpt->next=newnode;
                            newnode->previous=headpt;
                            newnode->next=NULL;
                            list_line=newnode;
                            temp=temp->previous;
                            if(argument) break;
                        }
                        else if (headpt->next!=NULL && headpt->previous==NULL){
                            headpt->next->previous=newnode;
                            newnode->next=headpt->next;
                            newnode->previous=headpt;
                            headpt->next=newnode;
                            //pointernew = headpt;
                            temp=temp->previous;
                            if(argument) break;
                        }
                        else if (headpt->next ==NULL && headpt->previous==NULL){
                            headpt->next=newnode;
                            newnode->previous=headpt;
                            newnode->next=NULL;
                            list_line=newnode;
                            temp=temp->previous;
                            //pointernew = headpt;
                            if(argument) break;
                            
                        }
                        
                    }
                    if(headpt->previous!=NULL){
                        headpt=headpt->previous;
                    }
                    else{
                        nodeptr newnode;
                        newnode = (nodeptr)malloc(sizeof(struct Snode ));
                        newnode->previous=NULL;
                        newnode->row=temp->row;
                        newnode->column=temp->column;
                        newnode->info =temp->info ;
                        newnode->next=headpt;
                        //pointernew = headpt;
                        headpt->previous=newnode;
                        temp=temp->previous;
                        if(argument) break;

                    }
                }
            }
            else{
                headpt=(nodeptr)malloc (sizeof(struct Snode));
                headpt->previous=NULL;
                headpt->next=NULL;
                headpt->row=temp->row;
                headpt->column=temp->column;
                headpt->info = temp->info;
                list_line=headpt;
                temp=temp->previous;
                if(argument) continue;
            }
        }while(temp!=NULL && temp->info == L[num2]->info);
    }
    return;
}

void FindMax(int num1, int num2)
{
    long int temp1=-1,temp2=-1,indnew,indnew1,tmpnew1=-1, tmpnew2=-1;
    nodeptr headptr;
    headptr = list_line;
    if(headptr == NULL) headptr = L[num2];
    else{
        do{
            if(headptr->row <= num1+K && headptr->column <= num2+K && headptr->row>=num1 && headptr->column>=num2 && tmpnew1==-1){
                temp1=headptr->info;
                indnew=headptr->row;
                indnew1=headptr->column;
                tmpnew1=FirstMove[indnew][indnew1];
                headptr=headptr->previous;
                if(argument) continue;
            }
            else if ((headptr->row <= num1+K && headptr->column <= num2+K && headptr->row>=num1 && headptr->column>=num2 && tmpnew1!=-1)){
                if (headptr->info == temp1 && FirstMove[headptr->row][headptr->column] <= tmpnew1){
                    indnew=headptr->row;
                    indnew1=headptr->column;
                    tmpnew1 = FirstMove[indnew][indnew1];
                    headptr=headptr->previous;
                    if(argument) continue;
                }
                else if ((headptr->info == temp1 && FirstMove[headptr->row][headptr->column] > tmpnew1)){
                    headptr=headptr->previous;
                    if(argument) continue;
                }
                else if (headptr->info < temp1) {     headptr=L[num2]; break;  }
            }
            else headptr=headptr->previous;
        }while(headptr!=NULL);
    }
    headptr=L[num2];
    if (headptr==NULL){
        index1=indnew;
        index2=indnew1;
        return;
    }
    do{
        if (headptr->row <= num1+K && headptr->column <= num2+K && tmpnew2==-1){
            index1=headptr->row;
            index2=headptr->column;
            temp2=headptr->info;
            tmpnew2=FirstMove[index1][index2];
            headptr=headptr->previous;
            if(argument) continue;
        }
        else if (headptr->row <= num1+K && headptr->column <= num2+K && headptr->row>=num1 && headptr->column>=num2 && tmpnew2!=-1){
            if (headptr->info == temp2 && FirstMove[headptr->row][headptr->column]<=tmpnew2){
                index1=headptr->row;
                index2=headptr->column;
                tmpnew2 = FirstMove[index1][index2];
                headptr=headptr->previous;
                if(argument) continue;
            }
            else if (headptr->info == temp1 && FirstMove[headptr->row][headptr->column] > tmpnew2){
                headptr=headptr->previous;
                if(argument) continue;
            }
            else if (headptr->info < temp2) break;
        }
        else{
            headptr=headptr->previous;
        }
    }while(headptr!=NULL);
    
    if(temp1>temp2){
        index1=indnew;
        index2=indnew1;
    }
    else if (temp1==temp2 && tmpnew1 <tmpnew2){
        index1=indnew;
        index2=indnew1;
    }
    return;
}

int main(int argc, char *argv[])
{
    int i,j;
    long int resultF, resultS;
    //input
    N=readLong();
    M=readLong();
    K=readLong();
    IntializeAndMalloc();
    for (i=0; i<N; i++){
        for (j=0; j<M; j++){
            Coins[i][j]=readLong();
            if( i == (N-1) && j == (M-1) ){
                FirstMove[i][j]=0;
                SecondMove[i][j]=0;
            }
        }
    }
    //free(TestArr);
    for(i=N-1;i>=0;i--){
        for(j=M-1;j>=0;j--){
            if ( ( i != (N-1 )) || ( j != (M-1) ) ){
                FindMax(i,j);
                FirstMove[i][j]=Coins[index1][index2]+SecondMove[index1][index2];
                SecondMove[i][j]=FirstMove[index1][index2];
                NodeInsertion(i,j);
                ListInsertion(i,j);
            }
            else if ( ( i == (N-1 )) && ( j == (M-1) ) ){
                NodeInsertion(i,j);
                ListInsertion(i,j);
            }
            else continue;
        }
        if( i < 0 )  return 0;
        list_line=NULL;         // gia na kanw allagh grammhs
    }
    
    resultF=FirstMove[0][0];
    resultS=SecondMove[0][0];
    
    printf("%ld %ld\n",resultF,resultS);
   
    //  free the malloc arrays
    for (i = 0; i < N; i++){
        free(Coins[i]);
        free(FirstMove[i]);
        free(SecondMove[i]);
    }
    
    free(Coins);
    free(FirstMove);
    free(SecondMove);
    
    return 0;
}


