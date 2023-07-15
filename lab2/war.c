#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define N 52

#define CBUFF_SIZE 52

int cbuffA[CBUFF_SIZE];
int cbuffB[CBUFF_SIZE];
int outA = 0, lenA = 0;
int outB = 0, lenB = 0;


void A_won(int n) {
    int tmp;
    for (int i=0;i<n;i++){
        tmp=cbuffA[outA];
        cbuffA[(outA+lenA)%52]=tmp;
        outA=(outA+1)%52;
    }
    for (int i=0;i<n;i++){
        tmp=cbuffB[outB];
        cbuffA[(outA+lenA)%52]=tmp;
        lenA++;
        lenB--;
        outB=(outB+1)%52;
    }
}

void B_won(int n) {
    int tmp;
    for (int i=0;i<n;i++){
        tmp=cbuffB[outB];
        cbuffB[(outB+lenB)%52]=tmp;
        outB=(outB+1)%52;
    }
    for (int i=0;i<n;i++){
        tmp=cbuffA[outA];
        cbuffB[(outB+lenB)%52]=tmp;
        lenB++;
        lenA--;
        outA=(outA+1)%52;
    }
}

void cbuff_printB(void) {
	for (int i=0;i<lenB;i++){
		printf("%d ", cbuffB[(outB+i)%52]);
	}
}

void cbuff_printA(void) {
	for (int i=0;i<lenA;i++){
		printf("%d ", cbuffA[(outA+i)%52]);
	}
}

void easier_vesrion_draw(void){
    cbuffA[(outA+lenA)%52]=cbuffA[outA];
    cbuffB[(outB+lenB)%52]=cbuffB[outB];
    outA=(outA+1)%52;
    outB=(outB+1)%52;
}


int rand_from_interval(int a, int b) {	
	if(a>b){
		return INT_MIN;
	}else if(b-a>RAND_MAX){
		return INT_MAX;
	}else if (a==b){
		return a;
	}else{
		return a + (rand() % (b - a + 1));
	}
}

void swap (int array[], int i, int k) {
	int tmp=array[i];
	array[i]=array[k];
	array[k]=tmp;
}

// random permutation of integers from [0, n-1]
void rand_permutation(int n, int array[]) {
	for(int i=0;i<n;i++){
		array[i]=i;
	}
	for(int i=0;i<n-1;i++){
		int k=rand_from_interval(i,n-1);
		swap(array,i,k);
	}
}

void rozdziel_karty (int n,int array[]) {
    for(int i=0;i<(n/2);i++){
        cbuffA[lenA]=array[i];
        lenA++;
    }
    for(int i=(n/2);i<n;i++){
        cbuffB[lenB]=array[i];
        lenB++;
    }
}


int main(void){
    int seed, game_vesion, game_len;
    scanf("%d %d %d", &seed, &game_vesion, &game_len);
    srand((unsigned) seed); // set the seed

    int array[N];

    rand_permutation(N,array);
    rozdziel_karty(N,array);


    int indexA=outA,indexB=outB,queue=1;
    for(int i=0;i<game_len;i++){
        if ((cbuffA[indexA]/4)>(cbuffB[indexB]/4)){
            A_won(queue);
            queue=1;
            indexA=outA;
            indexB=outB;
        } else if((cbuffA[indexA]/4)<(cbuffB[indexB]/4)){
            B_won(queue);
            queue=1;
            indexA=outA;
            indexB=outB;
        } else {
            if (game_vesion==1){
                easier_vesrion_draw();
                indexA=outA;
                indexB=outB;
            } else{
                indexA=((indexA+2)%52);
                indexB=((indexB+2)%52);
                queue+=2;
                if ((queue>lenA) || (queue>lenB)){

                    printf("1 %d %d", lenA, lenB);
                    return 0;
                }
            }
        }

        if (lenB==0){
            printf("2 %d", i+1);
            return 0;
        }
        if (lenA==0){
            printf("3 \n");
            cbuff_printB();
            return 0;
        }
    }
    printf("0 %d %d", lenA, lenB);


    return 0;
}