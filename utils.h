/******************************************************************************/
/* Tema 2 Protocoale de Comunicatie (Aprilie 2015)                            */
/******************************************************************************/

#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdlib.h>
#include <stdio.h>

#define loop while(1)

#define eprintf(str, ...) \
	fprintf (stderr, "[SIM ERR]: " str "\n", ##__VA_ARGS__)

#define bug_on(cond, str, ...)						\
	do {								\
		if (cond) {						\
			fprintf(stderr, "(%s, %d):" str "\n",		\
				__FILE__, __LINE__, ##__VA_ARGS__);	\
			perror("");					\
			exit(EXIT_FAILURE);				\
		}							\
	} while(0)

#define DIE(cond, str, ...)						\
	do {								\
		if (cond) {						\
			fprintf(stderr, "(%s, %d):" str "\n",		\
				__FILE__, __LINE__, ##__VA_ARGS__);	\
			exit(EXIT_FAILURE);				\
		}							\
	} while(0)

#endif /* _UTILS_H_ */