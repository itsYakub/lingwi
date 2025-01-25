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

#include "lingwi.h"
#include "lingwi-utils.h"

#include <string.h>

static const char	*LINGW_ISO_639_1_LANGCODES[] = {
    "aa", "ab", "ae", "af", "ak", "am", "an", "ar", "as", "av", "ay", "az", 
    "ba", "be", "bg", "bh", "bi", "bm", "bn", "bo", "br", "bs", 
    "ca", "ce", "ch", "co", "cr", "cs", "cu", "cv", "cy", 
    "da", "de", "dv", "dz", 
    "ee", "el", "en", "eo", "es", "et", "eu", 
    "fa", "ff", "fi", "fj", "fo", "fr", "fy", 
    "ga", "gd", "gl", "gn", "gu", "gv", 
    "ha", "he", "hi", "ho", "hr", "ht", "hu", "hy", "hz", 
    "ia", "id", "ie", "ig", "ii", "ik", "io", "is", "it", "iu", 
    "ja", "jv", 
    "ka", "kg", "ki", "kj", "kk", "kl", "km", "kn", "ko", "kr", "ks", "ku", "kv", "kw", "ky", 
    "la", "lb", "lg", "li", "ln", "lo", "lt", "lu", "lv",
    "mg", "mh", "mi", "mk", "ml", "mn", "mr", "ms", "mt", "my",
    "na", "nb", "nd", "ne", "ng", "nl", "nn", "no", "nr", "nv", "ny", 
    "oc", "oj", "om", "or", "os", 
    "pa", "pi", "pl", "ps", "pt", 
    "qu", 
    "rm", "rn", "ro", "ru", "rw", 
    "sa", "sc", "sd", "se", "sg", "si", "sk", "sl", "sm", "sn", "so", "sq", "sr", "ss", "st", "su", "sv", "sw", 
    "ta", "te", "tg", "th", "ti", "tk", "tl", "tn", "to", "tr", "ts", "tt", "tw", "ty", 
    "ug", "uk", "ur", "uz", 
    "ve", "vi", "vo", 
    "wa", "wo", 
    "xh", 
    "yi", "yo", 
    "za", "zh", "zu",
	NULL
};

static char	LINGWI_SOURCE_LANGUAGE[8];
static char	LINGWI_TARGET_LANGUAGE[8];
static char	LINGWI_ENGINE[64];
static char	LINGWI_APIKEY[128];

static i32	__lingwi_validate_langcode(const char *code);

LINGWI_API i32	lingwi_set_engine(const char *engine) {
	lingwi_strlcpy(LINGWI_ENGINE, engine, 128);
	return (1);
}

LINGWI_API i32	lingwi_set_slang(const char *code) {
	if (__lingwi_validate_langcode(code)) {
		lingwi_strlcpy(LINGWI_SOURCE_LANGUAGE, code, 8);
		return (1);
	}
	else if (!strcmp(code, "auto")) {
		lingwi_strlcpy(LINGWI_SOURCE_LANGUAGE, "auto", 8);
		return (1);
	}
	return (0);
}

LINGWI_API char	*lingwi_get_slang(void) {
	return (LINGWI_SOURCE_LANGUAGE);
}

LINGWI_API i32	lingwi_set_tlang(const char *code) {
	if (__lingwi_validate_langcode(code)) {
		lingwi_strlcpy(LINGWI_TARGET_LANGUAGE, code, 8);
		return (1);
	}
	return (0);
}

LINGWI_API char	*lingwi_get_tlang(void) {
	return (LINGWI_TARGET_LANGUAGE);
}

LINGWI_API i32	lingwi_set_apikey(const char *key) {
	lingwi_strlcpy(LINGWI_APIKEY, key, 128);
	return (1);
}

LINGWI_API i32	lingwi_get_syslang(char *dst) {
	char	*env;

	env = getenv("LANG");
	if (!env) {
		return (0);
	}
	strlcpy(dst, env, 3);
	if (!__lingwi_validate_langcode(dst)) {
		return (0);
	}
	return (1);
}

static i32	__lingwi_validate_langcode(const char *code) {
	for (size_t i = 0; LINGW_ISO_639_1_LANGCODES[i]; i++) {
		if (!strcmp(LINGW_ISO_639_1_LANGCODES[i], code)) {
			return (1);
		}
	}
	return (0);
}

LINGWI_API i32	lingwi_default_settings(void) {
	char	syslang[8];

	lingwi_set_apikey("");
	lingwi_set_engine("google");
	lingwi_set_slang("auto");
	lingwi_get_syslang(syslang);
	lingwi_set_tlang(syslang);
	return (1);
}
