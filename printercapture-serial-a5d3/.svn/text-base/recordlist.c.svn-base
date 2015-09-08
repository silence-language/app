#ifndef offset_of 
#define offset_of(type, memb) \
  ((unsigned long )(&((type*)0)->memb))
#endif
#ifndef container_of
#define container_of
((type*)(((char*)obj) - offset_of(type, memb)))
#endif
struct list_head
{
  struct list_head *next;
  struct list_head *prev;
};
#define LIST_HEAD_INIT(name) {&(name), &(name)}
#define LIST_HEAD(name) \
  struct list_head name = LIST_HEAD_INIT(name)
static inline void INIT_LIST_HEAD(struct list_head *list)
{
  list->next = list;
  list->prev = list;
}
static inline void __list_add(struct list_head *new, struct list_head *prev, struct list_head *next)
{
  next->prev = new;
  new->next = next;
  new->prev = prev;
  prev->next = new;
}
static inline void list_add(struct list_head *new, struct list_head *head)
{
  __list_add(new, head,head->next);
}
static inline void list_add_tail(struct list_head *new, struct list_head *head)
{
  __list_add(new, head->prev, head);
}
static inline void __list_del(struct list_head *prev, struct list_head *next)
{
  next->prev = prev;
  prev->next = next;
}
static inline void list_del(struct *entry))
{
  __list_del(entry->prev, entry->next);
}
struct RecordFileStruct
{
  char filename[40];
  FILE *file;
  list_head entry;
  
};
