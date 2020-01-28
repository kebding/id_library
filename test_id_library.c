/* test_id_library.c
   This file tests to verify that id_library works as intended.
   Author: Kyle Ebding
*/

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "id_library.h"

int main(int argc, char* argv[]) {
    table_struct* T = create_table(16);
    if (T == NULL) {
        printf("FAILED: create_table()\n");
        exit(-1);
    }
    else {
        printf("PASSED: create_table()\n");
    }

    char* values_array[] = {"foo", "bar", "blarg"};
    // the following values were pre-computed as the hashes of the elements of values_array
    uint32_t hash_array[] = {5, 6, 7};

    // check if assign_id() works
    int i;
    uint32_t ids[8];
    for (i = 0; i < 3; i++) {
        ids[i] = assign_id(T, values_array[i], strlen(values_array[i]));
    }

    // check if there are valid IDs stored
    int success = 1;
    for (i = 0; i < 3; i++) {
        if (ids[i] != hash_array[i]) {
            success = 0;
            break;
        }
    }
    if (success) {
        printf("PASSED: simple assign_id()\n");
    }
    else {
        printf("FAILED: simple assign_id()\n");
    }

    // check if collisions are assigned correctly
    char collision_str[] = "oof"; // has same hash value as "foo" (5)
    int oof_rehash = 4; // pre-calculated: hash("5")=4
    ids[4] = assign_id(T, collision_str, strlen(collision_str));
    if (ids[4] != oof_rehash) {
        printf("FAILED: collision assign_id()\n");
    }
    else {
        printf("PASSED: collision assign_id()\n");
    }

    // check if IDs can be queried correctly
    char query_result[16] = {'\0'};
    for (i = 0; i < 3; i++) {
        success = query_id(T, ids[i], query_result);
        if (!success || strcmp(query_result, values_array[i]) != 0) {
            success = 0;
            break;
        }
        memset(query_result, '\0', sizeof(query_result));  // clear the buffer
    }
    if (success) {
        printf("PASSED: query_id()\n");
    }
    else {
        printf("FAILED: query_id()\n");
    }
    
    // check if the IDs can be freed correctly
    success = 1;
    for (i = 0; i < 3; i++) {
        success = free_id(T, ids[i]);
        if (!success) break;
        success = query_id(T, ids[i], query_result);
        if (success || strcmp(query_result, values_array[i]) == 0) {
            success = 0;
            break;
        }
        else success = 1;
        memset(query_result, '\0', sizeof(query_result));  // clear the buffer
    }
    if (success) {
        printf("PASSED: free_id()\n");
    }
    else {
        printf("FAILED: free_id()\n");
    }


    // destroy the table to free heap memory
    destroy_table(T);
    /* I could not find a good way to check if the table was successfully destroyed outside of using
       gdb. The final line of destroy_table() does not set the caller's pointer to NULL, so I cannot
       simply check whether (T == NULL), and because I've free()'d the table I cannot check any of
       its previously-existing members. However, testing with valgrind showed there are no memory
       leaks.
    */

}
