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

LINGWI_API char		*lingwi_strjoin(char *s1, char *s2) {
	char	*str;

	str = (char *) LINGWI_CALLOC(strlen(s1) + strlen(s2) + 1, sizeof(char));
	if (!str) {
		return (NULL);
	}
	strcpy(str, s1);
	strcat(str, s2);
	LINGWI_FREE(s1);
	return (str);
}
