/**
 * growing array(not thread safe).
 */
#include <stdlib.h>
#include <stdio.h>

#include "cj_array.h"

/**
 * init an array, with n element.
 * @n, initial number elements in an array.
 * return a pointer to cj_array with capacity of n elements,
 * otherwise, it returns NULL.
 */
struct cj_array *cj_array_alloc(int n) {
  struct cj_array *arr;

  arr = malloc(sizeof(struct cj_array));
  if (arr == NULL)
    return NULL;
  
  if (n <= 0)
    return NULL;
  
  arr->raw_array = malloc(n*sizeof(void *));
  if (arr->raw_array == NULL) 
    return NULL;
  arr->size = 0;
  arr->capacity = n;

  return arr;
}

/**
 * add a elm to the end of an array
 * @arr, array being added into
 * @elm, added element
 * return 0 upon success, -1 otherwise.
 */
int cj_array_add(struct cj_array *arr, void *elm) {
  void **new_raw_array;
  int i;
  
  if (arr->size == arr->capacity) {
    /* grow capacity */
    arr->capacity *= 2;
    new_raw_array = malloc(arr->capacity*sizeof(void *));
    if (new_raw_array == NULL) {
      arr->capacity /= 2;
      return -1;
    }
    for (i = 0; i < arr->size; ++i) {
      new_raw_array[i] = arr->raw_array[i];
    }
    free(arr->raw_array);
    arr->raw_array = new_raw_array;
  }

  arr->raw_array[arr->size++] = elm;
  return 0;
}


/**
 * remove the lasted added elemenet, and return it.
 * @arr, array.
 * return the pointer to an element if success. else NULL.
 */
void *cj_array_pop(struct cj_array *arr) {
  if (arr->size <= 0) {
    return NULL;
  }
  
  return arr->raw_array[--arr->size];
}


#ifdef CJ_TEST
#include <assert.h>

int main() {
  int i;
  int *tmp;
  struct cj_array *my_arr;

  my_arr = cj_array_alloc(3);
  assert(my_arr != NULL);
  
  for (i = 0; i < 10; ++i) {
    tmp = malloc(sizeof(int));
    *tmp = i;
    cj_array_add(my_arr, tmp);
  }
  
  i = 9;
  while ((tmp = cj_array_pop(my_arr)) != NULL) {
    assert(*tmp == i--);
  }

  CJ_ARRAY_DESTROY(my_arr, int *);
  
  printf("cj_array passed the test.\n");
  return 0;
}
#endif

