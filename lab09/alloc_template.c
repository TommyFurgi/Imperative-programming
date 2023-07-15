#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#define TAB_SIZE  1000
#define BUF_SIZE  1000

int get(int cols, int row, int col, const int *A) {
	return A[cols*row+col];
}

void set(int cols, int row, int col, int *A, int value) {
	A[cols*row+col]=value;
}

void prod_mat(int rowsA, int colsA, int colsB, int *A, int *B, int *AB){
	int tmp;
	for (int t=0;t<rowsA;t++){
		for (int i=0;i<colsB;i++){
			tmp=0;
			for (int j=0;j<colsA;j++){ //colsA==rowsB
				tmp+=get(colsB,j,i,B)*get(colsA,t,j,A);
			}
			set(colsB,t,i,AB,tmp);
		}
	}
}

void read_mat(int rows, int cols, int *t) {
	int k;
	for (int j=0;j<rows;j++){
		for (int i=0;i<cols;i++){
			scanf("%d",&k);
			set(cols,j,i,t,k);
		}
	}
}

void print_mat(int rows, int cols, int *t) {
	for (int j=0;j<rows;j++){
		for (int i=0;i<cols;i++){
			printf("%d ",get(cols,j,i,t));
		}
		printf("\n");
	}
}

int read_char_lines(char *array[], int n) {
	int cnt=0;
	char buffer[BUF_SIZE];
	char result[BUF_SIZE];
	while (fgets(buffer, BUF_SIZE, stdin) != NULL) {
		if (buffer[0] == '\0') break;
		if (cnt==n){
			int i=0;
			while (buffer[i]!='\n'){
				result[i]=buffer[i];
				i+=1;
			}
			result[i]='\n';
				
		}	
			
		cnt+=1;
	
	}
	int i=0;
	while (result[i]!='\n'){
		printf("%c",result[i]);
		i+=1;
	}
	return cnt;
}

void write_char_line(char *array[], int n) {
}

void delete_lines(char *array[]) {
}

int read_int_lines_cont(int *ptr_array[],int n) {
	int cnt=0;
	char buffer[BUF_SIZE];
	char result[BUF_SIZE];
	while (fgets(buffer, BUF_SIZE, stdin) != NULL) {
		if (buffer[0] == '\0') break;
		if (cnt==n){
			int i=0;
			while (buffer[i]!='\n'){
				result[i]=buffer[i];
				i+=1;
			}
			result[i]='\n';
		}
		
	cnt+=1;
	
	}
	int i=0;
	while (result[i]!='\n'){
		printf("%c",result[i]);
		i+=1;
	}
	return cnt;
}

void write_int_line_cont(int *ptr_array[], int n) {	
	// int tab=ptr_array[n];
	// int i=0;
	// while (ptr_array[n][i]!=NULL){
	// 	printf("%d ",ptr_array[n][i]);
	// 	i+=1;
	// }
}

typedef struct {
	int *values;
	int len;
	double average;
} line_type;

int read_int_lines(line_type* lines_array[]) {
	int lines = 0;
    int tab[TAB_SIZE];
    int cnt, suma;
    char line[BUF_SIZE];
    char* word;
    char space[] = " ";
    
    while (fgets(line, BUF_SIZE, stdin) != NULL) {
        cnt = suma = 0;
        word = strtok(line, space);
        while (word != NULL) {
            tab[cnt++] = atoi(word);
            suma += tab[cnt - 1];
            word = strtok(NULL, space);
        }
        lines_array[lines] = (line_type*)malloc(sizeof(line_type)); // Allocate memory for the struct
        

        
        lines_array[lines]->values = (int*)malloc(sizeof(int) * cnt); // Allocate memory for the values array
        

        lines_array[lines]->len = cnt;
        lines_array[lines]->average = suma / cnt;
        for (int i = 0; i < cnt; i++) {
            *(lines_array[lines]->values + i) = tab[i];
        }
        lines++;
    }
    return lines;
}

void write_int_line(line_type *lines_array[], int n) {
	// line_type tmp=lines_array[n];
	for (int i=0;i<lines_array[n]->len;i++){
		printf("%d ",lines_array[n]->values[i]);
	}
	printf("\n%.2f",lines_array[n]->average);
}

void delete_int_lines(line_type array[], int line_count) {
}

int cmp (const void *a, const void *b) {
	const line_type *p1 = (const line_type *)a;
    const line_type *p2 = (const line_type *)b;
	
	if (p1->average-p2->average>0)
		return 1;
	else
		return -1;
}

void sort_by_average(line_type *lines_array[], int line_count) {
	for(int i=0;i<line_count-1;i++){ 
		line_type *curr=lines_array[i];
		for(int j=i+1;j<line_count;j++){
			if (cmp(curr,lines_array[j])>0){
				lines_array[i]=lines_array[j];
				lines_array[j]=curr;
				curr=lines_array[i];
			}
		}
	}
}

