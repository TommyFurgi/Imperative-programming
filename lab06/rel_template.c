#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_REL_SIZE 100
#define MAX_RANGE 100

typedef struct {
	int first;
	int second;
} pair;

int is_asymmetric(pair *tab, int n);

// Add pair to existing relation if not already there
int add_relation (pair*, int, pair);

// Case 1:

// The relation R is reflexive if xRx for every x in X
int is_reflexive(pair *tab, int n){
	int flag;
	for(int i=0;i<n;i++){
		flag=0;
		for(int j=0;j<n;j++){
			if (tab[j].first==tab[j].second){
				flag=1;
				break;
			} 
		}
		if (flag==0) return 0;
	}
	return 1;
}


// The relation R on the set X is called irreflexive
// if xRx is false for every x in X
int is_irreflexive(pair *tab, int n){
	int flag;
	for(int i=0;i<n;i++){
		flag=0;
		for(int j=0;j<n;j++){
			if (tab[j].first==tab[j].second){
				flag=1;
				break;
			} 
		}
		if (flag==1) return 0;
	}
	return 1;
}

// A binary relation R over a set X is symmetric if:
// for all x, y in X xRy <=> yRx
int is_symmetric(pair *tab, int n){
	int flag;
	for(int i=0;i<n;i++){
		flag=0;
		for(int j=0;j<n;j++){
			if (tab[i].first==tab[j].second && tab[j].first==tab[i].second){
				flag=1;
				break;
			} 
		}
		if (flag==0) return 0;
	}
	return 1;
}

// A binary relation R over a set X is antisymmetric if:
// for all x,y in X if xRy and yRx then x=y
int is_antisymmetric(pair *tab, int n){
	for(int i=0;i<n;i++){
		for(int j=0;j<n;j++){
			if (tab[i].first==tab[j].second && tab[j].first==tab[i].second){
				if (tab[j].first!=tab[j].second) return 0;
			} 
		}
	}
	return 1;
}

// A binary relation R over a set X is asymmetric if:
// for all x,y in X if at least one of xRy and yRx is false
int is_asymmetric(pair *tab, int n){
	for(int i=0;i<n;i++){
		for(int j=0;j<n;j++){
			if (tab[i].first==tab[j].second && tab[j].first==tab[i].second){
				return 0;
			} 
		}
	}
	return 1;
}

// A homogeneous relation R on the set X is a transitive relation if:
// for all x, y, z in X, if xRy and yRz, then xRz
int is_transitive(pair *tab, int n){
	int flag;
	for(int i=0;i<n;i++){
		flag=1;
		for(int j=0;j<n;j++){
			if (tab[j].first==tab[i].second){
				flag=0;
				for (int z=0;z<n;z++){
					if (tab[z].first==tab[i].first && tab[z].second==tab[j].second){
						flag=1;
						break;
					} 
				}
				// if (flag==1) break;
			} 
		}
		if (flag==0) return 0;
	}
	return 1;
}

// Case 2:

// A partial order relation is a homogeneous relation that is
// reflexive, transitive, and antisymmetric
int is_partial_order(pair *tab, int n){
	if(is_reflexive(tab,n)==1 && is_antisymmetric(tab,n)==1 && is_transitive(tab,n)==1) return 1;
	else return 0;
}

void swap (int array[], int i, int k) {
	int tmp=array[i];
	array[i]=array[k];
	array[k]=tmp;
}

void bubble_sort(int n, int array[]){
	int flag;
	for(int i=0;i<n;i++){
		flag=0;
		for(int j=0;j<n-i-1;j++){
			if (array[j]>array[j+1]){
				swap(array,j,j+1);
				flag=1;
			}
		}
		if (flag==0){
			break;
		}
	}
}

int get_domain(pair *tab, int n, int *X){
	int cnt=0,flag;
	for(int i=0;i<n;i++){
		flag=0;
		for(int j=0;j<cnt;j++){
			if (X[j]==tab[i].first) {
				flag=1;
				break;
			}
		}
		if (flag==0){
			X[cnt]=tab[i].first;
			cnt+=1;
		}
		flag=0;
		for(int j=0;j<cnt;j++){
			if (X[j]==tab[i].second) {
				flag=1;
				break;
			}
		}
		if (flag==0){
			X[cnt]=tab[i].second;
			cnt+=1;
		}
	}
	bubble_sort(cnt,X);
	return cnt;
}

// A total order relation is a partial order relation that is connected
int is_total_order(pair *tab, int n){
	if (is_partial_order(tab,n)==1 && is_connected(tab,n)==1) return 1;
	else return 0;
}

