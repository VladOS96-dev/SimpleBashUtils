#ifndef LIST
#define LIST

typedef struct list {
  char *value;
  struct list *next;
} list;
struct list *init(char *value);
struct list *add(list *elem, char *value);
list *change_last_elem(list *elem, char *value);
void destroy(list *root);
#endif
