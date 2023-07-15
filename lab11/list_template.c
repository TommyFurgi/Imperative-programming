#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUFFER_SIZE 1024

struct List;
typedef void (*DataFp)(void*);
typedef void (*ConstDataFp)(const void*);
typedef  int (*CompareDataFp)(const void*, const void*);

typedef struct ListElement {
	struct ListElement *next;
	void *data;
} ListElement;

typedef struct {
	ListElement *head;
	ListElement *tail;
	ConstDataFp dump_data;
	DataFp free_data;
	CompareDataFp compare_data;
	DataFp modify_data;
} List;

void *safe_malloc(size_t size) {
	void *ptr = malloc(size);
	if(ptr) return ptr;
	printf("malloc error\n");
	exit(EXIT_FAILURE);
}

void *safe_strdup(const char *string) {
	void *ptr = strdup(string);
	if(ptr) return ptr;
	printf("strdup error\n");
	exit(EXIT_FAILURE);
}

// --- generic functions --- for any data type

void init_list(List *p_list, ConstDataFp dump_data, DataFp free_data,
		CompareDataFp compare_data, DataFp modify_data) {
	p_list->head=NULL;
	p_list->tail=NULL;
	p_list->dump_data=dump_data;
	p_list->free_data=free_data;
	p_list->compare_data=compare_data;
	p_list->modify_data;

}

// Print elements of the list
void dump_list(const List* p_list) {
	ListElement* current = p_list->head;
    while (current != NULL) {
        p_list->dump_data(current->data);
        current = current->next;
    }
}


// Free element pointed by data using free_data() function
void free_element(DataFp free_data, ListElement *to_delete) {
	if (to_delete == NULL) {
        return;
    }

    if (free_data != NULL) {
        free_data(to_delete->data);
    }
    
    free(to_delete);
}

// Free all elements of the list
void free_list(List* p_list) {
	ListElement* current = p_list->head;
    while (current != NULL) {
        ListElement* temp = current;
        current = current->next;

        if (p_list->free_data != NULL) {
            p_list->free_data(temp->data);
        }

        free(temp);
    }

    p_list->head = NULL;
    p_list->tail = NULL;
}

// Push element at the beginning of the list
void push_front(List *p_list, void *data){
    ListElement* new_element = (ListElement*)malloc(sizeof(ListElement));
    new_element->data = data;
    new_element->next = NULL;

    if (p_list->head == NULL) {
        p_list->head = new_element;
        p_list->tail = new_element;
    } else {
        new_element->next = p_list->head;
        p_list->head = new_element;
    }
}

// Push element at the end of the list
void push_back(List *p_list, void *data) {
	ListElement* new_element = (ListElement*)malloc(sizeof(ListElement));
    new_element->data = data;
    new_element->next = NULL;

    if (p_list->tail == NULL) {
        p_list->head = new_element;
        p_list->tail = new_element;
    } else {
        p_list->tail->next = new_element;
        p_list->tail = new_element;
    }
}

// Remove the first element
void pop_front(List *p_list) {
	if (p_list->head == NULL) {
        return;
    }

    ListElement* element_to_remove = p_list->head;
    p_list->head = p_list->head->next;

    if (p_list->head == NULL) {
        p_list->tail = NULL;
    }

    if (p_list->free_data != NULL) {
        p_list->free_data(element_to_remove->data); // free int
    }

    // free_element(p_list->free_data,element_to_remove);
}

// Reverse the list
void reverse(List *p_list) {
	ListElement* current = p_list->head;
    ListElement* prev = NULL;
    ListElement* next = NULL;

    while (current != NULL) {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }

    p_list->tail = p_list->head;
    p_list->head = prev;
}

// find element in sorted list after which to insert given element
ListElement* find_insertion_point(const List *p_list, ListElement *p_element) {
}

// Insert element after 'previous'
void push_after(List *p_list, void *data, ListElement *previous) {
}

// Insert element preserving order
void insert_in_order(List *p_list, void *p_data) {
	ListElement* new_element = (ListElement*)malloc(sizeof(ListElement));
    new_element->data = p_data;
    new_element->next = NULL;

    if (p_list->head == NULL || p_list->compare_data(p_data, p_list->head->data) <= 0) {
		if (p_list->head != NULL && p_list->compare_data(p_data, p_list->head->data) == 0)
            // if (p_list->modify_data!=NULL)
            //     p_list->modify_data(p_list->head->data);
			return;
        new_element->next = p_list->head;
        p_list->head = new_element;
        if (p_list->tail == NULL) {
            p_list->tail = new_element;
        }
        return;
    }

    ListElement* current = p_list->head;
    while (current->next != NULL && p_list->compare_data(p_data, current->next->data) > 0) {
        current = current->next;
    }
    if (current->next != NULL && p_list->compare_data(p_data, current->next->data) == 0)
    //         if (p_list->modify_data!=NULL)
    //             p_list->modify_data(p_list->head->data);
			return;
    new_element->next = current->next;
    current->next = new_element;
    if (new_element->next == NULL) {
        p_list->tail = new_element;
    }
}

// -----------------------------------------------------------
// --- type-specific definitions

// int element

void dump_int(const void *d) {
	printf("%d ", *(int*)d);
}

void free_int(void *d) {
	free(d);
}

int cmp_int(const void *a, const void *b) {
	int num_a = *(int*)a;
    int num_b = *(int*)b;
    if (num_a < num_b) {
        return -1;
    } else if (num_a > num_b) {
        return 1;
    } else {
        return 0;
    }
}

int *create_data_int(int v) {
	int* data = (int*)malloc(sizeof(int));
    *data = v;
    return data;
}

// Word element

