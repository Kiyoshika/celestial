#include "vector.h"

bool
vec_init(
	struct vector_t* vec,
	size_t element_size,
	int (*cmp)(const void*, const void*))
{
	vec->len = 0;
	vec->capacity = 8;
	vec->element_size = element_size;

	vec->data = calloc(vec->capacity, element_size);
	if (!vec->data)
		return false;

	vec->cmp = cmp;

	return true;
}

size_t
vec_len(
	const struct vector_t* vec)
{
	if (!vec)
		return 0;

	return vec->len;
}

bool
vec_is_empty(
	const struct vector_t* vec)
{
	if (!vec)
		return true;

	return vec->len == 0;
}

void
vec_push_back(
	struct vector_t* vec,
	const void* item)
{
	if (!vec)
		return;

	void* position = &vec->data[vec->len];
	memcpy(position, item, vec->element_size);
	vec->len++;
	if (vec->len == vec->capacity)
	{
		size_t new_capacity = vec->capacity * 2;
		void* alloc = realloc(vec->data, vec->element_size * new_capacity);
		if (!alloc)
		{
			fprintf(stderr, "WARN: couldn't allocate more memory to vector in vec_push_back.\n");
			return;
		}
		vec->data = alloc;
		vec->capacity = new_capacity;
	}
}

void
vec_pop_back(
	struct vector_t* vec)
{
	if (!vec || vec->len == 0)
		return;

	vec->len--;
}

void
vec_foreach(
	struct vector_t* vec,
	void* callback_data,
	void (*callback)(void*, void*))
{
	if (!vec || !callback)
		return;

	for (size_t i = 0; i < vec->len; ++i)
		callback(&vec->data[i], callback_data);
}

void*
vec_get(
	const struct vector_t* vec,
	size_t idx)
{
	if (!vec || idx >= vec->len)
		return NULL;

	return &vec->data[idx];
}

void
vec_set(
	struct vector_t* vec,
	size_t idx,
	const void* item)
{
	if (!vec || idx >= vec->len)
		return;

	memcpy(vec->data, item, vec->element_size);
}

void
vec_insert(
	struct vector_t* vec,
	size_t idx,
	const void* item)
{
	if (!vec || idx >= vec->len)
		return;

	const void* blank = calloc(1, vec->element_size);
	vec_push_back(vec, blank);

	// shift everything over one position after adding blank item
	// (additional -1 to account for the newly-added item)
	size_t size = (vec->len - idx - 1) * vec->element_size;
	memmove(&vec->data[idx + 1], &vec->data[idx], size);

	// add new item into position
	memcpy(&vec->data[idx], item, vec->element_size);
}

void
vec_remove(
	struct vector_t* vec,
	size_t idx)
{
	if (!vec || idx >= vec->len)
		return;

	// special case
	if (idx == vec->len - 1)
		vec_pop_back(vec);
	else
	{
		size_t size = (vec->len - idx - 1) * vec->element_size;
		memmove(&vec->data[idx], &vec->data[idx + 1], size);
		vec->len--;
	}
}

void
vec_clear(
	struct vector_t* vec)
{
	memset(&vec->data, 0, vec->capacity * vec->element_size);
	vec->len = 0;
}

void
vec_resize(
	struct vector_t* vec,
	size_t n_items)
{
	if (!vec)
		return;

	void* alloc = realloc(vec->data, n_items * vec->element_size);
	if (!alloc)
	{
		fprintf(stderr, "WARN: couldn't allocate enough memory when using vec_resize().\n");
		return;
	}

	vec->data = alloc;
	memset(vec->data, 0, n_items * vec->element_size);
	vec->len = n_items;
	vec->capacity = n_items;
}

void
vec_fill(
	struct vector_t* vec,
	const void* item)
{
	if (!vec)
		return;

	for (size_t i = 0; i < vec->len; ++i)
		vec_set(vec, i, item);
}

void
vec_append(
	struct vector_t* dest,
	const struct vector_t* src)
{
	if (!dest || !src || dest->element_size != src->element_size)
		return;

	for (size_t i = 0; i < src->len; ++i)
		vec_push_back(dest, vec_get(src, i));
}

void
vec_copy(
	struct vector_t* dest,
	const struct vector_t* src)
{
	if (!dest || !src)
		return;

	if (dest->capacity < src->len)
	{
		void* alloc = realloc(dest->data, src->len * src->element_size);
		if (!alloc)
		{
			fprintf(stderr, "WARN: unable to allocate enough memory when using vec_copy().\n");
			return;
		}

		dest->data = alloc;
	}

	dest->capacity = src->len;
	dest->len = src->len;
	dest->element_size = src->element_size;

	for (size_t i = 0; i < src->len; ++i)
		vec_set(dest, i, vec_get(src, i));
}

void*
vec_find(
	struct vector_t* vec,
	const void* item,
	size_t* idx)
{
	if (!vec)
		return NULL;

	for (size_t i = 0; i < vec->len; ++i)
	{
		if (vec->cmp(vec_get(vec, i), item) == 0)
		{
			*idx = i;
			return &vec->data[i];
		}
	}

	return NULL;
}

void
vec_set_comparison(
	struct vector_t* vec,
	int (*cmp)(const void*, const void*))
{
	vec->cmp = cmp;
}

void
vec_sort(
	struct vector_t* vec)
{
	if (!vec)
		return;

	qsort(vec->data, vec->len, vec->element_size, vec->cmp);
}

void
vec_subset(
	const struct vector_t* vec,
	const size_t* indices,
	size_t n_indices,
	struct vector_t* dest)
{
	if (!vec || !indices)
		return;

	vec_init(dest, vec->element_size, vec->cmp);

	for (size_t i = 0; i < n_indices; ++i)
	{
		void* item = vec_get(vec, indices[i]);
		if (!item)
			continue;

		vec_push_back(dest, item);
	}
}

void
vec_filter(
	const struct vector_t* vec,
	const void* filter_data,
	bool (*filter)(const void*, const void*),
	struct vector_t* dest)
{
	if (!vec || !filter)
		return;

	vec_init(dest, vec->element_size, vec->cmp);

	for (size_t i = 0; i < vec->len; ++i)
	{
		const void* item = vec_get(vec, i);
		if (filter(item, filter_data))
			vec_push_back(dest, item);
	}
}

bool
vec_eq(
	const struct vector_t* v1,
	const struct vector_t* v2)
{
	if (!v1 || !v2 || v1->len != v2->len)
		return false;

	for (size_t i = 0; i < v1->len; ++i)
	{
		const void* i1 = vec_get(v1, i);
		const void* i2 = vec_get(v2, i);
		if (v1->cmp(i1, i2) != 0)
			return false;
	}

	return true;
}

void
vec_free(
	struct vector_t* vec)
{
	free(vec->data);
	vec->data = NULL;
}
