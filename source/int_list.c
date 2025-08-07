#include <stdlib.h>
#include <stdbool.h>
#include "list.h"

IntList *int_list_new(int init_size) {
    IntList *list = (IntList *)malloc(sizeof(IntList));
    if (list == NULL) return NULL;
    list->_array = (int *)malloc(sizeof(int) * init_size);
    if (!list->_array) 
    {
        free(list);
        return NULL;
    }
    list->size = 0;
    list->allocated_size = init_size;
    return list;
}

void int_list_destroy(IntList **list) {
    if (list == NULL || *list == NULL)
        return; 
    {
        free((*list)->_array);
        free(*list);
    }

    *list = NULL;
}

bool int_list_add(IntList *list, int value) 
{
    if (list->size >= list->allocated_size) 
    {
        int new_size = list->allocated_size * 2;
        int *new_arr = (int *)realloc(list->_array, sizeof(int) * new_size);
        if (new_arr == NULL) 
            return false;
        list->_array = new_arr;
        list->allocated_size = new_size;
    }
    list->_array[list->size++] = value;
    return true;
}

bool int_list_remove(IntList *list, int index) {
    if (index < 0 || index >= list->size) 
        return false;
    for (int i = index; i < list->size - 1; ++i) 
    {
        list->_array[i] = list->_array[i + 1];
    }
    list->size--;
    return true;
}

int int_list_get(IntList *list, int index) 
{
    if (index < 0 || index >= list->size) 
        return 0;
    return list->_array[index];
}