typedef struct DataWord {
	char *word;
	int counter;
} DataWord;

void dump_word (const void *d) {
	const DataWord* data = (const DataWord*)d;
    printf("%s ", data->word);
}

void free_word(void *d) {
	const DataWord* data = (const DataWord*)d;
    free(data->word);
    free((void*)data);
}

int cmp_word_alphabet(const void *a, const void *b) {
	const DataWord* word_a = (const DataWord*)a;
    const DataWord* word_b = (const DataWord*)b;

    if (strcmp(word_b->word, " ") == 0)
        return 1;

    return strcasecmp(word_a->word, word_b->word);
}

int cmp_word_counter(const void *a, const void *b) {
	const DataWord* word_a = (const DataWord*)a;
    const DataWord* word_b = (const DataWord*)b;

    if (word_a->counter == word_b->counter)
        return 1;
    return 0;
}   

void modify_word(void *p) {
	DataWord* word = (DataWord*)p;
    word->counter++;
}

void dump_word_lowercase(const void *p_list, void *d){
    DataWord* word_data = (DataWord*)d;
    char* word = word_data->word;

    for (size_t i = 0; i < strlen(word); i++) {
        word[i] = tolower(word[i]);
    }
    printf("%s ", word);
}

void *create_data_word(char *string, int counter) {
    DataWord *newDataWord = (DataWord *)malloc(sizeof(DataWord));
    newDataWord->word = (char *)malloc((strlen(string) + 1) * sizeof(char));
    strcpy(newDataWord->word, string);
    newDataWord->counter = counter;
    return (void *)newDataWord;
}


// Print elements of the list if comparable to data
void dump_list_if(List *p_list, void *data) {
	DataWord *target_data = (DataWord *)data;
    ListElement *current = p_list->head;

    while (current != NULL) {
        DataWord *current_data = (DataWord *)current->data;
        if (p_list->compare_data(current->data,target_data)==1) {
            printf("%s ", current_data->word);  
        }
        current = current->next;
    }
}

ListElement* create_element(void* data) { 
    ListElement* new_element = malloc(sizeof(ListElement));
    if (new_element == NULL) {
        fprintf(stderr, "Failed to allocate memory for new element.\n");
        exit(EXIT_FAILURE);
    }
    
    new_element->data = data;
    new_element->next = NULL;
    
    return new_element;
}

void insert_sorted(List *p_list, void *data, CompareDataFp cmp) { // cmp - cmp_word_alphabet
    ListElement *current = p_list->head;
    ListElement *previous = NULL;
	DataWord *d=(void*) data;
    char* word = d->word;

    for (size_t i = 0; i < strlen(word); i++) {
        word[i] = tolower(word[i]);
    }

    while (current != NULL && cmp(d, current->data) > 0) {
        previous = current;
        current = current->next;
    }

    if (current != NULL && cmp(d, current->data) == 0) {
        // Element już istnieje, zwiększ liczbę wystąpień
        modify_word(current->data);
        // free_element(p_list->free_data, data);
    } else {
        // Wstawianie nowego elementu

        
        ListElement *new_element = create_element(d);
        // DataWord *dataw = new_element->data;
        // printf("%s ", dataw->word);


        if (previous == NULL) {
            // Wstawianie na początek listy
            new_element->next = p_list->head;
            p_list->head = new_element;
        } else {
            // Wstawianie w środek listy
            new_element->next = current;
            previous->next = new_element;
        }

    }
}

#define DELIMITERS " .,?!:;-"
// read text, parse it to words, and insert those words to the list.
// Order of insertions is given by the last parameter of type CompareDataFp.
// (comparator function address). If this address is not NULL the element is
// inserted according to the comparator. Otherwise, read order is preserved.
void stream_to_list(List *p_list, FILE *stream, CompareDataFp cmp) {
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
                DataWord* data = create_data_word(start, 1);



                if (cmp != NULL) {
                    insert_sorted(p_list, data, cmp);
                } else {
                    push_back(p_list, data);
                }
            }

            token = strtok(NULL, DELIMITERS);
        }
    }
}

// test integer list
void list_test(List *p_list, int n) {
	char op;
	int v;
	for (int i = 0; i < n; ++i) {
		scanf(" %c", &op);
		switch (op) {
			case 'f':
				scanf("%d", &v);
				push_front(p_list, create_data_int(v));
				break;
			case 'b':
				scanf("%d", &v);
				push_back(p_list, create_data_int(v));
				break;
			case 'd':
				pop_front(p_list);
				break;
			case 'r':
				reverse(p_list);
				break;
			case 'i':
				scanf("%d", &v);
				insert_in_order(p_list, create_data_int(v));
				break;
			default:
				printf("No such operation: %c\n", op);
				break;
		}
	}
}

int main(void) {
	int to_do, n;
	List list;

	scanf ("%d", &to_do);
	switch (to_do) {
		case 1: // test integer list
			scanf("%d",&n);
			init_list(&list, dump_int, free_int, cmp_int, NULL);
			list_test(&list, n);
			dump_list(&list);
			free_list(&list);
			break;
		case 2: // read words from text, insert into list, and print
			init_list(&list, dump_word, free_word, NULL, NULL);
			stream_to_list(&list, stdin, NULL);
			dump_list(&list);
			free_list(&list);
			break;
		case 3: // read words, insert into list alphabetically, print words encountered n times
			scanf("%d",&n);
			init_list(&list, dump_word_lowercase, free_word, NULL, modify_word);
			stream_to_list(&list, stdin, cmp_word_alphabet);
			list.compare_data = cmp_word_counter;
			DataWord data = { NULL, n };
			dump_list_if(&list,&data);
			free_list(&list);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}

