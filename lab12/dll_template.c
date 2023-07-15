#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// list node
typedef struct Node {
	int *data;
	size_t array_size;
	struct Node* next;
	struct Node* prev;
} Node;

// doubly linked list
typedef struct List {
	Node *head;
	Node *tail;
} List;

// iterator
typedef struct iterator {
	struct Node* node_ptr;
	size_t position;
} iterator;

// forward initialization
iterator begin(Node* head) {
	iterator it = { head, 0 };
	return it;
}

// backward initialization;
// points to the element following the last one
iterator end(Node* tail) {
	iterator it = { tail, tail->array_size };
	return it;
}

void *safe_malloc(size_t size) {
	void *ptr = malloc(size);
	if (ptr) return ptr;
	exit(EXIT_FAILURE);
}

void *safe_realloc(void *old_ptr, size_t size) {
	void *ptr = realloc(old_ptr, size);
	if (ptr) return ptr;
	free(old_ptr);
	exit(EXIT_FAILURE);
}

Node *create_node(int *data, size_t array_size, Node *next, Node *prev) {
	Node *node = safe_malloc(sizeof(Node));
	node->data = data;
	node->array_size = array_size;
	node->next = next;
	node->prev = prev;
	return node;
}

// initialize list
// creates the front and back sentinels
void init(List *list) {
	list->head = create_node(NULL, 0, NULL, NULL);
	list->tail = create_node(NULL, 0, NULL, list->head);
	list->head->next = list->tail;
}

// to implement ...

// append node to the list
void push_back(List *list, int *data, size_t array_size) {
	List *l = (List*) list;
	int *d = (int*) data;


	Node *a=l->tail->prev;
	Node *p = create_node(d,array_size,l->tail,a);
	a->next=p;
	l->tail->prev=(Node*)p;
}

// set iterator to move n elements forward from its current position
void skip_forward(iterator* itr, size_t n) {
}

// forward iteration - get n-th element in the list
int get_forward(List *list, size_t n) {
	int m=n;
	for(Node *node = list->head->next; node != list->tail; node = node->next) {
		if (m<=node->array_size)
			return node->data[m-1];
		m-=node->array_size;
	
	}
}

// set iterator to move n elements backward from its current position
void skip_backward(iterator* itr, size_t n) {
}

// backward iteration - get n-th element from the end of the list
int get_backward(List *list, size_t n) {
	int m=n;
	for(Node *node = list->tail->prev; node != list->head; node = node->prev) {
		if (m<=node->array_size)
			return node->data[node->array_size-m];
		m-=node->array_size;
	}
}

void remove_node(Node *node_ptr) {
	Node *p=(Node*)node_ptr;
	p->prev->next=p->next;
	p->next->prev=p->prev;
	free(p);
}

// remove n-th element; if array empty remove node
void remove_at(List *list, size_t n) {
	int m=n;
	for(Node *node = list->head->next; node != list->tail; node = node->next) {
		if (m<=node->array_size){
			for (int i=m;i<node->array_size;i++)
				node->data[i-1]=node->data[i];

			node->array_size=node->array_size-1;
			if (node->array_size==0)
				remove_node(node);
			return;
		}
		m-=node->array_size;
	
	}
}

// return the number of digits of number n
size_t digits(int n) {
	int d=1;
	while (abs(n)>pow(10,d))
		d+=1;
	return d;
}

// inserts 'value' to the node with the same digits' count
// otherwise insert new node
void put_in_order(List *list, int value) {
	int cnt=digits(value);
	Node *node = list->head->next;
	for(node = list->head->next; node != list->tail; node = node->next) {
		int d=digits(node->data[0]);
		if (d==cnt){
			int index = node->array_size;
            int *dat = safe_realloc(node->data, (index + 1) * sizeof(int));
            
            while (index > 0 && dat[index - 1] > value) {
                dat[index] = dat[index - 1];
                index--;
            }
            
            dat[index] = value;
            node->data = dat;
            node->array_size++;
            return;
		}
		if (d>cnt){
			break;
		}

	}
	int *tab = (int*) safe_malloc(sizeof(int));
	tab[0]=value;
	Node *a=node->prev;
	Node *new = create_node(tab,1,node,a);
	a->next=new;
	node->prev=new;
}

// -------------------------------------------------------------
// helper functions

// print list
void dumpList(const List *list) {
	for(Node *node = list->head->next; node != list->tail; node = node->next) {
		printf("-> ");
		for (int k = 0; k < node->array_size; k++) {
			printf("%d ", node->data[k]);
		}
		printf("\n");
	}
}

// free list
void freeList(List *list) {
	Node *to_delete = list->head->next, *next;
	while(to_delete != list->tail) {
		next = to_delete->next;
		remove_node(to_delete);
		to_delete = next;
	}
}

// read int vector
void read_vector(int tab[], size_t n) {
	for (size_t i = 0; i < n; ++i) {
		scanf("%d", tab + i);
	}
}

// initialize the list and push data
void read_list(List *list) {
	int n;
	size_t size;
	scanf("%d", &n); // number of nodes
	for (int i = 0; i < n; i++) {
		scanf("%zu", &size); // length of the array in i-th node
		int *tab = (int*) safe_malloc(size * sizeof(int));
		read_vector(tab, size);
		push_back(list, tab, size);
	}
}

int main() {
	int to_do, value;
	size_t size, m;
	List list;
	init(&list);

	scanf("%d", &to_do);
	switch (to_do) {
		case 1:
			read_list(&list);
			dumpList(&list);
			break;
		case 2:
			read_list(&list);
			scanf("%zu", &size);
			for (int i = 0; i < size; i++) {
				scanf("%zu", &m);
				printf("%d ", get_forward(&list, m));
			}
			printf("\n");
			break;
		case 3:
			read_list(&list);
			scanf("%zu", &size);
			for (int i = 0; i < size; i++) {
				scanf("%zu", &m);
				printf("%d ", get_backward(&list, m));
			}
			printf("\n");
			break;
		case 4:
			read_list(&list);
			scanf("%zu", &size);
			for (int i = 0; i < size; i++) {
				scanf("%zu", &m);
				remove_at(&list, m);
			}
			dumpList(&list);
			break;
		case 5:
			scanf("%zu", &size);
			for (int i = 0; i < size; i++) {
				scanf("%d", &value);
				put_in_order(&list, value);
			}
			dumpList(&list);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	freeList(&list);

	return 0;
}
