#include <stdio.h>
#include <stdlib.h>
#include "rbtree.h"

#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

#define container_of(ptr, type, member) ({            \
      const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
      (type *)( (char *)__mptr - offsetof(type,member) );})

typedef struct
{
  struct rb_node    rb_node;
  int               key;
  int               data;
} MyRBDemo;

typedef void (*myrb_iterae_func)(MyRBDemo* n);

struct rb_node* 
indexer_find_equal_or_bigger(struct rb_root* root, int v)
{
  struct rb_node*   n = root->rb_node;
  struct rb_node*   p;
  MyRBDemo*         demo_node;

  while(n)
  {
    p = n;
    demo_node = rb_entry(n, MyRBDemo, rb_node);

    if(v == demo_node->key)
    {
      return n;
    }
    else if(v < demo_node->key)
    {
      n = n->rb_left;
    }
    else 
    {
      n = n->rb_right;
    }
  }

  demo_node = rb_entry(p, MyRBDemo, rb_node);
  if(demo_node->key > v)
  {
    return p;
  }
  else
  {
    return rb_next(p);
  }

}

struct rb_node*
indexer_get_next(struct rb_node* n)
{
  return rb_next(n);
}

int
lookup_table_insert(struct rb_root* root, MyRBDemo* n)
{
  struct rb_node**  p = &root->rb_node;
  struct rb_node*   parent = NULL;
  MyRBDemo*         page;

  while(*p)
  {
    parent = *p;
    page = rb_entry(parent, MyRBDemo, rb_node);

    if(n->key < page->key)
    {
      p = &(*p)->rb_left;
    }
    else if(n->key > page->key)
    {
      p = &(*p)->rb_right;
    }
    else
    {
      return -1;
    }
  }

  rb_link_node(&n->rb_node, parent, p);
  rb_insert_color(&n->rb_node, root);
  return 0;
}

void
lookup_table_delete(struct rb_root* root, MyRBDemo* n)
{
  rb_erase(&n->rb_node, root);
}


int
main()
{
  struct rb_root    root = RB_ROOT;
  int i;
  MyRBDemo*      d;
  struct rb_node*   n;

  printf("beginning to insert\n");
  for(i = 0; i < 100; i += 2)
  {
    d = (MyRBDemo*)malloc(sizeof(MyRBDemo));
    rb_init_node(&d->rb_node);

    d->key   = i;
    d->data  = i;

    if(lookup_table_insert(&root, d) != 0)
    {
      printf("insert failed for %d\n", i);
      return -1;
    }
  }

  n = indexer_find_equal_or_bigger(&root, 40);
  if(n == NULL)
  {
    printf("BUG: %d\n", 40);
  }
  else
  {
    d = rb_entry(n, MyRBDemo, rb_node);
    printf("result: %d -> %d\n", 40, d->key);
  }

  n = indexer_find_equal_or_bigger(&root, 71);
  if(n == NULL)
  {
    printf("BUG: %d\n", 71);
  }
  else
  {
    d = rb_entry(n, MyRBDemo, rb_node);
    printf("result: %d -> %d\n", 71, d->key);
  }

  n = indexer_find_equal_or_bigger(&root, 80);
  if(n == NULL)
  {
    printf("BUG: %d\n", 80);
  }
  else
  {
    d = rb_entry(n, MyRBDemo, rb_node);
    printf("result: %d -> %d\n", 80, d->key);
  }

  n = indexer_find_equal_or_bigger(&root, -1);
  if(n == NULL)
  {
    printf("BUG: %d\n", -1);
  }
  else
  {
    d = rb_entry(n, MyRBDemo, rb_node);
    printf("result: %d -> %d\n", -1, d->key);
  }

  n = indexer_find_equal_or_bigger(&root, 100);
  if(n != NULL)
  {
    printf("BUG: %d\n", 100);
  }

  n = indexer_find_equal_or_bigger(&root, 55);
  if(n == NULL)
  {
    printf("BUG: %d\n", 55);
  }
  else
  {
    d = rb_entry(n, MyRBDemo, rb_node);
    printf("result: %d -> %d\n", 55, d->key);
  }

  n = indexer_get_next(n);
  if(n == NULL)
  {
    printf("BUG: %d\n", 58);
  }
  else
  {
    d = rb_entry(n, MyRBDemo, rb_node);
    printf("result: %d -> %d\n", 58, d->key);
  }

  n = indexer_find_equal_or_bigger(&root, 98);
  if(n == NULL)
  {
    printf("BUG: %d\n", 98);
  }
  else
  {
    d = rb_entry(n, MyRBDemo, rb_node);
    printf("result: %d -> %d\n", 98, d->key);
  }

  n = indexer_get_next(n);
  if(n != NULL)
  {
    printf("BUG: next of 98 gotta be NULL\n");
  }
  else
  {
    printf("got NULL for next of 98\n");
  }
  return 0;
}
