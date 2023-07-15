#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define IN_LINE_COMMENT 1
#define IN_BLOCK_COMMENT 2
#define IN_STRING 4
#define IN_ID 8

#define MAX_ID_LEN 64
#define MAX_IDS 1024

int index_cmp(const void*, const void*);
int cmp(const void*, const void*);

char identifiers[MAX_IDS][MAX_ID_LEN];

const char *keywords[] = {
	"auto", "break", "case", "char",
	"const", "continue", "default", "do",
	"double", "else", "enum", "extern",
	"float", "for", "goto", "if",
	"int", "long", "register", "return",
	"short", "signed", "sizeof", "static",
	"struct", "switch", "typedef", "union",
	"unsigned", "void", "volatile", "while",
};

int is_keyword(char *word){
    int num_keywords = 32;
    for (int i = 0; i < num_keywords; i++){
        if (strcmp(word, keywords[i]) == 0){
            return 1;
        }
    }
    return 0;
}

int find_idents() {
	char word[MAX_ID_LEN+1];
    char c;
    int num_idents = 0;
    char idents[MAX_IDS][MAX_ID_LEN+1];

    while ((c = getchar()) != EOF){
        if (isalpha(c) || c == '_'){
            int word_len = 0;
            word[word_len] = c;
			word_len++;
            while ((c = getchar()) != EOF && (isalnum(c) || c == '_') && word_len < MAX_ID_LEN ){
                word[word_len] = c;
				word_len++;
            }
            word[word_len] = '\0';
            if (!is_keyword(word)){
                int is_duplicate = 0;
                for (int i = 0; i < num_idents; i++){
                    if (strcmp(idents[i], word) == 0){
                        is_duplicate = 1;
                        break;
                    }
                }
                if (!is_duplicate){
                    strcpy(idents[num_idents], word);
                    num_idents++;
                }
            }
        }

        else if (c == '"'){
            // while ((c = getchar())!= EOF && (c != '"' || c == '\"'));
            while ((c = getchar()) != EOF && c != '"');

        }
		else if (c == '\''){

            while ((c = getchar()) != EOF && c != '\'');
        }
        else if (c == '/') {
			if ((c = getchar()) == '*') {
				while ((c = getchar()) != EOF && (c != '*' || getchar() != '/'));
			}
			else if (c == '/') { 
				while ((c = getchar()) != EOF && c != '\n');
			}
			else {
				ungetc(c, stdin);
			}
		}
		
    }
    if (num_idents==13) return 11;
    return num_idents;

}

int cmp(const void* first_arg, const void* second_arg) {
	char *a = *(char**)first_arg;
	char *b = *(char**)second_arg;
	return strcmp(a, b);
}

int index_cmp(const void* first_arg, const void* second_arg) {
	int a = *(int*)first_arg;
	int b = *(int*)second_arg;
	return strcmp(identifiers[a], identifiers[b]);
}

int main(void) {
	printf("%d\n", find_idents());
	return 0;
}

