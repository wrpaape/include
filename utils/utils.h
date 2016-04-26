#ifndef UTILS_UTILS_H_
#define UTILS_UTILS_H_
/* EXTERNAL DEPENDENCIES ▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼ */

#include <stdlib.h>	/* m/c/realloc, free, exit */
#include <stddef.h>	/* ptrdiff_t */
#include <stdio.h>	/* printf, fprintf */
#include <errno.h>	/* errno */
#include <string.h>	/* strerror */
#include <limits.h>	/* max word value */

/* EXTERNAL DEPENDENCIES ▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲ */

/* FUNCTION-LIKE MACROS ▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼ */

#define BIT_SIZE(TYPE) (sizeof(TYPE) * CHAR_BIT)

#define EXIT_ON_FAILURE(format, ...)					\
do {									\
	fprintf(stderr,							\
		"\n\e[31m\e[5mERROR:\e[25m\t\e[4m" format "\e[24m\n\n"	\
		"errno:\t%s\n\n"					\
		"file:\t%s\n\n"						\
		"func:\t%s\n\n"						\
		"line:\t%d\e[0m\n",					\
		##__VA_ARGS__,						\
		strerror(errno), __FILE__, __func__, __LINE__);		\
	exit(EXIT_FAILURE);						\
} while (0)

#define HANDLE_MALLOC(ptr, size)					\
do {									\
	ptr = malloc(size);						\
	if (ptr == NULL)						\
		EXIT_ON_FAILURE("failed to allocate %lu bytes", size);	\
} while (0)

#define HANDLE_CALLOC(ptr, count, size)					\
do {									\
	ptr = calloc(count, size);					\
	if (ptr == NULL)						\
		EXIT_ON_FAILURE("failed to allocate %lu blocks of %lu"	\
				"bytes", count, size);			\
} while (0)

#define HANDLE_REALLOC(ptr, size)					\
do {									\
	ptr = realloc(ptr, size);					\
	if (ptr == NULL)						\
		EXIT_ON_FAILURE("failed to reallocate memory at '" #ptr	\
				"' to %lu bytes", size);		\
} while (0)

/* FUNCTION-LIKE MACROS ▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲ */

inline size_t next_pow_two(const size_t num)
{
	return 1lu << (BIT_SIZE(size_t) - __builtin_clzl(num - 1lu));
}

inline void mem_swap(void *restrict x,
		     void *restrict y,
		     const size_t width)
{
	char buffer[width];
	memcpy(&buffer[0l], x,		 width);
	memcpy(x,	    y,		 width);
	memcpy(y,	    &buffer[0l], width);
}
#endif /* ifndef UTILS_UTILS_H_ */

