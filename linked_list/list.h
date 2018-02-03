#ifndef LIST_H
#define LIST_H

typedef struct Node {
  void* data;
  struct Node* next;

} Node;

typedef struct List {
  struct Node* front;
  struct Node* rear;
  bool (*equals)(const void* data1, const void* data2);
  int size;
} List;

void list_init(List* l, bool (*equals)(const void* data1, const void* data2));
void* peek(List* l, void* data);
void dequeue(List* l);
void enqueue(List* l, void* data);
void* search(List* l, void* data);
void empty_queue(List* l);

#endif