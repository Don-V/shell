#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "list.h"

/*Initializes list with function pointers, and other front and rear pointers*/
void list_init(List* l, bool (*equals)(const void* data1, const void* data2)) {
  l->front = 0;
  l->rear = 0;
  l->size = 0;
  l->equals = equals;
}

/* Add to queue without priority (linear queue)*/
void enqueue(List* l, void* data) {
  struct Node* temp = (struct Node*)malloc(sizeof(struct Node));
  temp->data = data;
  temp->next = NULL;
  l->size++;
  if (l->front == NULL && l->rear == NULL) {
    l->front = l->rear = temp;
    return;
  }
  l->rear->next = temp;
  l->rear = temp;
}

/* Removes node with highest priority*/
void dequeue(List* l) {
  struct Node* temp = l->front;
  if (l->front == NULL) return;
  if (l->front == l->rear) {
    l->front = l->rear = NULL;
  } else {
    l->front = l->front->next;
  }
  free(temp);
  l->size--;
}

/* Modifies Node data pointer to data of the node with the highest priority*/
void* peek(List* l, void* data) {
  if (l->front == NULL) return NULL;
  data = l->front->data;
  return data;
}

/*Removes all elements from the queue*/
void empty_queue(List* l) {
  struct Node* temp = l->front;
  while (temp) {
    l->front = l->front->next;
    free(temp);
    temp = l->front;
  }
  l->front = l->rear = NULL;
  l->size = 0;
}

/**
 * Search list for data
 * Uses equals function pointer, data is passed as the second argument
 * \param l list
 * \param data being searched
 */
void* search(List* l, void* data) {
  struct Node* current = l->front;
  while (current) {
    void* curr_data = current->data;
    if (l->equals(curr_data, data)) return curr_data;
    current = current->next;
  }

  return 0;
}