// Relation R is connected if for each x, y in X:
// xRy or yRx (or both)
int is_connected(pair *tab, int n){
	int pom[MAX_REL_SIZE];
	int dl=get_domain(tab,n,pom);
	int a,b,flag;
	for(int i=0;i<(dl-1);i++){
		a=pom[i];
		for(int j=i+1;j<dl;j++){
			b=pom[j];
			flag=0;
			for(int k=0;k<n;k++){
				if ((tab[k].first==a && tab[k].second==b) || (tab[k].first==b && tab[k].second==a)) flag=1;
			}
			if (flag==0) return 0;
		}
	}
	return 1;
}

int find_max_elements(pair *tab, int n, int *el){
	int cnt=0;
	int pom[MAX_REL_SIZE];
	int dl=get_domain(tab,n,pom);
	int flag;
	for(int i=0;i<dl;i++){
		flag=1;
		for(int j=0;j<n;j++){
			if (tab[j].second!=pom[i] && tab[j].first==pom[i]) flag=0;
		}
		if (flag==1){
			el[cnt]=pom[i];
			cnt+=1;
		}
	}
	return cnt;
}

int find_min_elements(pair *tab, int n, int *el){
	int cnt=0;
	int pom[MAX_REL_SIZE];
	int dl=get_domain(tab,n,pom);
	int flag;
	for(int i=0;i<dl;i++){
		flag=1;
		for(int j=0;j<n;j++){
			if (tab[j].second==pom[i] && tab[j].first!=pom[i]) flag=0;
		}
		if (flag==1){
			el[cnt]=pom[i];
			cnt+=1;
		}
	}
	return cnt;
}


// Case 3:

int insert_int (pair *tab, int n, pair new_element) {
	int a=new_element.first,b=new_element.second;
	for(int i=0;i<n;i++){
		if (tab[i].first==a && tab[i].second==b) return 0;
	}
	return 1;

}

int composition (pair *R, int size_r, pair *S, int size_s, pair *el){
	int cnt=0;
	pair new_pair;
	for(int i=0;i<size_r;i++){
		for(int j=0;j<size_s;j++){
			if (R[i].second==S[j].first){
				new_pair.first=R[i].first;
				new_pair.second=S[j].second;
				if (insert_int(el,cnt,new_pair)==1){
					el[cnt]=new_pair;
					cnt+=1;
				}
				
			}
		}
	}
	return cnt;
}

// // Comparator for pair
// int cmp_pair (const void *a, const void *b) {
// }



// Add pair to existing relation if not already there
int add_to_relation (pair *tab, int n, pair new_pair) {
	for(int i=0;i<n;i++){
		if (new_pair.first==tab[i].first && new_pair.second==tab[i].second) return 0;
	}
	tab[n]=new_pair;
	return 1;
}

// Read number of pairs, n, and then n pairs of ints
int read_relation(pair *relation) {
	int n,a,b,cnt=0;
	scanf("%d",&n);
	pair new_pair;
	for(int i=0;i<n;i++){
		scanf("%d %d",&a,&b);
		new_pair.first=a;
		new_pair.second=b;
		if (add_to_relation(relation,cnt,new_pair)) cnt++;
	}
	return cnt;
}

void print_int_array(int *array, int n) {
	printf("%d\n",n);
	for(int i=0;i<n;i++){
		printf("%d ",array[i]);
	}
	printf("\n");
}

int main(void) {
	int to_do;
	pair relation[MAX_REL_SIZE];
	pair relation_2[MAX_REL_SIZE];
	pair comp_relation[MAX_REL_SIZE];
	int domain[MAX_REL_SIZE];
	int max_elements[MAX_REL_SIZE];
	int min_elements[MAX_REL_SIZE];

	scanf("%d",&to_do);
	int size = read_relation(relation);
	int ordered, size_2, n_domain;

	switch (to_do) {
		case 1:
			printf("%d ", is_reflexive(relation, size));
			printf("%d ", is_irreflexive(relation, size));
			printf("%d ", is_symmetric(relation, size));
			printf("%d ", is_antisymmetric(relation, size));
			printf("%d ", is_asymmetric(relation, size));
			printf("%d\n", is_transitive(relation, size));
			break;
		case 2:
			ordered = is_partial_order(relation, size);
			n_domain = get_domain(relation, size, domain);
			printf("%d %d\n", ordered, is_total_order(relation, size));
			print_int_array(domain, n_domain);
			if (!ordered) break;
			int no_max_elements = find_max_elements(relation, size, max_elements);
			int no_min_elements = find_min_elements(relation, size, min_elements);
			print_int_array(max_elements, no_max_elements);
			print_int_array(min_elements, no_min_elements);
			break;
		case 3:
			size_2 = read_relation(relation_2);
			printf("%d\n", composition(relation, size, relation_2, size_2, comp_relation));
			// for(int i=0;i<4;i++){
			// 	printf("%d %d \n", comp_relation[i].first,comp_relation[i].second);
			// }
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}

