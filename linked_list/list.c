#include <stdio.h>
#include <stdlib.h>

#include "list.h"

/*Initializes queue with function pointers, and other front and rear pointers*/
void queue_init(Queue* q) {
  q->front = NULL;
  q->rear = NULL;
  q->size = 0;
}

/* Add to queue without priority (linear queue)*/
void enqueue(Queue* q, void* data) {
  struct Node* temp = (struct Node*)malloc(sizeof(struct Node));
  temp->data = data;
  temp->next = NULL;
  temp->priority = 0;
  q->size++;
  if (q->front == NULL && q->rear == NULL) {
    q->front = q->rear = temp;
    return;
  }
  q->rear->next = temp;
  q->rear = temp;
}

/* Removes node with highest priority*/
void dequeue(Queue* q) {
  struct Node* temp = q->front;
  if (q->front == NULL) return;
  if (q->front == q->rear) {
    q->front = q->rear = NULL;
  } else {
    q->front = q->front->next;
  }
  free(temp);
  q->size--;
}

/* Modifies Node data pointer to data of the node with the highest priority*/
void* peek(Queue* q, void* data) {
  if (q->front == NULL) return NULL;
  data = q->front->data;
  return data;
}

/*Removes all elements from the queue*/
void empty_queue(Queue* q) {
  struct Node* temp = q->front;
  while (temp) {
    q->front = q->front->next;
    free(temp);
    temp = q->front;
  }
  q->front = q->rear = NULL;
  q->size = 0;
}