#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    int L,C;
	scanf("%d %d\n", &L, &C);
    int Input[L][L],SumArr[L+1],Temp[L];
    int rr,i,j,r,pp,pivot,k,xx;
	for (i=0; i<L; i++) {
		for (j=0; j<L; j++){
			Input[i][j] = getchar() - '0';
		}
		scanf("\n");
	}
    long long result=0LL;
	for (i=0; i<L; i++) {
		for (j=0; j<L; j++) {
			Temp[j]=0;
		}
		for (j=i; j<L; j++) {
            SumArr[0]=0;
            for (k=1; k<=L; k++) {
				Temp[k-1] += Input[j][k-1];
                SumArr[k] = SumArr[k-1] + Temp[k-1];
            }
            r=0;
            pivot=SumArr[0];
            while (r<=L) {
                rr=1;
                for (pp=r+1; pp<=L; pp++) {
                    if (SumArr[pp] == pivot) rr++;
                    else {
                        xx=pivot+C;
                        if (SumArr[pp] == xx) result += rr;
                        if (SumArr[pp] > xx) break;
                    }
                }
                r+=rr;
                pivot = SumArr[r];
            }		
        }	
    }
    printf("%lld\n", result);
    return 0;
}

