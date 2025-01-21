#ifndef LINGWI_UTILS_H
# define LINGWI_UTILS_H

/*	SECTION:
 *		Header Inclusions
 * */

#include <stddef.h>
#include "lingwi.h"

/*	SECTION:
 *		Function Declarations
 * */

LINGWI_API size_t	lingwi_strlcpy(char *dst, const char *src, size_t s);
LINGWI_API char		*lingwi_strdup(const char *str);

#endif /* LINGWI_UTILS_H */
