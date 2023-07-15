#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define BUFFER_SIZE 1024
#define MAX_RATE 4
#define MEMORY_ALLOCATION_ERROR  (-1)

typedef struct DataWord {
	char *word;
	int counter;
} DataWord;

typedef union {
	int int_data;
	char char_data;
	// ... other primitive types used
	DataWord *ptr_data;
} data_union;

typedef struct ht_element {
	struct ht_element *next;
	data_union data;
} ht_element;

typedef void (*DataFp)(data_union);
typedef void (*DataPFp)(data_union*);
typedef int (*CompareDataFp)(data_union, data_union);
typedef size_t (*HashFp)(data_union, size_t);
typedef data_union (*CreateDataFp)(void*);

typedef struct {
	size_t size;
	size_t no_elements;
	ht_element *ht;
	DataFp dump_data;
	CreateDataFp create_data;
	DataFp free_data;
	CompareDataFp compare_data;
	HashFp hash_function;
	DataPFp modify_data;
} hash_table;

// ---------------------- functions to implement

// initialize table fields
void init_ht(hash_table *p_table, size_t size, DataFp dump_data, CreateDataFp create_data,
		 DataFp free_data, CompareDataFp compare_data, HashFp hash_function, DataPFp modify_data) {

	p_table->size = size;
	p_table->no_elements = 0;
	p_table->ht = (ht_element*)malloc(size * sizeof(ht_element));
	p_table->dump_data = dump_data;
	p_table->create_data = create_data;
	p_table->free_data = free_data;
	p_table->compare_data = compare_data;
	p_table->hash_function = hash_function;
	p_table->modify_data = modify_data;

	// Initialize ht elements
	for (size_t i = 0; i < size; i++) {
		p_table->ht[i].next = NULL;
	}
}

// print elements of the list with hash n
void dump_list(const hash_table* p_table, size_t n) {
    ht_element *current = p_table->ht[n].next;
    while (current != NULL) {
        p_table->dump_data(current->data);
		current = current->next;
    }
}

// Free element pointed by data_union using free_data() function
void free_element(DataFp free_data, ht_element *to_delete) {
	if (free_data != NULL) {
        free_data(to_delete->data);   
    }
	free(to_delete);
}

// free all elements from the table (and the table itself)
void free_table(hash_table* p_table) {
	for (size_t i = 0; i < p_table->size; i++) {
		ht_element *current = p_table->ht[i].next;
		while (current != NULL) {
			ht_element *next = current->next;
			free_element(p_table->free_data, current);
			current = next;
		}
	}
	free(p_table->ht);
	p_table->ht = NULL;
	p_table->size = 0;
	p_table->no_elements = 0;
}

// calculate hash function for integer k
size_t hash_base(int k, size_t size) {
	static const double c = 0.618033988; // (sqrt(5.) – 1) / 2.;
	double tmp = k * c;
	return (size_t)floor((double)size * (tmp - floor(tmp)));
}

void rehash(hash_table *p_table) {
	size_t new_size = p_table->size * 2;
	ht_element* new_ht = (ht_element*)malloc(new_size * sizeof(ht_element));

	if (new_ht == NULL) {
		// Failed to allocate memory for the new hash table
		return;
	}

	// Initialize the new hash table with sentinel nodes
	for (size_t i = 0; i < new_size; i++) {
		new_ht[i].next = NULL;
	}

	// Rehash the elements from the existing hash table
	for (size_t i = 0; i < p_table->size; i++) {
		ht_element* current = p_table->ht[i].next;  // Skip the sentinel node
		while (current != NULL) {
			ht_element* next = current->next;
			size_t new_index = p_table->hash_function(current->data, new_size);

			// Insert the element at the beginning of the new list
			current->next = new_ht[new_index].next;
			new_ht[new_index].next = current;

			current = next;
		}
	}

	// Free the old hash table
	free(p_table->ht);

	// Update the hash table fields with the new hash table
	p_table->ht = new_ht;
	p_table->size = new_size;
}

// find element; return pointer to previous
ht_element* find_previous(hash_table* p_table, data_union data) {
    size_t index = p_table->hash_function(data, p_table->size);
    ht_element* prev = &(p_table->ht[index]);
    ht_element* current = prev->next;

    while (current != NULL) {
        if (p_table->compare_data(current->data, data) == 0) {
            return prev;
        }

        prev = current;
        current = current->next;
    }

    return NULL;
}

// return pointer to element with given value
ht_element* get_element(hash_table* p_table, data_union* data) {
    size_t index = p_table->hash_function(*data, p_table->size);
    ht_element* current = p_table->ht[index].next;

    while (current != NULL) {
        if (p_table->compare_data(current->data, *data) == 0) {
            return current;
        }
        current = current->next;
    }

    return NULL;
}

// insert element
void insert_element(hash_table* p_table, data_union* data) {
    // size_t index = p_table->hash_function(data,p_table->size) % p_table->size;
	// ht_element new_element;
    // new_element->data = *data;
    // new_element->next = p_table->ht[index].next;
    // p_table->ht[index].next = new_element;
    // p_table->no_elements++;
	size_t index = p_table->hash_function(*data, p_table->size) % p_table->size;
	if (get_element(p_table,data)==NULL){
		ht_element* new_element = (ht_element*)malloc(sizeof(ht_element));
		new_element->data = *data;
		new_element->next = p_table->ht[index].next;
		p_table->ht[index].next = new_element;
		p_table->no_elements++;
		if (p_table->no_elements>p_table->size*MAX_RATE)
			rehash(p_table);
	}
}

