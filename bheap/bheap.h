#ifndef BHEAP_BHEAP_H_
#define BHEAP_BHEAP_H_
struct BHeap {
	size_t count;
	size_t alloc;
	void **nodes;
	int (*compare)(const void *,
		       const void *);
};

/* initialize, destroy, resize
 ******************************************************************************/
inline struct BHeap *init_sized_bheap(const size_t size,
				      int (*compare)(const void *,
						     const void *))
{
	struct BHeap *heap;

	HANDLE_MALLOC(heap, sizeof(struct BHeap));
	HANDLE_MALLOC(heap->nodes, sizeof(void *) * size);

	/* sentinel node at index 0 in */
	--(heap->nodes);

	heap->count   = 0ul;
	heap->alloc   = size;
	heap->compare = compare;

	return heap;
}

inline struct BHeap *init_bheap_from_array(void **array,
					   const size_t length,
					   int (*compare)(const void *,
							  const void *))
{
	struct BHeap *heap;

	HANDLE_MALLOC(heap, sizeof(struct BHeap));

	/* sentinel node at index 0 in */
	heap->nodes   = array - 1l;
	heap->count   = length;
	heap->alloc   = length;
	heap->compare = compare;

	return heap;
}

inline struct BHeap *init_bheap(int (*compare)(const void *,
					       const void *))
{
	return init_sized_bheap(compare, 1ul);
}


inline void clear_bheap(struct BHeap *heap)
{
	heap->count = 0ul;
}

inline void free_bheap(struct BHeap *heap)
{
	free(&heap->nodes[1l]);
	free(heap);
}

inline void resize_bheap(struct BHeap *heap,
			 const size_t size)
{
	void **nodes = realloc(&heap->nodes[1l], sizeof(void *) * size);

	if (nodes == NULL)
		EXIT_ON_FAILURE("failed to reallocate number of nodes"
				"from %lu to %lu",
				heap->alloc, size);

	heap->nodes = nodes - 1l;
	heap->alloc = size;
}



/* insertion
 ******************************************************************************/
void do_insert(void **nodes,
	       void *next,
	       const size_t next_i,
	       int (*compare)(const void *,
			      const void *));

void bheap_insert_array(struct BHeap *heap,
			void **array,
			const size_t length);

inline void bheap_insert(struct BHeap *heap,
			 void *next)
{
	++(heap->count);

	do_insert(heap->nodes, next, heap->count, heap->compare);

	if (heap->count == heap->alloc)
		resize_bheap(heap, heap->alloc * 2ul);
}




/* extraction
 ******************************************************************************/
void *bheap_extract(struct BHeap *heap);

void do_shift(void **nodes,
	      void *next,
	      const size_t next_i,
	      const size_t base_i,
	      int (*compare)(const void *,
			     const void *));


/* display
 ******************************************************************************/
void print_bheap(struct BHeap *heap,
		 void (*node_to_string)(char *,
					const void *));




/* heapsort
 ******************************************************************************/
void bheap_sort(void **array,
		const size_t length,
		int (*compare)(const void *,
			       const void *));



/* convienience, misc
 ******************************************************************************/
inline struct BHeap *array_into_bheap(void **array,
				      const size_t width,
				      const size_t length,
				      int (*compare)(const void *,
						     const void *))
{
	const size_t array_size = width * length;
	void **nodes;

	HANDLE_MALLOC(nodes, array_size);

	memcpy(nodes, array, array_size);

	bheap_sort(nodes, length, compare);

	return init_bheap_from_array(nodes, length, compare);

}
#endif /* ifndef BHEAP_BHEAP_H_ */
