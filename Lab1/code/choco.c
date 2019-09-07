#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct node {
    int Si;
    int Fi;
}*nodeptr;

int fcheck (struct node * beg, long long choco,int size, int lst) {
    int i = 0;
    long long total = 0LL;
    while (i < size) {
        if (lst < (beg[i].Si)) break;
        total += (long long) (lst - (beg[i].Si) + 1);
        if (lst > (beg[i].Fi)) total -= (long long) (lst - (beg[i].Fi));
        i++;
    }
    if (total < choco) return 0;
    else return 1;
}

int mcompare (const void * x, const void * y) {
    int r1,r2;
    nodeptr tmp1 = (struct node *) x;
    nodeptr tmp2 = (struct node *) y;
    r1=tmp1->Si;
    r2=tmp2->Si;
    return (r1-r2);
}

int main () {
	int N,i,min,midean;
    long long K;
    int max=0, time=0, flag=0;
	scanf("%d %lld", &N ,&K);
	nodeptr alarm = (nodeptr)malloc(N*sizeof(struct node))	;
	if (alarm == NULL) {
		fprintf(stderr, "File not found!");
		exit(1);
	}
	for (i=0; i<N; i++) {
		scanf("%d %d", &(alarm[i].Si),&(alarm[i].Fi));
		if (!(max >= alarm[i].Fi)) max = alarm[i].Fi;//alarm[i].F > max
	}
	qsort(alarm, N, sizeof(struct node), mcompare);
	min = alarm[0].Si;
	while (max > min) {
		if ((max - min) <= 2) {
            if (fcheck(alarm,K, N, min)) {
                time = min;
                flag=1;
            }
            else {
                flag=1;
                time = max;
            }
		}
        midean = (min+max)/2;
		if (fcheck(alarm,K, N, midean))
			if (!fcheck(alarm,K, N, midean-1)) {
                flag=1;
                time = midean;
			}
			else max = midean;
		else min = midean;
		if (flag == 1) break;
	}
	printf("%d\n", time);
	return 0;
}
