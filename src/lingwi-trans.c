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
#include "lingwi.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <curl/easy.h>

static size_t	__lingwi_trans_callback(char* ptr, size_t size, size_t nmemb, char* data);
static i32		__lingwi_trans_parse(char **dst, char **src);
static char		*__lingwi_create_url(char *input);

LINGWI_API i32	lingwi_translate(char **input, char **output) {
	CURLcode	ccode;
	CURL		*curl;
	char		*input_escaped;
	char		*copt_url;
	char		*copt_out;

	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
	if (!curl) {
		fprintf(stderr, "ERR.: Could not initialize cURL\n");
		return (0);
	}
	input_escaped = curl_easy_escape(curl, *input, 0);
	if (!input_escaped) {
		fprintf(stderr, "ERR.: Input escaping failed\n");
		curl_easy_cleanup(curl);
		return (0);
	}
	copt_url = __lingwi_create_url(input_escaped);
	copt_out = (char *) LINGWI_CALLOC(5001, sizeof(char));
	curl_easy_setopt(curl, CURLOPT_URL, copt_url);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, __lingwi_trans_callback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, copt_out);
	curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L);
	ccode = curl_easy_perform(curl);
	if (ccode != CURLE_OK) {
		fprintf(stderr, "ERR>: %s\n", curl_easy_strerror(ccode));
		curl_free(input_escaped);
		curl_easy_cleanup(curl);
		LINGWI_FREE(copt_url);
		LINGWI_FREE(copt_out);
		return (0);
	}
	curl_free(input_escaped);
	curl_easy_cleanup(curl);
	LINGWI_FREE(copt_url);
	if (!__lingwi_trans_parse(output, &copt_out)) {
		LINGWI_FREE(copt_out);
		return (0);
	}
	LINGWI_FREE(copt_out);
	return (1);
}

static size_t	__lingwi_trans_callback(char* ptr, size_t size, size_t nmemb, char* data) {
	return (lingwi_strlcpy(data, ptr, size * nmemb + 1));
}

static i32		__lingwi_trans_parse(char **dst, char **src) {
	*dst = (char *) calloc(strlen(*src) + 1, sizeof(char));
	if (!*dst) {
		return (0);
	}
	if (!sscanf(*src, "[[\"%5000[^\"]\", \"%8[a-z]\"]]", *dst, lingwi_get_tlang())) {
		if (!sscanf(*src, "[\"%5000[^\"]]", *dst)) {
			fprintf(stderr, "ERR.: Parsing error\n");
			return (0);
		}
	}
	return (1);
}

static char		*__lingwi_create_url(char *input) {
	char	*url;

	url = (char *) calloc(1, 1);
	url = lingwi_strjoin(url, "https://translate.googleapis.com/translate_a/t?client=gtx&sl=");
	url = lingwi_strjoin(url, lingwi_get_slang());
	url = lingwi_strjoin(url, "&tl=");
	url = lingwi_strjoin(url, lingwi_get_tlang());
	url = lingwi_strjoin(url, "&dt=t&q=");
	url = lingwi_strjoin(url, input);
	return (url);
}
