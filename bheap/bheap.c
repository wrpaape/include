#include <utils/utils.h>
#include <bheap/bheap.h>

/* initialize, destroy, resize
 ******************************************************************************/
extern inline struct BHeap *init_bheap(const size_t width,
				       int (*compare)(const void *,
						      const void *));

extern inline struct BHeap *init_sized_bheap(const size_t width,
					     const size_t size,
					     int (*compare)(const void *,
							    const void *));

extern inline void free_bheap(struct BHeap *heap);


extern inline void realloc_bheap(struct BHeap *heap,
				 const size_t size);

/* insertion
 ******************************************************************************/
extern inline void bheap_insert(struct BHeap *heap,
				void *const next);

void bheap_insert_array(struct BHeap *heap,
			void *const array,
			const size_t length)
{
	void *const nodes  = heap->nodes;
	const size_t count = heap->count;
	const size_t width = heap->width;
	const size_t next_count = count + length;

	if (heap->alloc < next_count)
		realloc_bheap(heap, next_pow_two(next_count));

	int (*compare)(const void *,
		       const void *) = heap->compare;


	for (size_t i = 0ul; i < length; ++i)
		do_insert(nodes, &array[i], count + i, width, compare);

	heap->count = next_count;
}



void do_insert(void *const nodes,
	       void *const next,
	       const size_t width,
	       const ptrdiff_t i_next,
	       int (*compare)(const void *,
			      const void *))
{
	/* sentinel node has been reached, 'next' is new root node */
	if (i_next == 1l) {
		/* nodes[1l] = next; */
		memcpy(&nodes[1l], next, width);
		return;
	}


	const ptrdiff_t i_parent = i_next / 2l;
	void *const parent = &nodes[i_parent];

	if (compare(parent, next)) {
		/* nodes[i_next] = next; */
		memcpy(&nodes[i_next], next, width);
		return;
	}

	/* nodes[i_next] = parent; */
	memcpy(&nodes[i_next], parent, width);
	do_insert(nodes, next, width, i_parent, compare);
}




/* extraction
 ******************************************************************************/
void *bheap_extract(struct BHeap *heap)
{
	if (heap->count == 0ul)
		return NULL;

	void *const nodes = heap->nodes;
	void *const root  = &nodes[1l];
	void *const base  = &nodes[heap->count];

	--(heap->count);

	do_bheap_shift(nodes, base, heap->width,
		       1l, heap->count, heap->compare);

	return root;
}

void do_bheap_shift(void *const restrict nodes,
		    void *const restrict next,
		    const size_t width,
		    const ptrdiff_t i_next,
		    const ptrdiff_t i_base,
		    int (*compare)(const void *,
				   const void *))
{

	const ptrdiff_t i_lchild = i_next * 2l;

	/* if base level of heap has been reached (no more children), replace
	 **********************************************************************/
	if (i_lchild > i_base) {
		memcpy(&nodes[i_next], next, width);
		/* nodes[i_next] = next; */
		return;
	}

	const ptrdiff_t i_rchild = i_lchild + 1l;

	void *const restrict lchild = &nodes[i_lchild];

	/* compare left child with 'next':
	 *
	 * if 'lchild' belongs above 'next'...
	 **********************************************************************/
	if (compare(lchild, next)) {

		/* if base level of heap has been reached (no more children),
		 * place 'next' below 'lchild' and return
		 **************************************************************/
		if (i_rchild > i_base) {
			memcpy(&nodes[i_lchild], next,   width);
			memcpy(&nodes[i_next],   lchild, width);
			/* nodes[i_lchild] = next; */
			/* nodes[i_next]	= lchild; */
			return;
		}

		void *const restrict rchild = &nodes[i_rchild];

		/* compare left child with right child:
		 *
		 * if 'lchild' belongs above 'rchild'...
		 **************************************************************/
		if (compare(lchild, rchild)) {
			/* place 'lchild' at 'i_next' and continue recursion
			 * down left branch
			 ******************************************************/
			/* nodes[i_next] = lchild; */
			memcpy(&nodes[i_next], lchild, width);
			do_bheap_shift(nodes, next, width,
				       i_lchild, i_base, compare);

		} else {
			/* place 'rchild' at 'i_next' and continue recursion
			 * down right branch
			 ******************************************************/
			/* nodes[i_next] = rchild; */
			memcpy(&nodes[i_next], rchild, width);
			do_bheap_shift(nodes, next, width,
				       i_rchild, i_base, compare);
		}
		return;
	}

	/* if base level of heap has been reached (no more children), place
	 * 'next' above 'lchild' (new base/last element) and return
	 **********************************************************************/
	if (i_rchild > i_base) {
		/* nodes[i_next] = next; */
		memcpy(&nodes[i_next], next, width);
		return;
	}

	void *const rchild = &nodes[i_rchild];

	/* compare 'next' with right child:
	 *
	 * if 'rchild' belongs above 'next'...
	 **********************************************************************/
	if (compare(rchild, next)) {
		/* nodes[i_next] = rchild; */
		memcpy(&nodes[i_next], rchild, width);
		do_bheap_shift(nodes, next, width, i_rchild, i_base, compare);
		return;
	}

	/* otherwise, 'next' belongs above lchild and rchild: place at 'i_next'
	 * and return
	 **********************************************************************/
	/* nodes[i_next] = next; */
	memcpy(&nodes[i_next], next, width);
}




/* display
 ******************************************************************************/
void print_bheap(struct BHeap *heap,
		 void (*node_to_string)(char *,
					const void *))
{
	const size_t count = heap->count;

	if (count == 0ul) {
		puts("[ EMPTY ]");
		return;
	}

	void *const nodes = heap->nodes;
	char buffer[256];

	for (size_t i = 1ul; i <= count; ++i) {
		node_to_string(buffer, &nodes[i]);
		printf("nodes[%zu]:\n%s\n", i, buffer);
	}
}




/* heapsort
 ******************************************************************************/
extern inline void bheap_sort(void *const array,
			      const size_t length,
			      const size_t width,
			      int (*compare)(const void *,
					     const void *));

void sort_bheap_nodes(void *const nodes,
		      const size_t length,
		      const size_t width,
		      int (*compare)(const void *,
				     const void *))
{
	ptrdiff_t i = length;
	void *next;

	while (i > 1l) {
		next = &nodes[i];
		--i;
		do_bheap_shift(nodes, next, width, i, length, compare);
	}
}


/* convienience, misc
 ******************************************************************************/
extern inline struct BHeap *array_into_bheap(void *const array,
					     const size_t length,
					     const size_t width,
					     int (*compare)(const void *,
							    const void *));