// remove element
void remove_element(hash_table* p_table, data_union data) {
    size_t index = p_table->hash_function(data, p_table->size) % p_table->size;
    ht_element* prev = &p_table->ht[index];
    ht_element* current = p_table->ht[index].next;

    while (current != NULL) {
        if (p_table->compare_data(current->data, data) == 0) {
            prev->next = current->next;
			free_element(p_table->free_data,current);
            p_table->no_elements--;
            return;
        }

        prev = current;
        current = current->next;
    }
}

// type-specific definitions

// int element
size_t hash_int(data_union data, size_t size) {
	return hash_base(data.int_data, size);
}

void dump_int(data_union data) {
    printf("%d ", data.int_data);
}

int cmp_int(data_union a, data_union b) {

    if (a.int_data < b.int_data) {
        return -1;
    } else if (a.int_data > b.int_data) {
        return 1;
    } else {
        return 0;
    }
}

// data_union create_int(void* value) {
// 	data_union u;
//     int* new_int = (int*)malloc(sizeof(int));
//     *new_int = *((int*)value);
// 	u.int_data=new_int;
// 	return u;
// 	// data_union data;
//     // data.int_data = *(int*)value;
//     // return data;
// }
data_union create_int(data_union *data) {
	int d;
	scanf(" %d", &d);
	data->int_data=d;
	return *data;
}
// char element

size_t hash_char(data_union data, size_t size) {
	return hash_base((int)data.char_data, size);
}

void dump_char(data_union data) {
	printf("%c ", data.char_data);
}

int cmp_char(data_union a, data_union b) {
	// return strcmp(a.char_data,b.char_data);
	if (a.char_data < b.char_data) {
        return -1;
    } else if (a.char_data > b.char_data) {
        return 1;
    } else {
        return 0;
    }
}

// data_union create_char(void* value) {
data_union create_char(data_union *data) {
	char d;
	scanf(" %c", &d);
	data->char_data=d;
	return *data;
}

// Word element



void dump_word(data_union data) {
	printf("%s %d", data.ptr_data->word,data.ptr_data->counter);
}

void free_word(data_union data) {
    free(data.ptr_data->word);
	free(data.ptr_data);
}

int cmp_word(data_union a, data_union b) {
	// const DataWord* word_a = (const DataWord*)a;
    // const DataWord* word_b = (const DataWord*)b;

    // if (strcmp(word_b->word, " ") == 0)
    //     return 1;

    return strcasecmp(a.ptr_data->word, b.ptr_data->word);
}

size_t hash_word(data_union data, size_t size) {
	int s = 0;
	DataWord *dw = (DataWord*)data.ptr_data;
	char *p = dw->word;
	while (*p) {
		s += *p++;
	}
	return hash_base(s, size);
}

void modify_word(data_union *data) {
	data_union *d = (data_union*)data;
	d->ptr_data->counter+=1;
}

data_union create_data_word(void *value) {
	char *c=(char*)value;
	for (size_t i = 0; i < strlen(c); i++) {
		c[i] = tolower(c[i]);
	}
	data_union p;
	p.ptr_data = (DataWord*)malloc(sizeof(DataWord)); 
    p.ptr_data->word = strdup(c); 
    p.ptr_data->counter = 1;
    
    return p;
}

#define DELIMITERS " .,?!:;-"
// read text, parse it to words, and insert these words to the hashtable
void stream_to_ht(hash_table *p_table, FILE *stream) {
	char line[BUFFER_SIZE];

    while (fgets(line, sizeof(line), stream) != NULL) {
        char* token = strtok(line, DELIMITERS);
        while (token != NULL) {
            // Remove leading and trailing whitespaces from the token
            char* start = token;
            char* end = token + strlen(token) - 1;
            while (isspace(*start)) start++;
            while (end > start && isspace(*end)) end--;
            *(end + 1) = '\0';

            // Check if the token is not empty
            if (*start != '\0') {
                data_union data = create_data_word(start);
				ht_element *element = get_element(p_table,&data);
				if (element!=NULL)
					p_table->modify_data(&(element->data));
				else
					insert_element(p_table,&data);
            }

            token = strtok(NULL, DELIMITERS);
        }
    }
}

// test primitive type list
void test_ht(hash_table *p_table, int n) {
	char op;
	data_union data;
	for (int i = 0; i < n; ++i) {
		scanf(" %c", &op);
		data=p_table->create_data(&data);
		// data = p_table->create_data(NULL); // should give the same result as the line above
		switch (op) {
			case 'r':
				remove_element(p_table, data);
				break;
			case 'i':
				insert_element(p_table, &data);
				break;
			default:
				printf("No such operation: %c\n", op);
				break;
		}
	}
}

int main(void) {
	int to_do, n;
	size_t index;
	hash_table table;
	char buffer[BUFFER_SIZE];
	data_union data;

	scanf ("%d", &to_do);
	switch (to_do) {
		case 1: // test integer hash table
			scanf("%d %zu", &n, &index);
			init_ht(&table, 4, dump_int, create_int, NULL, cmp_int, hash_int, NULL);
			test_ht(&table, n);
			printf ("%zu\n", table.size);
			dump_list(&table, index);
			break;
		case 2: // test char hash table
			scanf("%d %zu", &n, &index);
			init_ht(&table, 4, dump_char, create_char, NULL, cmp_char, hash_char, NULL);
			test_ht(&table, n);
			printf ("%zu\n", table.size);
			dump_list(&table, index);
			break;
		case 3: // read words from text, insert into hash table, and print
			scanf("%s", buffer);
			init_ht(&table, 8, dump_word, create_data_word, free_word, cmp_word, hash_word, modify_word);
			stream_to_ht(&table, stdin);
			printf ("%zu\n", table.size);
			data = table.create_data(buffer);
			ht_element *e = get_element(&table, &data);
			if (e) table.dump_data(e->data);
			if (table.free_data) table.free_data(data);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	free_table(&table);

	return 0;
}

