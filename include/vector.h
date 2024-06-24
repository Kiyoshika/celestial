#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

struct vector_t
{
    void* data;
    size_t len;
    size_t capacity;
    size_t element_size;
    int (*cmp)(const void*, const void*);
};

// init new vector passed by address and specifying size (in bytes) of each item.
// takes a comparison function (cmp) used for sorting and finding items (see qsort().)
// comparison function must return a negative value if the first arg is less than the second arg (i.e., ascending order), zero if equal and positive if first arg is greater than second arg.
// returns false if an error occurred.
bool
vec_init(
    struct vector_t* vec,
    size_t element_size,
    int (*cmp)(const void*, const void*));

// get vector length.
size_t
vec_len(
    const struct vector_t* vec);

// check if vector is empty.
bool
vec_empty(
    const struct vector_t* vec);

// push item to end of vector
void
vec_push_back(
    struct vector_t* vec,
    void* item);

// remove item from end of vector
void
vec_pop_back(
    struct vector_t* vec);

// apply function to each element of vector.
// first arg of apply() is the pointer to the current item in the array.
// second arg of apply() is any additional data passed to the apply function (possibly NULL.)
void
vec_apply(
    struct vector_t* vec,
    void (*apply)(void*, void*));

// get pointer to item in vector.
// returns NULL if idx is out of bounds.
void*
vec_get(
    struct vector_t* vec,
    size_t idx);

// overwrite contents at [idx] of the vector with [item]
// does nothing if idx is out of bounds.
void
vec_set(
    struct vector_t* vec,
    size_t idx,
    const void* item);

// insert a copy of the contents of [item] into the index of the vector and moves remaining items to the right.
// does nothing if idx is out of bounds.
void
vec_insert(
    struct vector_t* vec,
    size_t idx,
    const void* item);

// remote item from vector and shift items left by one.
// does nothing if idx is out of bounds.
void
vec_remove(
    struct vector_t* vec,
    size_t idx);

// clear the vector of all items (reset length to 0.)
void
vec_clear(
    struct vector_t* vec);

// sets a new size and CLEARS the vector contents.
// if you 
void
vec_resize(
    struct vector_t* vec,
    size_t size);

// append contents of [src] vector into [dest] vector.
// if element_size does not match between vectors, does nothing.
void
vec_append(
    const struct vector_t* src,
    struct vector_t* dest);

// copy [src] vector into [dest] vector.
// sets the same length, capacity, element_size and comparison function.
void
vec_copy(
    struct vector_t* src,
    struct vector_t* dest);

// find an [item] inside the vector and returns a pointer to it.
// if item is NOT found, returns NULL.
// if item is found, writes the index into [idx] and returns pointer to item in vec.
void*
vec_find(
    struct vector_t* vec,
    const void* item, size_t* idx);

// sort vector ascending/descending.
void
vec_sort(
    struct vector_t* vec,
    bool ascending);

// check if contents of v1 == v2 element wise.
bool
vec_eq(
    const struct vector_t* v1,
    const struct vector_t* v2);

// check if contents of v1 < v2 element wise.
bool
vec_lt(
    const struct vector_t* v1,
    const struct vector_t* v2);

// check if contents olf v1 <= v2 element wise.
bool
vec_lte(
    const struct vector_t* v1,
    const struct vector_t* v2);

// check if contents of v1 > v2 element wise.
bool
vec_gt(
    const struct vector_t* v1,
    const struct vector_t* v2);

// check if contents of v1 >= v2 element wise.
bool
vec_gte(
    const struct vector_t* v1,
    const struct vector_t* v2);

// check if contents of v1 != v2 element wise.
bool
vec_ne(
    const struct vector_t* v1,
    const struct vector_t* v2);

// subset a vector by specifying indices to grab.
// allocates new (possibly empty) vector to return which must be free'd by the user.
struct vector_t*
vec_subset(
    const struct vector_t* vec,
    const size_t* indices);

// filter a vector by passing a [filter] function which returns true if item matches criteria.
// first arg in [filter] is the current item in vector
// second arg in [filter] is any additional data passed to the filter (possibly NULL)
//
// returns a new (possibly empty) vector containing the items that matched the filter predicate
// that must be free'd by the user.
struct vector_t*
vec_filter(
    const struct vector_t* vec,
    bool (*filter)(const void*, const void*));

// free memory from vector
void
vec_free(
    struct vector_t* vec);

#endif
