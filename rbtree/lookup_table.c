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
  double            key;
  double            data;
} MyRBDemo;

typedef void (*myrb_iterae_func)(MyRBDemo* n);

static inline double
linear_interpolate_double(double x0, double y0, double x1, double y1, double x)
{   
  double a = (y1 - y0) / (x1 - x0);
  double b = -a*x0 + y0;
  double y = a * x + b;

  return y;
}


double
lookup_table_interpolate(struct rb_root* root, double v)
{
  struct rb_node*   n = root->rb_node;
  struct rb_node*   adj;
  MyRBDemo*         demo_node;
  MyRBDemo          *first, *second;

  // step 1. find the closest leaf
  while(n)
  {
    demo_node = rb_entry(n, MyRBDemo, rb_node);

    if(v == demo_node->key)
    {
      break;
    }
    else if(v < demo_node->key)
    {
      if(n->rb_left == NULL)
      {
        break;
      }

      n = n->rb_left;
    }
    else 
    {
      if(n->rb_right == NULL)
      {
        break;
      }

      n = n->rb_right;
    }
  }

  if(demo_node->key > v)
  {
    // find prev
    adj = rb_prev(n);
    if(adj == NULL)
    {
      adj = rb_next(n);
    }
  }
  else
  {
    adj = rb_next(n);
    if(adj == NULL)
    {
      adj = rb_prev(n);
    }
  }

  if(adj == NULL)
  {
    printf("why the fuck NULL!!!\n");
    return 0.0;
  }

  first   = rb_entry(n, MyRBDemo, rb_node);
  second  = rb_entry(adj, MyRBDemo, rb_node);

  if(first->key > second->key)
  {
    MyRBDemo*   temp;

    temp = first;
    first = second;
    second = temp;
  }

  double r;

  r = linear_interpolate_double(
      first->key,  first->data,
      second->key, second->data,
      v);

  printf("first: %f, second %f, value: %f:%f\n", first->key, second->key, v, r);

  return r;
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

  printf("beginning to insert\n");
  for(i = 0; i < 3000000; i++)
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

  lookup_table_interpolate(&root, -1);
  lookup_table_interpolate(&root, 10);
  lookup_table_interpolate(&root, 3000001);
  lookup_table_interpolate(&root, 1000000.5);
  return 0;
}
