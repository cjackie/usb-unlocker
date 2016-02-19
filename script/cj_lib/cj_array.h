#ifndef CJ_ARRAY_H
#define CJ_ARRAY_H

struct cj_array {
  int capacity;
  int size;
  void **raw_array;
};

struct cj_array *cj_array_alloc(int n);
int cj_array_add(struct cj_array *arr, void *elm);
void *cj_array_pop(struct cj_array *arr);

/**
 * free memory of cj_array
 * @arr, a pointer to struct cj_array. memory being freed.
 * @type, type of the element stored in arr.
 * return 0 on success, -1 otherwise.
 */
#define CJ_ARRAY_DESTROY(arr, type) ({		\
  int i;					\
  int retval;					\
						\
  retval = 0;					\
						\
  for (i = 0; i < (arr)->size; ++i) {		\
    if ((arr)->raw_array[i] == NULL) {		\
      retval = -1;				\
      break;					\
    }						\
  }						\
						\
  if (retval == 0) {				\
    for (i = 0; i < (arr)->size; ++i) {		\
      free((type)(arr)->raw_array[i]);		\
      (arr)->raw_array[i] = NULL;		\
    }						\
    free((arr)->raw_array);			\
    free(arr);					\
    retval = 0;					\
  }						\
						\
  retval;					\
})						\


#endif	/* CJ_ARRAY_H */

