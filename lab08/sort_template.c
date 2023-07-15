#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

typedef struct {
	int day; 
	int month; 
	int year;
} Date;

// 1 bsearch2

#define FOOD_MAX  30   // max. number of goods
#define ART_MAX   15   // max. length of name's name + 1
#define RECORD_MAX 40  // max. line length

typedef struct {
	char name[20];
	float price;
	int amount;
	Date valid_date;
} Food;

typedef int (*ComparFp)(const void *, const void *);

// compare dates
int cmp_date(const void *d1, const void *d2) {
    const Food *p1 = (const Food *) d1;
    const Food *p2 = (const Food *) d2;

    if (p1->valid_date.year != p2->valid_date.year) {
        return p1->valid_date.year - p2->valid_date.year;
    } else if (p1->valid_date.month != p2->valid_date.month) {
        return p1->valid_date.month - p2->valid_date.month;
    } else {
        return p1->valid_date.day - p2->valid_date.day;
    }
}

// compare foods
int compar(const void *a, const void *b) {
	const Food *pa = (const Food *)a;
    const Food *pb = (const Food *)b;

    
	float k=(pa->price-pb->price);
	if (fabs(k) < 0.01){
		if (cmp_date(pa,pb)==0){
			return strcmp(pa->name, pb->name);

		}else
			return cmp_date(pa,pb);
	}else
		if (pa->price>pb->price)
			return 1;
		else
			return -1;

		
}

// bsearch returning address where to insert new element
void *bsearch2 (const void *key, const void *base, int nmemb, size_t size, ComparFp compar, int *result) {
	const char *p = (const char *)base;
    int l = 0;
	int r = nmemb-1;

    while (l <= r) {
        int mid = (l + r) / 2;
        const char *q = p + mid * size;
        int cmp = compar(key, q);
        if (cmp < 0) {
            r = mid-1;
        } else if (cmp > 0) {
            l = mid + 1;
        } else {
			*result=1;
            return (void *)q;
        }
    }
	*result = 0; 
	const char *q = p + (r+1) * size;
    return (void *)q;
}



// print goods of given name
void print_art(Food *p, int n, char *art) {
	float k=0.0;
	for (int i=0; i<n;i++){
		if (strcmp(p[i].name, art) == 0){
			printf("%.2f %d %02d.%02d.%d\n",p[i].price,p[i].amount,p[i].valid_date.day,p[i].valid_date.month,p[i].valid_date.year);
		}
	}
}

// add record to table if absent
Food* add_record(Food *tab, int *np, ComparFp compar, Food *new) {
	int result;

    Food *existing_record = (Food *)bsearch2(new, tab, *np, sizeof(Food), compar, &result);
    if (result == 1 || compar(new,existing_record)==0) {
        existing_record->amount += new->amount;
        return existing_record;
    } else {
        int index = ((long int)(existing_record) - (long int)(tab))/sizeof(Food);
		for (int i = *np; i > index; i--) {
            tab[i] = tab[i-1];
        }
		
		
		tab[index] = *new;
		(*np)++;
        return &tab[index];
    }
}

// read no lines of data
// calls add_record if sorted = 1 and just adds element if sorted = 0
int read_goods(Food *tab, int no, FILE *stream, int sorted) {
	int day,month,year,amount;
	float value;
	int np=0;
	for (int i=0;i<no;i++){
		Food new;
		fscanf(stream, "%s %f %d %d.%d.%d",new.name,&value,&amount,&day,&month,&year);
		new.price=value;
		new.amount=amount;
		new.valid_date.day=day;
		new.valid_date.month=month;
		new.valid_date.year=year;
		Food *pointer = add_record(tab,&np,compar,&new);	
	}

	return np;
}

int cmp_qs(const void *a, const void *b) {
	Food *fa = (Food*)a, *fb = (Food*)b;
	return cmp_date(&fa->valid_date, &fb->valid_date);
}

int cmp_bs(const void *a, const void *b) {
	Date *pd = (Date*)a;
	Food *fb = (Food*)b;
	return cmp_date(pd, &fb->valid_date);
}

void increase_date(Date *date, int n) {
    struct tm time = {0};
    time.tm_year = date->year - 1900;
    time.tm_mon = date->month - 1;
    time.tm_mday = date->day;
    time_t t = mktime(&time);
    t += n * 24 * 60 * 60;
    struct tm *new_time = localtime(&t);
    date->day = new_time->tm_mday;
    date->month = new_time->tm_mon + 1;
    date->year = new_time->tm_year + 1900;
}