typedef struct {
	int r, c, v;
} triplet;

int read_sparse(triplet *triplet_array, int n_triplets) {
	triplet new;
	for (int k=0;k<n_triplets;k++){
		scanf("%d %d %d",&new.r,&new.c,&new.v);
		triplet_array[k]=new;
	}
	return n_triplets;
}

int cmp_triplets(const void *t1, const void *t2) {
	const triplet *a = (const triplet *)t1;
    const triplet *b = (const triplet *)t2;

	if (a->r-b->r==0)
		return a->c-b->c;
	else
		return a->r-b->r;
}

void make_CSR(triplet *triplet_array, int n_triplets, int rows, int *V, int *C, int *R) {
	
	for(int i=0;i<n_triplets-1;i++){ 
		triplet curr=triplet_array[i];
		for(int j=i+1;j<n_triplets;j++){
			if (cmp_triplets(&curr,&triplet_array[j])>0){
				triplet_array[i]=triplet_array[j];
				triplet_array[j]=curr;
				curr=triplet_array[i];
			}
		}
	}

	for(int i=0;i<n_triplets;i++){
		triplet curr=triplet_array[i];
		V[i]=curr.v;
		C[i]=curr.c;
		for (int j=curr.r+1;j<=rows;j++)
			R[j]+=1;
	}
}

void multiply_by_vector(int rows, const int *V, const int *C, const int *R, const int *x, int *y) {
	int cnt_in_row;
	int curr_pos=0;
	int row_matrix_val;
	for(int i=0;i<rows;i++){
		cnt_in_row=R[i+1]-R[i]; // ile ma i-ty rzad liczb
		row_matrix_val=0;
		for(int j=0;j<cnt_in_row;j++){
			row_matrix_val+=(V[curr_pos]*x[C[curr_pos]]);
			curr_pos+=1;
		}
		y[i]=row_matrix_val;
	}
}

void read_vector(int *v, int n) {
	for (int i=0;i<n;i++){
		scanf("%d",&v[i]);
	}
}

void write_vector(int *v, int n) {
	for(int i=0;i<n;i++){
		printf("%d ",v[i]);
	}
	printf("\n");
}

int read_int() {
	char c_buf[BUF_SIZE];
	fgets(c_buf, BUF_SIZE, stdin);
	return (int)strtol(c_buf, NULL, 10);
}

int main(void) {
	int to_do = read_int();

	int A[TAB_SIZE], B[TAB_SIZE], AB[TAB_SIZE];
	int n, lines_counter, rowsA, colsA, rowsB, colsB;
	int rows, cols, n_triplets;
	char *char_lines_array[TAB_SIZE] = { NULL };
	int continuous_array[TAB_SIZE];
	int *ptr_array[TAB_SIZE];
	triplet triplet_array[TAB_SIZE];
	int V[TAB_SIZE], C[TAB_SIZE], R[TAB_SIZE];
	int x[TAB_SIZE], y[TAB_SIZE];
	line_type *int_lines_array[TAB_SIZE];

	switch (to_do) {
		case 1:
			scanf("%d %d", &rowsA, &colsA);
			read_mat(rowsA, colsA, A);
			scanf("%d %d", &rowsB, &colsB);
			read_mat(rowsB, colsB, B);
			prod_mat(rowsA, colsA, colsB, A, B, AB);
			print_mat(rowsA, colsB, AB);
			// print_mat(rowsB,colsB,B);
			break;
		case 2:
			n = read_int() - 1; // we count from 1 :)
			ptr_array[0] = continuous_array;
			read_int_lines_cont(ptr_array, n);
			// write_int_line_cont(ptr_array, n);
			break;
		case 3:
			n = read_int() - 1;
			read_char_lines(char_lines_array,n);
			// write_char_line(char_lines_array, n);
			// delete_lines(char_lines_array);
			break;
		case 4:
			n = read_int() - 1;
			lines_counter = read_int_lines(int_lines_array);
			sort_by_average(int_lines_array, lines_counter);
			write_int_line(int_lines_array, n);
			// delete_int_lines(int_lines_array, lines_counter);
			break;
		case 5:
			scanf("%d %d %d", &rows, &cols, &n_triplets);
			n_triplets = read_sparse(triplet_array, n_triplets);
			read_vector(x, cols);
			make_CSR(triplet_array, n_triplets, rows, V, C, R);
			multiply_by_vector(rows, V, C, R, x, y);
			write_vector(V, n_triplets);
			write_vector(C, n_triplets);
			write_vector(R, rows + 1);
			write_vector(y, rows);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}

