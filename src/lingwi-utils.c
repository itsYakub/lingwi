#include "lingwi-utils.h"

#include <string.h>

#ifdef BSD
# include <bsd/string.h>
#endif /* BSD */

LINGWI_API size_t	lingwi_strlcpy(char *dst, const char *src, size_t s) {

#ifdef BSD
	
	return (strlcpy(dst, src, s));

#else

	const char	*sptr;

	sptr = src;
	if (s) {
		while (--s) {
			*dst++ = *sptr;
			if (!*sptr++) {
				break;
			}
		}
	}
	if (!s) {
		*dst = '\0';
	}
	return (strlen(src));	

#endif

}

LINGWI_API char		*lingwi_strdup(const char *str) {
	char	*res;
	size_t	iter;

	res = LINGWI_MALLOC((strlen(str) + 1) * sizeof(char));
	if (!res) {
		return (NULL);
	}
	for (iter = 0; str[iter]; iter++) {
		res[iter] = str[iter];
	}
	res[iter] = '\0';
	return (res);
}
