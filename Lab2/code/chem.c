#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#define BSIZE 1<<15

char buffer[BSIZE];
long bpos = 0L, bsize = 0L;

long int N,K;
long int Array[2501][2501];
long int Array2[2501][2501];

long readLong() 
{
	int d = 0, x = 0;
	char c;
	while (1){
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

int CostCompute(int, int);
int MakeCompute(int, int , int , int );
void Solver(int, int, int, int, int );

int main(){
 N=readLong();
 K=readLong();
 int i,j;
 for(i=1;i<=N;i++){
 	for(j=1;j<=N;j++){
 		if(i!=j && j>i){
 			Array[i][j]=readLong(); 		
 		}
 		if(j<i){
 			Array[i][j]=Array[j][i];
 		}
 	}
 }
 for(i = 1; i <= N; i++){
    for( j = 1; j <= N; j++){
			Array[i][j] =Array[i][j]+ Array[i-1][j] + Array[i][j-1] - Array[i-1][j-1];
	}
 }
 for( i = 1; i <= N; i++) Array2[1][i] = CostCompute(1, i);
 for( i = 2; i <= K; i++) Solver(i, 2, N, i-1, N);
 printf("%ld\n",(Array2[K][N]/2));
 return 0;
}

int CostCompute(int Left, int Right)
{
    return (Array[Right][Right] - Array[Right][Left-1] - Array[Left-1][Right] + Array[Left-1][Left-1]);
}

int MakeCompute(int dividor, int pos, int search_left, int search_right)
{
    Array2[dividor][pos] = INT_MAX;
    int ret = search_left,t,i;
    for(i = search_left; i <= search_right; i++)
    {
        t = Array2[dividor-1][i] + CostCompute(i+1, pos);
        if(t < Array2[dividor][pos])
        {
            Array2[dividor][pos] = t;
            ret = i;
        }
    }
    return ret;
}

void Solver(int dividor, int Left, int Right, int search_left, int search_right)
{
    int i,res;
    if(Left > Right)
        return;
    else if(Left == Right)
    {
        MakeCompute(dividor, Left, search_left, search_right);
        return;
    }
    if (search_right>Right-1) search_right=Right-1;
    if(search_left == search_right)
    {
        for( i = Left; i <= Right; i++) MakeCompute(dividor, i, search_left, search_right);
        return;
    }
    res = MakeCompute(dividor, ((Left + Right) / 2), search_left, search_right);
    Solver(dividor, Left, ((Left + Right) / 2)-1, search_left, res);
    Solver(dividor, ((Left + Right) / 2)+1, Right, res, search_right);
}





