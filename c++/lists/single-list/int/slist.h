#if !defined(SLISTH)
#  define SLISTH

/*
 *  single linked list implementation.
 */


class element
{
 public:
  element *next;
  int      data;
};

class slist
{
 private:
  element *head_element;
  element *duplicate_elements (element *e);  
  element *delete_elements (element *h);

 public:
  slist (void);
  ~slist (void);
  slist (const slist &from);
  slist& operator= (const slist &from);
  
  slist empty (void);
  bool  is_empty ();
  slist cons (int i);
  int   head (void);
  slist tail (void);
};


#endif
