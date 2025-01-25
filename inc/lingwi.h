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

#ifndef LINGWI_H
# define LINGWI_H

/*	SECTION:
 *		Macro Definitions
 * */

# ifndef LINGWI_API
#  ifdef __cplusplus
#   define LINGWI_API	extern "C"
#  else
#   define LINGWI_API	extern
#  endif
# endif

# ifndef LINGWI_TRANS_STR_LENGTH
#  define LINGWI_TRANS_STR_LENGTH 5001
# endif /* LINGWI_TRANS_STR_LENGTH */

# define LINGWI_VERSION			"1.3"
# define LINGWI_VERSION_MAJOR	1
# define LINGWI_VERSION_MINOR	3

# define LINGWI_UNUSED(x) ( (void) x )

# ifndef LINGWI_MALLOC
#  include <stdlib.h>
#  define LINGWI_MALLOC(siz) malloc(siz)
# endif /* LINGWI_MALLOC */

# ifndef LINGWI_CALLOC
#  include <stdlib.h>
#  define LINGWI_CALLOC(nmemb, siz) calloc(nmemb, siz)
# endif /* LINGWI_CALLOC */

# ifndef LINGWI_REALLOC
#  include <stdlib.h>
#  define LINGWI_REALLOC(ptr, siz) realloc(ptr, siz)
# endif /* LINGWI_REALLOC */

# ifndef LINGWI_FREE
#  include <stdlib.h>
#  define LINGWI_FREE(ptr) free(ptr)
# endif /* LINGWI_FREE */

/*	SECTION:
 *		Typedefs
 * */

# ifndef LINGWI_DISABLE_INT_TYPEDEFS

# include <stdint.h>

typedef uint8_t		u8;
typedef int8_t		i8;
typedef uint16_t	u16;
typedef int16_t		i16;
typedef uint32_t	u32;
typedef int32_t		i32;
typedef uint64_t	u64;
typedef int64_t		i64;

# endif /* LINGWI_DISABLE_INT_TYPEDEFS */

/*	SECTION:
 *		Function declarations
 * */

LINGWI_API i32	lingwi_set_engine(const char *engine);
LINGWI_API i32	lingwi_set_slang(const char *code);
LINGWI_API char	*lingwi_get_slang(void);
LINGWI_API i32	lingwi_set_tlang(const char *code);
LINGWI_API char	*lingwi_get_tlang(void);
LINGWI_API i32	lingwi_set_apikey(const char *key);
LINGWI_API i32	lingwi_translate(char **input, char **output);
LINGWI_API i32	lingwi_get_syslang(char *dst);
LINGWI_API i32	lingwi_opt(i32 ac, char **av, char **input);
LINGWI_API i32	lingwi_default_settings(void);

#endif /* LINGWI_H */
