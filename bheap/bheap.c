#include <utils/utils.h>
#include <bheap/bheap.h>

/* initialize, destroy, resize
 ******************************************************************************/
extern inline struct BHeap *init_bheap(int (*compare)(const void *,
						      const void *));

extern inline struct BHeap *init_sized_bheap(const size_t size,
					     int (*compare)(const void *,
							    const void *));

extern inline struct BHeap *init_bheap_from_array(void **array,
						  const size_t length,
						  int (*compare)(const void *,
								 const void *));

extern inline void free_bheap(struct BHeap *heap);


extern inline void resize_bheap(struct BHeap *heap,
				const size_t size);



/* insertion
 ******************************************************************************/
extern inline void bheap_insert(struct BHeap *heap,
				void *next);

void bheap_insert_array(struct BHeap *heap,
			void **array,
			const size_t length)
{
	const size_t count = heap->count;
	const size_t next_count = count + length;

	if (heap->alloc < next_count)
		resize_bheap(heap, next_pow_two(next_count));

	void **nodes = heap->nodes;
	int (*compare)(const void *,
		       const void *) = heap->compare;


	for (size_t i = 0ul; i < length; ++i)
		do_insert(nodes, array[i], count + i, compare);

	heap->count = next_count;
}



void do_insert(void **nodes,
	       void *next,
	       const ptrdiff_t i_next,
	       int (*compare)(const void *,
			      const void *))
{
	/* sentinel node has been reached, 'next' is new root node */
	if (i_next == 1l) {
		nodes[1l] = next;
		return;
	}


	const ptrdiff_t i_parent = i_next / 2l;
	void *parent = nodes[i_parent];

	if (compare(parent, next)) {
		nodes[i_next] = next;
		return;
	}

	nodes[i_next] = parent;
	do_insert(nodes, next, i_parent, compare);
}




/* extraction
 ******************************************************************************/
void *bheap_extract(struct BHeap *heap)
{
	if (heap->count == 0ul)
		return NULL;

	void **nodes = heap->nodes;
	void *root   = nodes[1l];
	void *base   = nodes[heap->count];

	--(heap->count);

	do_shift(nodes, base, 1l, heap->count, heap->compare);

	return root;
}

void do_shift(void **nodes,
	      void *next,
	      const ptrdiff_t i_next,
	      const ptrdiff_t i_base,
	      int (*compare)(const void *,
			     const void *))
{

	const ptrdiff_t i_lchild = i_next * 2l;

	/* if base level of heap has been reached (no more children), replace
	 **********************************************************************/
	if (i_lchild > i_base) {
		nodes[i_next] = next;
		return;
	}

	const ptrdiff_t i_rchild = i_lchild + 1l;

	void *lchild = nodes[i_lchild];

	/* compare left child with 'next':
	 *
	 * if 'lchild' belongs above 'next'...
	 **********************************************************************/
	if (compare(lchild, next)) {

		/* if base level of heap has been reached (no more children),
		 * place 'next' below 'lchild' and return
		 **************************************************************/
		if (i_rchild > i_base) {
			nodes[i_lchild] = next;
			nodes[i_next]	= lchild;
			return;
		}

		void *rchild = nodes[i_rchild];

		/* compare left child with right child:
		 *
		 * if 'lchild' belongs above 'rchild'...
		 **************************************************************/
		if (compare(lchild, rchild)) {
			/* place 'lchild' at 'i_next' and continue recursion
			 * down left branch
			 ******************************************************/
			nodes[i_next] = lchild;
			do_shift(nodes, next, i_lchild, i_base, compare);

		} else {
			/* place 'rchild' at 'i_next' and continue recursion
			 * down right branch
			 ******************************************************/
			nodes[i_next] = rchild;
			do_shift(nodes, next, i_rchild, i_base, compare);
		}
		return;
	}

	/* if base level of heap has been reached (no more children), place
	 * 'next' above 'lchild' (new base/last element) and return
	 **********************************************************************/
	if (i_rchild > i_base) {
		nodes[i_next] = next;
		return;
	}

	void *rchild = nodes[i_rchild];

	/* compare 'next' with right child:
	 *
	 * if 'rchild' belongs above 'next'...
	 **********************************************************************/
	if (compare(rchild, next)) {
		nodes[i_next] = rchild;
		do_shift(nodes, next, i_rchild, i_base, compare);
		return;
	}

	/* otherwise, 'next' belongs above lchild and rchild: place at 'i_next'
	 * and return
	 **********************************************************************/
	nodes[i_next] = next;
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

	void **nodes = heap->nodes;
	char buffer[256];

	for (size_t i = 1ul; i <= count; ++i) {
		node_to_string(buffer, nodes[i]);
		printf("nodes[%zu]:\n%s\n", i, buffer);
	}
}




/* heapsort
 ******************************************************************************/
void bheap_sort(void **array,
		const size_t length,
		int (*compare)(const void *,
			       const void *))
{
	void **const nodes = &array[-1];
	ptrdiff_t i = length;
	void *next;

	while (i > 1l) {
		next = nodes[i];
		--i;
		do_shift(nodes, next, i, length, compare);
	}
}


/* convienience, misc
 ******************************************************************************/
extern inline struct BHeap *array_into_bheap(void **array,
					     const size_t width,
					     const size_t length,
					     int (*compare)(const void *,
							    const void *));
