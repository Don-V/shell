#ifndef LIST_H
#define LIST_H

#include <stdbool.h>

typedef struct Node {
  void* data;
  struct Node* next;

} Node;

typedef struct List {
  Node* front;
  Node* rear;
  bool (*equals)(const void* data1, const void* data2);
  int size;
  Node* _iter;
} List;

/**
 * Initializes list with function pointers, and other front and rear pointers
 * \param l list
 * \param equals comparator function pointer
 *        \param data1 the data stored in comparison
 *        \param data2 the data being compared with
 */
void list_init(List* l, bool (*equals)(const void* data1, const void* data2));

/**
 * Add data to list
 * \param l the list
 * \param data data being added to the list
 */
void enqueue(List* l, void* data);

/**
 * Search list for data
 * Uses equals function pointer, data is passed as the second argument
 * \param l list
 * \param query data being searched
 * \return the data if found else 0
 */
void* list_search(List* l, void* query);

/**
 * Remove data from list and reset the iterator
 * Uses equals function pointer, data is passed as the second argument
 * \param l list
 * \param data to be removed
 * \return the data removed or 0 if not found
 */
void* list_remove(List* l, void* data);

void empty_list(List* l);

/**
 * Iterates through the list and returns the next member
 * starting from the front and ending at that tail
 * Iterator is reset if an element is removed
 * \param l the list being iterated
 * \param start starts iteration from the head again
 */
void* iterate(List* l, bool start);

#endif