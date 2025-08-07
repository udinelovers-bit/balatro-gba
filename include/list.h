#ifndef LIST_H
#define LIST_H

#include <stdbool.h>

typedef struct IntList
{
    int* _array;
    int size;
    int allocated_size;
} IntList;

IntList *int_list_new(int init_size);
void int_list_destroy(IntList **list);
bool int_list_add(IntList *list, int value);
bool int_list_remove(IntList *list, int index);
int int_list_get(IntList *list, int index);

#endif
