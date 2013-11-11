/*
 *  Copyright (C) 2013 Free Software Foundation, Inc.
 *
 *  introduce the fractional data type.
 *
 *  Gaius Mulley <gaius.mulley@southwales.ac.uk>
 */

#if !defined(GCH)
#  define GCH

typedef enum {freed = 1, marked = 2, in_use = 4, in_error=8, max_state = 16} state;

class entity
{
  void   *data;
  state   status;
  entity *a_next;
  entity *r_next;

 public:
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
  gc (int no_of_bytes, char *description);
  ~gc ();
  void collect (void);
  void *allocate (entity *&e);
  void root (entity *e);
  void unroot (entity *e);
  void *get_data (entity *e);
};


/*
 *  garbage_collect - pre-condition : none.
 *                    post-condition: all the garbage collectors
 *                                    will attempt to reclaim lost
 *                                    memory.
 */

void garbage_collect (void);

#endif
