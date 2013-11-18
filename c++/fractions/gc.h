/*
 *  Copyright (C) 2013 Free Software Foundation, Inc.
 *
 *  a simple garbage collector for C++
 *
 *  Gaius Mulley <gaius.mulley@southwales.ac.uk>
 */

#if !defined(GCH)
#  define GCH

typedef enum {freed = 1, marked = 2, in_use = 4, in_error=8, max_state = 16} state;

class entity
{
 public:
  void   *data;
  state   status;
  entity *a_next;
  entity *r_next;
  entity *f_next;

  entity (void);
  ~entity (void);
  entity (const entity &from);  // copy
  entity& operator= (const entity &from);  // assignment

  void unmark (void);
  void mark (void);
  bool is_marked (void);
  
  void free (void);
  void unfree (void);
  bool is_free (void);

  void used (void);
  void unused (void);
  bool is_used (void);

  void do_assert (void);
};

class gc
{
 private:
  entity *rooted;
  entity *allocated;
  int     bytes;
  char   *desc;
  entity *free_list;
  gc     *next;
 public:
  gc (int no_of_bytes, const char *description);
  ~gc ();
  void collect (void);
  void *allocate (entity *&e);
  void root (entity *e);
  void unroot (entity *e);
  void *get_data (entity *e);
  gc *find_gc (unsigned int no_bytes);
  entity *get_entity (void *data);
  bool is_rooted (entity *e);
  void mark_allocated (void);

  void stats (void);
  int no_of_allocated (void);
  int no_of_freed (void);
  int no_of_rooted (void);
};


/*
 *  garbage_collect - pre-condition : none.
 *                    post-condition: all the garbage collectors
 *                                    will attempt to reclaim lost
 *                                    memory.
 */

void garbage_collect (void);


/*
 *  allocate - pre-condition:  init_garbage has been called to maintain a, bytes, heap.
 *             post-condition: entity, e, is filled in and the allocated memory is returned.
 */

void *allocate (unsigned int bytes, entity *&e);


/*
 *  init_garbage - pre-condition :  none.
 *                 post-condition:  a garbage collector is created to serve calls
 *                                  for bytes amount of memory.
 */

gc *init_garbage (unsigned int bytes, const char *description);

#endif
