/* id_library.c
	Author: Kyle Ebding

The goal is to write an efficient “ID” library.  Given a binary byte string of arbitrary length up
to 4K, generate a 32 bit “ID” for it. If asked again for the same byte string, you should always
return the same ID. Once an ID is allocated, you can query for the string value given the ID. If the
ID is freed it should not be reused immediately for the next request.
*/

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#define MAX_ENTRY_LEN 4096
#define HASH_SEED 31  // arbitrary prime seed
#define HASH_MULTIPLIER_1 127  // another arbitrary prime
#define HASH_MULTIPLIER_2 257  // another arbitrary prime


/****** private functions and types ******/
/* struct entry_struct
   This struct contains an individual entry in the hash table, which consists of the entry itself 
   and an integer storing the length of the entry. The length can be no greater than 4096.
*/
typedef struct entry_struct {
    char data[MAX_ENTRY_LEN];
    int entry_length;
} entry_struct;

/* struct table_struct
   This struct contains an array of entry_structs and an integer storing the length of the table.
*/
typedef struct table_struct {
    int table_length;
    entry_struct* entries;
} table_struct;

/* hash
   This function takes an input string and returns a hash of the input.
PARAMS:
    input: the string to be hashed
RETURN: the index returned by the hash function
NOTES: not original code. This code is modified from a funtion found online at 
https://stackoverflow.com/questions/8317508/hash-function-for-a-string
*/
uint32_t hash (char* input, int table_len, int input_length) {
    unsigned long hash = HASH_SEED;  
    for (int i = 0; i < input_length; i++) {
	hash = (hash * HASH_MULTIPLIER_1) ^ ((uint8_t)input[i] * HASH_MULTIPLIER_2);
    }
    return (uint32_t) hash % table_len; 
}

/* no_collision
   This function determines if there is a collision in the hash table, i.e. if the input index
   already has a value that does not match the input value. 
PARAMS:
    index: the index in the table to check
    value: the byte string to compare
RETURN: 1 for non-collision, 0 for collision or invalid index
*/
int no_collision (table_struct* table, int index, char* value, int value_length) {
    // first, ensure the index is valid
    if (index < 0 || index >= table->table_length)
        return 0;

    int retVal, compare_length;
    if (table->entries[index].entry_length < value_length) { 
	compare_length = table->entries[index].entry_length;
    }
    else {
	compare_length = value_length;
    }
    
    if (table->entries[index].entry_length != 0 &&
            memcmp(table->entries[index].data, value, compare_length) != 0) {
        retVal = 0;
    }
    else {
        retVal = 1;
    }
    return retVal;
}


/****** public functions ******/

table_struct* create_table(int length) {
    table_struct* table = malloc(sizeof(table_struct*));
    assert(table != NULL);
    table->table_length = length;
    table->entries = calloc(length, sizeof(entry_struct));
    char* empty_string = calloc(MAX_ENTRY_LEN, sizeof(char));
    for (int i = 0; i < length; i++) {
        table->entries[i].entry_length = 0;
	memcpy(table->entries[i].data, empty_string, MAX_ENTRY_LEN);
    }
    free(empty_string);
    return table;
}

void destroy_table(table_struct* table) {
    free(table->entries);
    table->entries = NULL;
    free(table);
    table = NULL;
}

uint32_t assign_id (table_struct* table, char* input, int length) {
    uint32_t index = hash(input, table->table_length, length);
    // if there is a collision, rehash the index to try find an open index
    for (int i = 0; i < table->table_length; i++) {
        if (no_collision(table, index, input, length)) {
            memcpy(table->entries[index].data, input, length);
            table->entries[index].entry_length = length;
            return index;
        }
        else {
	    char index_str[32] = {'\0'};
	    sprintf(index_str, "%d", index);
            index = hash(index_str, table->table_length, strlen(index_str));
            // now try again by restarting the loop
        }
    }
    // if the code reaches this point, we failed to insert into the table.
    return -1;
}

int query_id (table_struct* table, uint32_t index, char* output) {
    if (index < 0 || index >= table->table_length || table->entries[index].entry_length == 0) {
        return 0;
    }
    else {
        memcpy(output, table->entries[index].data, table->entries[index].entry_length);
    }
    return 1;
}

int free_id (table_struct* table, uint32_t index) {
    if (index < 0 || index >= table->table_length || table->entries[index].entry_length == 0) {
        return 0;
    }
    else {
        char* empty_string = calloc(MAX_ENTRY_LEN, sizeof(char));
        assert(empty_string != NULL);
        memcpy(table->entries[index].data, empty_string, MAX_ENTRY_LEN);
        table->entries[index].entry_length = 0;
        free(empty_string);
    }
    return 1;
}

