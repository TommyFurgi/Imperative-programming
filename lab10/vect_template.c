#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_STR_LEN 64

typedef struct Vector {
	void *data;
	size_t element_size;
	size_t size;
	size_t capacity;
} Vector;

typedef struct Person {
	int age;
	char first_name[MAX_STR_LEN];
	char last_name[MAX_STR_LEN];
} Person;

typedef int(*cmp_ptr)(const void*, const void*);
typedef int(*predicate_ptr)(void*);
typedef void(*read_ptr)(void*);
typedef void(*print_ptr)(const void*);

// Allocate vector to initial capacity (block_size elements),
// Set element_size, size (to 0), capacity
void init_vector(Vector *vector, size_t block_size, size_t element_size) {
	vector->data = (size_t*)malloc(block_size * element_size);
    vector->element_size = element_size;
    vector->size = 0;
    vector->capacity = block_size;
}

// If new_capacity is greater than the current capacity,
// new storage is allocated, otherwise the function does nothing.
void reserve(Vector *vector, size_t new_capacity) {
	if (new_capacity > vector->capacity) {
        void* newData = realloc(vector->data, new_capacity * vector->element_size);
        vector->data = newData;
        vector->capacity = new_capacity;
    }
}

// Resizes the vector to contain new_size elements.
// If the current size is greater than new_size, the container is
// reduced to its first new_size elements.

// If the current size is less than new_size,
// additional zero-initialized elements are appended
void resize(Vector *vector, size_t new_size) {
	if (new_size < vector->size) {
        vector->size = new_size;
		vector->capacity = new_size;
    } else if (new_size > vector->size) {
        reserve(vector, new_size);
        size_t i;
        for (i = vector->size; i < new_size; i++) {
            char* element = (char*)vector->data + (i * vector->element_size);
            memset(element, 0, vector->element_size); // Zerowanie nowych elementów
        }
        vector->size = new_size;
    }
}


// Add element to the end of the vector
void push_back(Vector *vector, void *value) {
	if (vector->size == vector->capacity) {
        reserve(vector, vector->capacity * 2); // Podwajanie pojemności w przypadku braku miejsca
    }
    char* target = (char*)vector->data + (vector->size * vector->element_size);
    memcpy(target, value, vector->element_size);
    vector->size++;
}

// Remove all elements from the vector
void clear(Vector *vector) {
	vector->size = 0;
}

// Insert new element at index (0 <= index <= size) position
void insert(Vector *vector, size_t index, void *value) {
	if (index > vector->size) {
        return; // Nieprawidłowa pozycja
    }
    if (vector->size == vector->capacity) {
        reserve(vector, vector->capacity * 2); // Podwajanie pojemności w przypadku braku miejsca
    }
    size_t i;
    for (i = vector->size; i > index; i--) {
        char* dest = (char*)vector->data + (i * vector->element_size);
        char* src = (char*)vector->data + ((i - 1) * vector->element_size);
        memcpy(dest, src, vector->element_size);
    }
    char* target = (char*)vector->data + (index * vector->element_size);
    memcpy(target, value, vector->element_size);
    vector->size++;
}

// Erase element at position index
void erase(Vector *vector, size_t index) {
	if (index >= vector->size) {
        return; // Nieprawidłowa pozycja
    }
    size_t i;
    for (i = index; i < vector->size - 1; i++) {
        char* dest = (char*)vector->data + (i * vector->element_size);
        char* src = (char*)vector->data + ((i + 1) * vector->element_size);
        memcpy(dest, src, vector->element_size);
    }
    vector->size--;
}

// Erase all elements that compare equal to value from the container
void erase_value(Vector *vector, void *value, cmp_ptr cmp) {
	size_t i, j;
    for (i = 0, j = 0; i < vector->size; i++) {
        char* val = (char*)vector->data + (i * vector->element_size);
        if (cmp(value, val) != 0) {
            char* dest = (char*)vector->data + (j * vector->element_size);
            char* src = (char*)vector->data + (i * vector->element_size);
            if (dest != src) {
                memcpy(dest, src, vector->element_size);
            }
            j++;
        }
    }
    vector->size = j;
}

// Erase all elements that satisfy the predicate from the vector
void erase_if(Vector *vector, int (*predicate)(void *)) {
	size_t i, j;
    for (i = 0, j = 0; i < vector->size; i++) {
        char* element = (char*)vector->data + (i * vector->element_size);
        if (!predicate(element)) {
            char* dest = (char*)vector->data + (j * vector->element_size);
            char* src = (char*)vector->data + (i * vector->element_size);
            if (dest != src) {
                memcpy(dest, src, vector->element_size);
            }
            j++;
        }
    }
    vector->size = j;
}

// Request the removal of unused capacity
void shrink_to_fit(Vector *vector) {
	if (vector->size < vector->capacity) {
        vector->data = realloc(vector->data, vector->size * vector->element_size);
        vector->capacity = vector->size;
    }
}

