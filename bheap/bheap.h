#ifndef BHEAP_BHEAP_H_
#define BHEAP_BHEAP_H_
struct BHeap {
	size_t count;	/* count of occupied nodes */
	size_t alloc;	/* count of allocated nodes */
	size_t width;	/* byte size per node */
	void *nodes;
	int (*compare)(const void *,
		       const void *);
};

/* initialize, destroy, resize
 ******************************************************************************/
inline struct BHeap *init_sized_bheap(const size_t width,
				      const size_t size,
				      int (*compare)(const void *,
						     const void *))
{
	struct BHeap *heap;

	HANDLE_MALLOC(heap, sizeof(struct BHeap));
	HANDLE_MALLOC(heap->nodes, size);

	/* sentinel node at index 0 in */
	--(heap->nodes);

	heap->count   = 0ul;
	heap->alloc   = size;
	heap->width   = width;
	heap->compare = compare;

	return heap;
}

inline struct BHeap *init_bheap(const size_t width,
				int (*compare)(const void *,
					       const void *))
{
	return init_sized_bheap(width, width, compare);
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

inline void realloc_bheap(struct BHeap *heap,
			  const size_t alloc)
{
	void *nodes = realloc(&heap->nodes[1l], heap->width * alloc);

	if (nodes == NULL)
		EXIT_ON_FAILURE("failed to reallocate number of nodes"
				"from %lu to %lu",
				heap->alloc, alloc);

	heap->nodes = &nodes[-1l];
	heap->alloc = alloc;
}



/* insertion
 ******************************************************************************/
void do_insert(void *const nodes,
	       void *const next,
	       const size_t width,
	       const ptrdiff_t i_next,
	       int (*compare)(const void *,
			      const void *));

void bheap_insert_array(struct BHeap *heap,
			void *const array,
			const size_t length);

inline void bheap_insert(struct BHeap *heap,
			 void *const next)
{
	++(heap->count);

	if (heap->count == heap->alloc)
		realloc_bheap(heap, heap->alloc * 2ul);

	do_insert(heap->nodes, next, heap->count, heap->width, heap->compare);
}




/* extraction
 ******************************************************************************/
void *bheap_extract(struct BHeap *heap);

void do_bheap_shift(void *const restrict nodes,
		    void *const restrict next,
		    const size_t width,
		    const ptrdiff_t i_next,
		    const ptrdiff_t i_base,
		    int (*compare)(const void *,
				   const void *));


/* display
 ******************************************************************************/
void print_bheap(struct BHeap *heap,
		 void (*node_to_string)(char *,
					const void *));




/* heapsort
 ******************************************************************************/
void sort_bheap_nodes(void *const nodes,
		      const size_t length,
		      const size_t width,
		      int (*compare)(const void *,
				     const void *));

inline void bheap_sort(void *const array,
		       const size_t length,
		       const size_t width,
		       int (*compare)(const void *,
				      const void *))
{
	sort_bheap_nodes(&array[-1l], length, width, compare);
}



/* convienience, misc
 ******************************************************************************/
inline struct BHeap *array_into_bheap(void *const array,
				      const size_t length,
				      const size_t width,
				      int (*compare)(const void *,
						     const void *))
{
	const size_t array_size = width * length;
	struct BHeap *heap;
	void *nodes;

	HANDLE_MALLOC(heap, sizeof(struct BHeap));
	HANDLE_MALLOC(nodes, array_size);
	memcpy(nodes, array, array_size);

	/* sentinel node at index 0 */
	--nodes;

	sort_bheap_nodes(nodes, width, length, compare);

	heap->nodes   = nodes;
	heap->count   = length;
	heap->alloc   = length;
	heap->compare = compare;

	return heap;

}
#endif /* ifndef BHEAP_BHEAP_H_ */
