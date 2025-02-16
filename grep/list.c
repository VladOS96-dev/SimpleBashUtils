#include "list.h"

#include <stdio.h>
#include <stdlib.h>

struct list *init(char *value) {
  list *tmp = (list *)malloc(sizeof(list));
  tmp->value = value;
  tmp->next = NULL;
  return (tmp);
}
struct list *add(list *elem, char *value) {
  list *tmp = (list *)malloc(sizeof(list));
  tmp->value = value;
  tmp->next = NULL;
  list *p;
  p = elem;
  while (p->next != NULL) {
    p = p->next;
  }
  p->next = tmp;

  return (tmp);
}

void destroy(list *root) {
  while (root != NULL) {
    list *p = root;
    root = root->next;
    free(p);
  }
}