int cmp_date2(const void *p1, const void *p2) {
	const Date *d1 = (const Date *) p1;
    const Date *d2 = (const Date *) p2;

    if (d1->year != d2->year) {
        return d1->year - d2->year;
    } else if (d1->month != d2->month) {
        return d1->month - d2->month;
    } else {
        return d1->day - d2->day;
    }
}

// finds the value of goods due on 'curr_date'
float value(Food *food_tab, size_t n, Date curr_date, int days) {
	float val = 0.0;
	float k=0.0;
	increase_date(&curr_date,days);
	// printf("%d %d %d\n",curr_date.day,curr_date.month,curr_date.year);
	Date *pointer_curr=&curr_date;
	for (int i=0;i<n;i++){
		Date *pointer_tab=&food_tab[i].valid_date;
		int l=cmp_date2(pointer_curr,pointer_tab);
		if (l==0){
			// k=0.0;
			// k+=food_tab[i].amount;
			// printf("%.2f %.2f %02d.%02d.%d\n",food_tab[i].price,k,food_tab[i].valid_date.day,food_tab[i].valid_date.month,food_tab[i].valid_date.year);
			val+=food_tab[i].amount*food_tab[i].price;
		}
	}
	return val;
}

/////////////////////////////////////////////////////////////////
// 3 Succession

#define CHILD_MAX 20

enum Sex {F, M};
enum BOOL {no, yes};

struct Bit_data {
	enum Sex sex:1;
	enum BOOL in_line:1;
};

typedef struct {
	char *name;
	struct Bit_data bits;
	Date born;
	char *parent;
} Person;

typedef struct {
	char *par_name;
	int index;
} Parent;


const Date primo_date = { 28, 10, 2011 }; // new succession act

int cmp_primo(Person *person1, Person *person2) {
	if (person1->bits.sex == person2->bits.sex) return 0;
	if (person1->bits.sex == F && cmp_date(&person1->born, &primo_date) > 0) return 0;
	if (person2->bits.sex == F && cmp_date(&person2->born, &primo_date) > 0) return 0;
	return person2->bits.sex - person1->bits.sex;
}

// compare persons
int cmp_person(const void *a, const void *b) {
}

int fill_indices_tab(Parent *idx_tab, Person *pers_tab, int size) {
}

void persons_shifts(Person *person_tab, int size, Parent *idx_tab, int no_parents) {
}

int cleaning(Person *person_tab, int n) {
}

void print_person(const Person *p) {
	printf("%s\n", p->name);
}

int cmp_date3(const void *d1, const void *d2) {
    const Person *p1 = (const Person *) d1;
    const Person *p2 = (const Person *) d2;

    if (p1->born.year != p2->born.year) {
        return p1->born.year - p2->born.year;
    } else if (p1->born.month != p2->born.month) {
        return p1->born.month - p2->born.month;
    } else {
        return p1->born.day - p2->born.day;
    }
}

int create_list(Person *person_tab, int n) {
	Person tmp;
	int l=1;
	for (int i=0;i<n;i++){
		if (person_tab[i].parent==NULL){
			tmp=person_tab[i];
			person_tab[i]=person_tab[0];
			person_tab[0]=tmp;
			break;
		}
	}
	Person parent;
	Person chosen;
	int flag=0;
	int index;
	int lst_index=l;
	while (lst_index<n){
		parent=person_tab[l-1]; // szumay potomkow l
		flag=0;
		for (int i=lst_index;i<n;i++){
			if (person_tab[i].parent==parent.name){
				if (flag==0){
					flag=1;
					chosen=person_tab[i];
					index=i;
				}
				else{ // comare date
					if (person_tab[i].name!="Anne" && person_tab[i].name!="Louise"){
						Person *p1=&chosen;
						Person *p2=&person_tab[i];
						int cmp=cmp_date3(p1,p2);
						if (cmp>0 || chosen.name=="Anne" || chosen.name=="Louise"){
							chosen=person_tab[i];
							index=i;
						}
					}
				}
			}
		}
		if (flag==1){
			tmp=person_tab[lst_index];
			person_tab[lst_index]=chosen;
			person_tab[index]=tmp;
			
			lst_index+=1;
			l=lst_index;
		}
		else{
			for (int i=0;i<=lst_index;i++){ // jesli nieznaleziono to szukamy rodzca obecnie rozpatrywanego rekordu
				if (person_tab[i].name==parent.parent){
					index=i;
					break;
				}
			}
			l=index+1;

		}
	}
	int cnt=0;
	for (int i=0;i<n;i++){
		if (person_tab[i].bits.in_line==1)
			person_tab[i-cnt]=person_tab[i];
		else
			cnt+=1;
	}
}

