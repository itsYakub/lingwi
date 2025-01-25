// + -------------------------------------- +
// |  _   _                              _  |
// | | | (_)  _ __     __ _  __      __ (_) |
// | | | | | | '_ \   / _` | \ \ /\ / / | | |
// | | | | | | | | | | (_| |  \ V  V /  | | |
// | |_| |_| |_| |_|  \__, |   \_/\_/   |_| |
// |                  |___/                 |
// + -------------------------------------- +
// | Translation command-line utility.      |
// + -------------------------------------- +

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
LINGWI_API char		*lingwi_strcdup(const char *str, char c);
LINGWI_API char		*lingwi_strjoin(char *s1, char *s2);

#endif /* LINGWI_UTILS_H */
