/* id_library.h
	Author: Kyle Ebding

The goal is to write an efficient “ID” library.  Given a binary byte string of arbitrary length up
to 4K, generate a 32 bit “ID” for it. If asked again for the same byte string, you should always
return the same ID. Once an ID is allocated, you can query for the string value given the ID. If the
ID is freed it should not be reused immediately for the next request.
*/

typedef struct table_struct table_struct;

/* create_table
   This function creates a hash table to use with this library.
PARAMS:
    length: the length of the table, i.e. the capacity
RETURN: a pointer to the table_struct created by the function.
*/
table_struct* create_table(int length);

/* destroy_table
   This function destroys the input table by freeing the memory associated with it.
PARAMS: 
    table: the table to be destroyed
*/
void destroy_table(table_struct* table);

/* assign_id
   This function takes an input byte string and stores it in a table indexed by the hash of the
   input.
PARAMS: 
    input: the byte string to be stored and indexed. NOTE: must be 4096 in length because this
        library does not check bounds on inputs.
    input_length: the number of bytes in the input
RETURN: the index the input was mapped to
*/   
uint32_t assign_id (table_struct* table, void* input, int length);

/* query_id
   This function takes an input ID and buffer and fills the buffer with the string mapped to by the
   input ID, if there is one. If the index is empty, it will return the input buffer unmodified.
PARAMS: 
    query: the ID of the string to be found
    output: the buffer to be filled with the resultant string. NOTE: must be at least as long as the
        data mapped to the ID to avoid buffer overflows. If you are unsure how long the data is, use
        a buffer of 4096 bytes.
RETURN: 1 for success, 0 for error
*/
int query_id (table_struct* table, uint32_t query, void* output);

/* free_id
   This function removes an entry from the table, freeing the ID to be used again later by another
   string that hashes to the same ID.
PARAMS:
    id: the ID to remove from the table
RETURN: 1 for success, 0 for error
*/
int free_id (table_struct* table, uint32_t id);
