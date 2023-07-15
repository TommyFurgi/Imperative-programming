#include <stdio.h>
#include <math.h>

#define SIZE 40

void read_vector(double x[], int n) {
	for(int i = 0; i < n; ++i) {
		scanf("%lf", x++);
	}
}

void print_vector(double x[], int n) {
	for(int i = 0; i < n; ++i) {
		printf("%.4f ", x[i]);
	}
	printf("\n");
}

void read_mat(double A[][SIZE], int m, int n) {
	for(int i = 0; i < m; ++i) {
		for(int j = 0; j < n; ++j) {
			scanf("%lf", &A[i][j]);
		}
	}
}

void print_mat(double A[][SIZE], int m, int n) {
	for(int i = 0; i < m; ++i) {
		for(int j = 0; j < n; ++j) {
			printf("%.4f ", A[i][j]);
		}
		printf("\n");
	}
}

// 1. Calculate matrix product, AB = A X B
// A[m][p], B[p][n], AB[m][n]
void mat_product(double A[][SIZE], double B[][SIZE], double AB[][SIZE], int m, int p, int n) {
	double val=0;
	for (int i=0;i<n;i++){ // B na gorze n
		for (int j=0;j<m;j++){ // m
			val=0;
			for (int k=0;k<p;k++){
				val+=(A[j][k]*B[k][i]);
			}		
			AB[j][i]=val;
		}
	}
}


// 2. Matrix triangulation and determinant calculation - simplified version
// (no rows' swaps). If A[i][i] == 0, function returns NAN.
// Function may change A matrix elements.
double gauss_simplified(double A[][SIZE], int n) {
	double roz;
	for (int i=0;i<n-1;i++){
		if (A[i][i]==0) return NAN;
		for (int j=i+1;j<n;j++){
			roz = A[j][i] / A[i][i];
			for (int k=i+1;k<n;k++){
				A[j][k]-=(roz*A[i][k]);
			}		
			A[j][i]=0;
		}
	}
	double val=1;
	for (int i=0;i<n;i++){
		val*=A[i][i];
	}
	return val;
}

void backward_substitution_index(double A[][SIZE], const int indices[], double x[], int n) {

}

// 3. Matrix triangulation, determinant calculation, and Ax = b solving - extended version
// (Swap the rows so that the row with the largest, leftmost nonzero entry is on top. While
// swapping the rows use index vector - do not copy entire rows.)
// If max A[i][i] < eps, function returns 0.
// If det != 0 && b != NULL && x != NULL then vector x should contain solution of Ax = b.

void swap_rows(double A[][SIZE], int n, int k, int p) {
    double tmp;

    for (int j = 0; j <n; j++) {
        tmp = A[k][j];
        A[k][j] = A[p][j];
        A[p][j] = tmp;
    }
}

double gauss(double A[][SIZE], const double b[], double x[], const int n, const double eps) {
	double tab[SIZE];
	double roz;
	int zmiana_znak=0;
	for (int i=0;i<n;i++){
		tab[i]=b[i];
	}	

	for (int i=0;i<n-1;i++){
		int najw=i;
		for (int h=i+1;h<n;h++){
			if (fabs(A[h][i])>fabs(A[najw][i])){
				najw=h;
			}
		}
		if ((najw-i)%2==1) zmiana_znak++;

		swap_rows(A,n,i,najw);
		double tmp=tab[i];
		tab[i]=tab[najw];
		tab[najw]=tmp;
		if (fabs(A[i][i])<eps) return 0;

		for (int j=i+1;j<n;j++){
			roz = A[j][i] / A[i][i];
			double v=roz*tab[i];
			tab[j]-=(v);

			for (int k=i+1;k<n;k++){
				A[j][k]-=(roz*A[i][k]);
			}		
			A[j][i]=0;
		}
	}

	double det=1;
	for (int i=0;i<n;i++){
		det*=A[i][i];
	}
	if (zmiana_znak%2==1) det*=-1;

	int dl=0;
	double val;
	double v;
	for (int i=n-1;i>=0;i--){
		val=tab[i];
		for (int j=dl;j>0;j--){
			v=(A[i][i+j]*x[i+j]);
			val-=v;
		}
		x[i]=val/A[i][i];
		dl++;
	}


	return det;
}


