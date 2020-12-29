#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Start of our Code
typedef struct SinglyLinkedListNode SinglyLinkedListNode;
typedef struct SinglyLinkedList SinglyLinkedList;


char* readline();
char** split_string(char*);
int search_for_value(SinglyLinkedList** singly_linked_list, char* word);
void node_copy(SinglyLinkedListNode* node,SinglyLinkedListNode* node_original);
void sort_table(SinglyLinkedListNode** table, int size);
void node_copy(SinglyLinkedListNode* node,SinglyLinkedListNode* node_original);
void swap_values(SinglyLinkedListNode** table, int i, int j);
void free_singly_linked_list(SinglyLinkedList* list);
void free_table(SinglyLinkedListNode** table, int size);
void create_table(SinglyLinkedList** magazine, SinglyLinkedList** note, SinglyLinkedListNode** magazine_table, SinglyLinkedListNode** note_table, int m_unique_words, int n_unique_words);


struct SinglyLinkedList {
	SinglyLinkedListNode* head;
	SinglyLinkedListNode* tail;
	int amount_of_words;
};

struct SinglyLinkedListNode {
	char* data;
	int amount;
	SinglyLinkedListNode* next;
};


// Create the object node with values
SinglyLinkedListNode* create_singly_linked_list_node(char* node_data) {
	SinglyLinkedListNode* node = malloc(sizeof(SinglyLinkedListNode));
	while (!node)
		node = malloc(sizeof(SinglyLinkedListNode));
	node->data = node_data;
	node->amount = 1;
	node->next = NULL;
	return node;
}

//Adding the node to the list
void insert_node_into_singly_linked_list(SinglyLinkedList** singly_linked_list,
 char* node_data) {
	if (!node_data)
		return; /* Invalid arguments */
	// Serching for the data
	if(search_for_value(singly_linked_list, node_data))
		return;
	else{	// If the data doesn't exist
		SinglyLinkedListNode* node = create_singly_linked_list_node(node_data);
		if (!(*singly_linked_list)->head) {
			(*singly_linked_list)->head = node;
			(*singly_linked_list)->amount_of_words = 1;
		} else {
			//we can add here that the update will use the ordering
			
			(*singly_linked_list)->tail->next = node;
			(*singly_linked_list)->amount_of_words += 1;
		}
		(*singly_linked_list)->tail = node;
	}
	
}

//searching on all the list - can change later
int search_for_value(SinglyLinkedList** singly_linked_list, char* word){
	struct SinglyLinkedListNode* head = (*singly_linked_list)->head;
	int N = (*singly_linked_list)->amount_of_words;
	for(int i=0; i<N;i++)
	{
		if(!strcmp((head->data),word)){
			head->amount +=1;
			return 1;
		}
		head = head->next;
	}
	return 0;
}


// Complete the checkMagazine function below.
void check_magazine(int magazine_count, char** magazine, int note_count, char** note) {
	int i, j, m_unique_words, n_unique_words, result;
	
	// tables so the we will have the list ordered by value
	SinglyLinkedListNode** magazine_table;
	SinglyLinkedListNode** note_table;
	
	// Allocating memory for wraping struct
	SinglyLinkedList* linked_list_magazine = malloc(sizeof(SinglyLinkedList));
	SinglyLinkedList* linked_list_note = malloc(sizeof(SinglyLinkedList));
	
	//creating the unique lists
	for(i=0;i<magazine_count;i++)	//creating list for magazine
		insert_node_into_singly_linked_list((&linked_list_magazine), magazine[i]);
	for(i=0;i<note_count;i++)		//creating list for note
		insert_node_into_singly_linked_list((&linked_list_note), note[i]);
	
	//number of unique words in each list
	m_unique_words = (linked_list_magazine->amount_of_words);
	n_unique_words = (linked_list_note->amount_of_words);
	if(m_unique_words<n_unique_words){
		free_singly_linked_list(linked_list_magazine); // free nodes
		free_singly_linked_list(linked_list_note); // free nodes
		printf("No");
		return;
	}
	
	// We will use the "next" pointer in the struct to point to the original node in the list
	magazine_table = malloc(sizeof(SinglyLinkedListNode*)*m_unique_words);
	for(i=0;i<m_unique_words;i++)
		magazine_table[i] = malloc(sizeof(SinglyLinkedListNode));
	
	note_table = malloc(sizeof(SinglyLinkedListNode*)*n_unique_words);
	for(i=0;i<n_unique_words;i++)
		note_table[i] = malloc(sizeof(SinglyLinkedListNode));
	
	create_table((&linked_list_magazine), (&linked_list_note), magazine_table, note_table, m_unique_words, n_unique_words);
	
	// Sorting the tables
	sort_table(magazine_table, m_unique_words);
	sort_table(note_table, n_unique_words);
	
	j=0;
	for(i=0;i<m_unique_words;i++){
		if(j == n_unique_words)
			break; // We succesfuly finished all of the note's words
		result = strcmp(magazine_table[i]->data, note_table[j]->data);
		if(result > 0) 
			continue; //skip until they are mathced
		if(result == 0){
			if((magazine_table[i]->amount) < (note_table[i]->amount)){
				free_singly_linked_list(linked_list_magazine); 	//free wrapper and values
				free_singly_linked_list(linked_list_note);		//free wrapper and values
				free_table(magazine_table, m_unique_words);		//free table and values
				free_table(note_table, n_unique_words);			//free table and values
				printf("No");
				return;
			}
			j++;
		}
		else{ // We get here if the word in note_table[j] isn't in the magazine
			free_singly_linked_list(linked_list_magazine); 	//free wrapper and values
			free_singly_linked_list(linked_list_note);		//free wrapper and values
			free_table(magazine_table, m_unique_words);		//free table and values
			free_table(note_table, n_unique_words);			//free table and values
			printf("No");
			return;
		}
	}
	
	free_singly_linked_list(linked_list_magazine);
	free_singly_linked_list(linked_list_note);
	free_table(magazine_table, m_unique_words);
	free_table(note_table, n_unique_words);
	printf("Yes");
}

