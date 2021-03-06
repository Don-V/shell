#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "list.h"

typedef struct query_result {
  Node* res;
  Node* prev;
} query_result_t;

void list_init(List* l, bool (*equals)(const void* data1, const void* data2)) {
  l->front = 0;
  l->rear = 0;
  l->_iter = 0;
  l->size = 0;
  l->equals = equals;
}

void enqueue(List* l, void* data) {
  // create internal node with data
  struct Node* temp = (struct Node*)malloc(sizeof(struct Node));
  temp->data = data;
  temp->next = NULL;
  l->size++;

  // add node to list
  if (l->front == NULL && l->rear == NULL) {
    l->front = l->rear = temp;
    return;
  }
  l->rear->next = temp;
  l->rear = temp;
}

static query_result_t search(List* l, void* query) {
  Node* current = l->front;
  Node* prev = 0;
  query_result_t res = {0, 0};
  while (current) {
    void* curr_data = current->data;
    if (l->equals(curr_data, query)) {
      res.prev = prev;
      res.res = current;
      return res;
    }
    prev = current;
    current = current->next;
  }

  return res;
}

void* list_remove(List* l, void* data) {
  query_result_t found = search(l, data);
  Node *curr = found.res, *prev = found.prev;
  if (curr) {
    // move head ptr
    if (curr == l->front) {
      l->front = curr->next;
    }

    // move tail ptr
    if (curr == l->rear) {
      l->rear = prev;
    }

    // update previous ptr
    if (prev) {
      prev->next = curr->next;
    }

    // book keeping
    void* res = curr->data;
    free(curr);
    l->size--;
    l->_iter = 0;
    return res;
  }
  return 0;
}

void* list_search(List* l, void* query) {
  query_result_t found = search(l, query);
  if (found.res) return (found.res)->data;
  return 0;
}

void* iterate(List* l, bool start) {
  // change start position to start of the list
  if (start) l->_iter = l->front;

  // move iterator to next node
  Node* curr = l->_iter;
  if (curr) {
    l->_iter = curr->next;
    return curr->data;
  }
  return 0;
}