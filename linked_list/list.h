#ifndef QUEUE_H
#define QUEUE_H

typedef struct Node {
  void* data;
  struct Node* next;
  int priority;

} Node;

typedef struct Queue {
  struct Node* front;
  struct Node* rear;
  int size;
} Queue;

void queue_init(Queue* q);  // TODO const
void* peek(Queue* q, void* data);
void dequeue(Queue* q);
void enqueue(Queue* q, void* data);
void empty_queue(Queue* q);

#endif