int main(void) {
	Person person_tab[] = {
		{"Charles III", {M, no}, {14, 11, 1948},"Elizabeth II"},
		{"Anne", {F,yes}, {15, 8, 1950},"Elizabeth II"},
		{"Andrew", {M,yes}, {19, 2, 1960},"Elizabeth II"},
		{"Edward", {M,yes}, {10, 3, 1964} ,"Elizabeth II"},
		{"David", {M,yes}, {3, 11, 1961} ,"Margaret"},
		{"Sarah", {F,yes}, {1, 5, 1964}, "Margaret"},
		{"William", {M,yes}, {21, 6, 1982}, "Charles III"},
		{"Henry", {M,yes}, {15, 9, 1984}, "Charles III"},
		{"Peter", {M,yes}, {15, 11, 1977}, "Anne"},
		{"Zara", {F,yes}, {15, 5, 1981}, "Anne"},
		{"Beatrice", {F,yes}, {8, 8, 1988}, "Andrew"},
		{"Eugenie", {F,yes}, {23, 3, 1990}, "Andrew"},
		{"James", {M,yes}, {17, 12, 2007}, "Edward"},
		{"Louise", {F,yes}, {8, 11, 2003}, "Edward"},
		{"Charles", {M,yes}, {1, 7, 1999}, "David"},
		{"Margarita", {F,yes}, {14, 5, 2002}, "David"},
		{"Samuel", {M,yes}, {28, 7, 1996}, "Sarah"},
		{"Arthur", {M,yes}, {6, 5, 2019}, "Sarah"},
		{"George", {M,yes}, {22, 7, 2013}, "William"},
		{"George VI", {M,no}, {14, 12, 1895}, NULL},
		{"Charlotte", {F,yes}, {2, 5, 2015}, "William"},
		{"Louis", {M,yes}, {23, 4, 2018}, "William"},
		{"Archie", {M,yes}, {6, 5, 2019}, "Henry"},
		{"Lilibet", {F,yes}, {4, 6, 2021}, "Henry"},
		{"Savannah", {F,yes}, {29, 12, 2010}, "Peter"},
		{"Isla", {F,yes}, {29, 3, 2012}, "Peter"},
		{"Mia", {F,yes}, {17, 1, 2014}, "Zara"},
		{"Lena", {F,yes}, {18, 6, 2018}, "Zara"},
		{"Elizabeth II", {F,no}, {21, 4, 1925}, "George VI"},
		{"Margaret", {F,no}, {21, 8, 1930}, "George VI"},
		{"Lucas", {M,yes}, {21, 3, 2021}, "Zara"},
		{"Sienna", {F,yes}, {18, 9, 2021}, "Beatrice"},
		{"August", {M,yes}, {9, 2, 2021}, "Eugenie"}
	};

	int to_do, no;
	int n;
	Food food_tab[FOOD_MAX];
	char buff[ART_MAX];
	fgets(buff, ART_MAX, stdin);
	sscanf(buff, "%d", &to_do);

	switch (to_do) {
		case 1:  // bsearch2
			fgets(buff, ART_MAX, stdin);
			sscanf(buff, "%d", &no);
			n = read_goods(food_tab, no, stdin, 1);
			scanf("%s",buff);
			print_art(food_tab, n, buff);
			break;
		case 2: // qsort
			fgets(buff, ART_MAX, stdin);
			sscanf(buff, "%d", &no);
			n = read_goods(food_tab, no, stdin, 0);
			Date curr_date;
			int days;
			scanf("%d %d %d", &curr_date.day, &curr_date.month, &curr_date.year);
			scanf("%d", &days);
			printf("%.2f\n", value(food_tab, (size_t)n, curr_date, days));
			break;
		case 3: // succession
			scanf("%d",&no);
			int no_persons = sizeof(person_tab) / sizeof(Person);
			create_list(person_tab,no_persons);
			// for (int i=0;i<no_persons;i++)
			// 	print_person(person_tab + i+1 - 1);
			
			print_person(person_tab + no - 1 );
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
	}
	return 0;
}