// 4. Returns the determinant; B contains the inverse of A (if det(A) != 0)
// If max A[i][i] < eps, function returns 0.
double matrix_inv(double A[][SIZE], double B[][SIZE], int n, double eps) {
	double roz;
	int zmiana_znak=0;
	for (int i=0;i<n;i++){
		for (int j=0;j<n;j++){
			if (i==j) B[i][j]=1.0;
			else B[i][j]=0.0;
		}
	}	

	// for (int i=0;i<n;i++){
	// 	for (int j=0;j<n;j++){
	// 		printf("%f ",B[i][j]);
	// 	}
	// 	printf("\n");
	// }

	for (int i=0;i<n-1;i++){
		int najw=i;
		for (int h=i+1;h<n;h++){
			if (fabs(A[h][i])>fabs(A[najw][i])){
				najw=h;
			}
		}
		// if ((najw-i)%2==1) zmiana_znak++;
		if (najw!=i) zmiana_znak++;

		swap_rows(A,n,i,najw);
		swap_rows(B,n,i,najw);
		if (fabs(A[i][i])<eps) return 0;

		for (int j=i+1;j<n;j++){
			roz = A[j][i] / A[i][i];

			for (int k=0;k<n;k++){
				A[j][k]-=(roz*A[i][k]);
				B[j][k]-=(roz*B[i][k]);
			}		
		}
	// 	for (int i=0;i<n;i++){
	// 		for (int j=0;j<n;j++){
	// 			printf("%f ",B[i][j]);
	// 		}
	// 		printf("\n");
	// 	}
	}

	double det=1;
	for (int i=0;i<n;i++){
		det*=A[i][i];
	}
	if (zmiana_znak%2==1 && det!=0.0) det*=-1;
	


	for (int i=n-1;i>0;i--){
		for (int j=i-1;j>=0;j--){
			roz = A[j][i] / A[i][i];

			A[j][i]-=(roz*A[i][i]);
			for (int k=0;k<n;k++){
				B[j][k]-=(roz*B[i][k]);
			}	
			// B[j][i]-=(roz*B[j][i]);
		}
	}

	double v;
	for (int i=0;i<n;i++){
		v=A[i][i];
		A[i][i]=1;
		for (int j=0;j<n;j++){
			B[i][j]/=v;
		}
		
	}

	// for (int i=0;i<n;i++){
	// 	for (int j=0;j<n;j++){
	// 		if (fabs(B[i][j])<eps) B[i][j]=0.0;
	// 	}
	// }

	return det;
}


int main(void) {

	double A[SIZE][SIZE], B[SIZE][SIZE], C[SIZE][SIZE];
	double b[SIZE], x[SIZE], det, eps = 1.e-13;

	int to_do;
	int m, n, p;

	scanf ("%d", &to_do);

	switch (to_do) {
		case 1:
			scanf("%d %d %d", &m, &p, &n);
			read_mat(A, m, p);
			read_mat(B, p, n);
			mat_product(A, B, C, m, p, n);
			print_mat(C, m, n);
			break;
		case 2:
			scanf("%d", &n);
			read_mat(A, n, n);
			printf("%.4f\n", gauss_simplified(A, n));
			break;
		case 3:
			scanf("%d", &n);
			read_mat(A,n, n);
			read_vector(b, n);
			det = gauss(A, b, x, n, eps);
			printf("%.4f\n", det);
			if (det) print_vector(x, n);
			break;
		case 4:
			scanf("%d", &n);
			read_mat(A, n, n);
			det = matrix_inv(A, B, n, eps);
			printf("%.4f\n", det);
			if (det) print_mat(B, n, n);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}

