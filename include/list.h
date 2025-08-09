#ifndef LIST_H
#define LIST_H

#include <stdbool.h>

/* Defining an int list for simplicity
 * If another list is needed, a generic void*
 * list should be defined....
 */
typedef struct IntList
{
    int* _array;
    int size;
    int allocated_size;
} IntList;

IntList *int_list_new(int init_size);
void int_list_destroy(IntList **list);
bool int_list_append(IntList *list, int value);
bool int_list_remove_by_idx(IntList *list, int index);
int int_list_get(IntList *list, int index);
int int_list_get_size(IntList *list);
bool int_list_remove_by_value(IntList *list, int value);

#endif