// After we know the amount of unique words we create a table;
void create_table(SinglyLinkedList** magazine, SinglyLinkedList** note, SinglyLinkedListNode** magazine_table, SinglyLinkedListNode** note_table, int m_unique_words, int n_unique_words){
	SinglyLinkedListNode* temp_magazine = (*magazine)->head;
	SinglyLinkedListNode* temp_note = (*note)->head;
	
	//Creating tables for sort
	for(int i=0; i<m_unique_words;i++){
		magazine_table[i]->data = temp_magazine->data;
		magazine_table[i]->amount = temp_magazine->amount;
		magazine_table[i]->next = temp_magazine;
		temp_magazine = temp_magazine->next;
	}
	for(int i=0; i<n_unique_words;i++){
		note_table[i]->data = temp_note->data;
		note_table[i]->amount = temp_note->amount;
		note_table[i]->next = temp_note;
		temp_note = temp_note->next;
	}
}

// sort the tables by the string values
void sort_table(SinglyLinkedListNode** table, int size){
	int i,j;
	// Ordering from the smallest ascii value to the greatest - max sort
	for(i=0;i<size;i++)
		for(j=0;j<size;j++)
			if(strcmp(table[i]->data, table[j]->data)>0)
				swap_values(table, i, j);
}

// deep copy of a node to another one
void node_copy(SinglyLinkedListNode* node,SinglyLinkedListNode* node_original){
	node->data = node_original->data;
	node->amount = node_original->amount;
	node->next = node_original->next;
}

// Swap 2 values in the table
void swap_values(SinglyLinkedListNode** table, int i, int j){
	SinglyLinkedListNode* temp = malloc(sizeof(SinglyLinkedListNode));
	node_copy(temp,table[i]);
	node_copy(table[i], table[j]);
	node_copy(table[j],temp);
	free(temp);
}

// free the list and the list wrapper
void free_singly_linked_list(SinglyLinkedList* list) {
	SinglyLinkedListNode* node = list->head;
	while (node) {
		SinglyLinkedListNode* temp = node;
		node = node->next;
		free(temp);
	}
	free(list);
}

// free table
void free_table(SinglyLinkedListNode** table, int size) {
	for(int i=0;i<size;i++)
		free(table[i]);
	free(table);
}


// End of our code



int main()
{
    char** mn = split_string(readline());	

    char* m_endptr;
    char* m_str = mn[0];	//number of words in the paper
    int m = strtol(m_str, &m_endptr, 10);

    if (m_endptr == m_str || *m_endptr != '\0') { exit(EXIT_FAILURE); }

    char* n_endptr;
    char* n_str = mn[1];	//number of words in the note
    int n = strtol(n_str, &n_endptr, 10);

    if (n_endptr == n_str || *n_endptr != '\0') { exit(EXIT_FAILURE); }

    char** magazine_temp = split_string(readline());	//reading paper words

    char** magazine = malloc(m * sizeof(char*));	//allocating m pointers to the string of each word

    for (int i = 0; i < m; i++) {
        char* magazine_item = *(magazine_temp + i);

        *(magazine + i) = magazine_item;
    }

    int magazine_count = m;

    char** note_temp = split_string(readline());	//reading note words

    char** note = malloc(n * sizeof(char*));	//allocating n pointers to the string og each word

    for (int i = 0; i < n; i++) {
        char* note_item = *(note_temp + i);

        *(note + i) = note_item;
    }

    int note_count = n;

    check_magazine(magazine_count, magazine, note_count, note);

    return 0;
}

// Free data?
char* readline() {
    size_t alloc_length = 1024;
    size_t data_length = 0;
    char* data = malloc(alloc_length);

    while (true) {
        char* cursor = data + data_length;	//where the data is stored
        char* line = fgets(cursor, alloc_length - data_length, stdin); //mid var is for the length of the array passed as str is used. 

        if (!line) { //Not getting any input
            break;
        }

        data_length += strlen(cursor);

        if (data_length < alloc_length - 1 || data[data_length - 1] == '\n') {
            break;
        }

        alloc_length <<= 1; // shrinking to 512, 256, 128, 64, 32, 16, 8

        data = realloc(data, alloc_length);

        if (!line) { // Don't know what this line do (why it repeats)
            break;
        }
    }

    if (data[data_length - 1] == '\n') {
        data[data_length - 1] = '\0';

        data = realloc(data, data_length);
    } else {
        data = realloc(data, data_length + 1);

        data[data_length] = '\0';
    }

    return data;
}


// Take the string and devide it by " " to sub string. As long as it can still devide the original
// string' it continues and each time allocate a new memory for the strings up to now and adding
// the last string.
char** split_string(char* str) {
    char** splits = NULL;
    char* token = strtok(str, " ");

    int spaces = 0;

    while (token) {
        splits = realloc(splits, sizeof(char*) * ++spaces); // need to check if it free the previous memory

        if (!splits) {
            return splits;
        }

        splits[spaces - 1] = token;

        token = strtok(NULL, " ");
    }

    return splits;
}
