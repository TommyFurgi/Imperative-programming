#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>


#define MAX_LINE 128
// consider chars from [FIRST_CHAR, LAST_CHAR)
#define FIRST_CHAR 33
#define LAST_CHAR 127
#define MAX_CHARS (LAST_CHAR - FIRST_CHAR)
#define MAX_BIGRAMS ((LAST_CHAR - FIRST_CHAR) * (LAST_CHAR - FIRST_CHAR))

#define NEWLINE '\n'
#define IN_WORD 1

#define IN_LINE_COMMENT 1
#define IN_BLOCK_COMMENT 2

int count[MAX_BIGRAMS] = { 0 };

// sort indices according to their respective counts.
// sort alphabetically if counts equal
int cmp (const void *a, const void *b) {
	int va = *(int*)a;
	int vb = *(int*)b;
	if (count[va] == count[vb]) return va - vb;	return count[vb] - count[va];
}

// sort indices according to their respective counts.
// sort alphabetically if counts equal
int cmp_di (const void *a, const void *b) {
	int va = *(int*)a;
	int vb = *(int*)b;
	// sort according to second char if counts and the first char equal
	if (count[va] == count[vb] && va / MAX_CHARS == vb / MAX_CHARS) return va % MAX_CHARS - vb % MAX_CHARS;
	// sort according to first char if counts equal
	if (count[va] == count[vb]) return va / MAX_CHARS - vb / MAX_CHARS;
	return count[vb] - count[va];
}

// count lines, words & chars in a given text file
void wc(int *nl, int *nw, int *nc) {
	*nl=0;
	*nw=0;
	*nc=0;
	int i=0;
	char buffer[MAX_LINE];
	while (fgets(buffer, MAX_LINE, stdin) != NULL) {
		*nl+=1;

		for(i=0; i<MAX_LINE; i++){
			if (buffer[i] == '\0') break;
			if ((i==0) && (buffer[i]!=' ') && (buffer[i]!='\n')) *nw+=1;
			if ((buffer[i]==' ') && (buffer[i+1]!=' ') && (buffer[i+1]!='\0')) *nw+=1;
			*nc+=1;
		}
	}
}

void swap (int array[], int i, int k) {
	int tmp=array[i];
	array[i]=array[k];
	array[k]=tmp;
}

void char_count(int char_no, int *n_char, int *cnt) {
	int c[LAST_CHAR-FIRST_CHAR]={0};
	char buffer[MAX_LINE];
	while (fgets(buffer, MAX_LINE, stdin) != NULL) {
		for(int i=0; i<MAX_LINE; i ++){
			if (buffer[i] == '\0') break;
			c[(int)(buffer[i]) -FIRST_CHAR]+=1;
			
		}
	}

	int char_indices[LAST_CHAR - FIRST_CHAR];
    for (int i=0; i<(LAST_CHAR - FIRST_CHAR); i++) {
        char_indices[i] = i;
    }
	int n=LAST_CHAR-FIRST_CHAR;
	int flag=0;
	for(int i=0;i<n;i++){
		flag=0;
		for(int j=0;j<n-i-1;j++){
			if (c[j]<c[j+1]){
				swap(c,j,j+1);
				swap(char_indices,j,j+1);
				flag=1;
			}
		}
		if (flag==0){
			break;
		}
	}
	*cnt=0;

	*cnt=c[char_no-1];
	*n_char=(char)(char_indices[char_no-1]+FIRST_CHAR);
}

void bigram_count(int bigram_no, int bigram[]) {
	int n=MAX_BIGRAMS;
	int c[MAX_BIGRAMS]={0};
	char buffer[MAX_LINE];
	while (fgets(buffer, MAX_LINE, stdin) != NULL) {
		if (buffer[0] == '\0') break;
		for(int i=1; i<MAX_LINE; i ++){
			if (buffer[i] == '\0') break;
			if (((int)(buffer[i-1])>=FIRST_CHAR && (int)(buffer[i-1])<LAST_CHAR) && ((int)(buffer[i])>=FIRST_CHAR && (int)(buffer[i])<LAST_CHAR)){
				c[((int)(buffer[i]) -FIRST_CHAR)+((int)(buffer[i-1]) -FIRST_CHAR)*MAX_CHARS]+=1;
			}
			
		}
	}
	int char_indices[MAX_BIGRAMS];
    for (int i=0; i<(MAX_BIGRAMS); i++) {
        char_indices[i] = i;
	}
	int flag=0;
	for(int i=0;i<n;i++){
		flag=0;
		for(int j=0;j<n-i-1;j++){
			if (c[j]<c[j+1]){
				swap(c,j,j+1);
				swap(char_indices,j,j+1);
				flag=1;
			}
		}
		if (flag==0){
			break;
		}
	}

	bigram[2]=c[bigram_no-1];
	bigram[1]=(char)(char_indices[bigram_no-1]%MAX_CHARS +FIRST_CHAR);
	int tmp=(int)(char_indices[bigram_no-1]/MAX_CHARS +FIRST_CHAR);
	bigram[0]=(char)(tmp);

}


void find_comments(int *line_comment_counter, int *block_comment_counter) {
	*line_comment_counter=0;
	*block_comment_counter=0;
	char buffer[MAX_LINE];
	int coment=0;
	while (fgets(buffer, MAX_LINE, stdin) != NULL) {
		for(int i=0; i<MAX_LINE; i++){
			if (buffer[i] == '\0') break;
			if (coment==1 && i>0){
				if (buffer[i-1] == '\0') break;
			}
			if ((buffer[i]=='/') && (buffer[i+1]=='*') && (coment==0)){
				*block_comment_counter+=1;
				i++;
				coment=1;
			}
			else if ((buffer[i]=='*') && (buffer[i+1]=='/') && (coment==1)){
				i++;
				coment=0;
			}
			else if ((buffer[i]=='/') && (buffer[i+1]=='/') && (coment==0)){
				*line_comment_counter+=1;
				break;
			}
			
		}
	} 

}

int read_int() {
	char line[MAX_LINE];
	fgets(line, MAX_LINE, stdin); // to get the whole line
	return (int)strtol(line, NULL, 10);
}

int main(void) {
	int to_do;
	int nl, nw, nc, char_no, n_char, cnt;
	int line_comment_counter, block_comment_counter;
	int bigram[3];

	to_do = read_int();
	switch (to_do) {
		case 1: // wc()
			wc (&nl, &nw, &nc);
			printf("%d %d %d\n", nl, nw, nc);
			break;
		case 2: // char_count()
			char_no = read_int();
			char_count(char_no, &n_char, &cnt);
			printf("%c %d\n", n_char, cnt);
			break;
		case 3: // bigram_count()
			char_no = read_int();
			bigram_count(char_no, bigram);
			printf("%c%c %d\n", bigram[0], bigram[1], bigram[2]);
			break;
		case 4:
			find_comments(&line_comment_counter, &block_comment_counter);
			printf("%d %d\n", block_comment_counter, line_comment_counter);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}