// integer comparator
int int_cmp(const void *v1, const void *v2) {
	int valueA = *(int*)v1;
    int valueB = *(int*)v2;
    if (valueA < valueB) {
        return -1;
    } else if (valueA > valueB) {
        return 1;
    } else {
        return 0;
    }
}

// char comparator
int char_cmp(const void *v1, const void *v2) {
	char valueA = *(char*)v1;
    char valueB = *(char*)v2;
    if (valueA < valueB) {
        return -1;
    } else if (valueA > valueB) {
        return 1;
    } else {
        return 0;
    }
}

// Person comparator:
// Sort according to age (decreasing)
// When ages equal compare first name and then last name
int person_cmp(const void *p1, const void *p2) {
	Person *A = (Person*)p1;
    Person *B = (Person*)p2;
    if (A->age > B->age) {
        return -1;
    } else if (A->age < B->age) {
        return 1;
    } else {
		if (strcmp(A->first_name, B->first_name)==0){
			if (strcmp(A->last_name, B->last_name)==0){
				return 0;
			}
			return strcmp(A->last_name, B->last_name);
		}
        return strcmp(A->first_name, B->first_name);
    }
}

// predicate: check if number is even
int is_even(void *value) {
	int val = *(int*)value;
	if (val%2==0)
		return 1;
	return 0;
}

// predicate: check if char is a vowel
int is_vowel(void *value) {
	char c = *(char*)value;
	if (c=='a' || c=='e' || c=='o' || c=='y' || c=='i' || c=='u' || c=='A' || c=='E' || c=='O' || c=='Y' || c=='I' || c=='U')
		return 1;
	return 0;
}

// predicate: check if person is older than 25
int is_older_than_25(void *person) {
	Person *p= (Person*)person;
	if (p->age>25)
		return 1;
	return 0;
}

// print integer value
void print_int(const void *v) {
	int value = *(int*)v;
    printf("%d ", value);
}

// print char value
void print_char(const void *v) {
	char c= *(char*)v;
	printf("%c ", c);
}

// print structure Person
void print_person(const void *v) {
	Person* person = (Person*)v;
    printf("%d %s %s\n", person->age, person->first_name, person->last_name);
}

// print capacity of the vector and its elements
void print_vector(Vector *vector, print_ptr print) {
	printf("%zu\n", vector->capacity);
    size_t i;
    for (i = 0; i < vector->size; i++) {
        char* element = (char*)vector->data + (i * vector->element_size);
        print(element);
    }
}

// read int value
void read_int(void* value) {
	int val;
	scanf("%d",&val);
	 *(int*)value = val;
}

// read char value
void read_char(void* value) {
	char val;
    scanf(" %c", &val);
    *(char*)value = val;
}

// read struct Person
void read_person(void* value) {
	Person* person = (Person*)value;
    scanf("%d %s %s", &(person->age), person->first_name, person->last_name);
}

void vector_test(Vector *vector, size_t block_size, size_t elem_size, int n, read_ptr read,
		 cmp_ptr cmp, predicate_ptr predicate, print_ptr print) {
	init_vector(vector, block_size, elem_size);
	void* v = malloc(vector->element_size);
	size_t index, size;
	for (int i = 0; i < n; ++i) {
		char op;
		scanf(" %c", &op);
		switch (op) {
			case 'p': // push_back
				read(v);
				push_back(vector, v);
				break;
			case 'i': // insert
				scanf("%zu", &index);
				read(v);
				insert(vector, index, v);
				break;
			case 'e': // erase
				scanf("%zu", &index);
				erase(vector, index);
				break;
			case 'v': // erase
				read(v);
				erase_value(vector, v, cmp);
				break;
			case 'd': // erase (predicate)
				erase_if(vector, predicate);
				break;
			case 'r': // resize
				scanf("%zu", &size);
				resize(vector, size);
				break;
			case 'c': // clear
				clear(vector);
				break;
			case 'f': // shrink
				shrink_to_fit(vector);
				break;
			case 's': // sort
				qsort(vector->data, vector->size,
				      vector->element_size, cmp);
				break;
			default:
				printf("No such operation: %c\n", op);
				break;
		}
	}
	print_vector(vector, print);
	free(vector->data);
	free(v);
}

int main(void) {
	int to_do, n;
	Vector vector_int, vector_char, vector_person;

	scanf("%d%d", &to_do, &n);

	switch (to_do) {
		case 1:
			vector_test(&vector_int, 4, sizeof(int), n, read_int, int_cmp,
				is_even, print_int);
			break;
		case 2:
			vector_test(&vector_char, 2, sizeof(char), n, read_char, char_cmp,
				is_vowel, print_char);
			break;
		case 3:
			vector_test(&vector_person, 2, sizeof(Person), n, read_person,
				person_cmp, is_older_than_25, print_person);
			break;
		default:
			printf("Nothing to do for %d\n", to_do);
			break;
	}

	return 0;
}

