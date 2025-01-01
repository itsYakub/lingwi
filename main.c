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
// | Licence: bottom of the file            |
// + -------------------------------------- +

// SECTION: Inclusions
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <getopt.h>
#include <unistd.h>
#include <curl/curl.h>

// SECTION: Macro definitions
#define LINGWI_VERSION "1.2"
#define LINGWI_IO_SIZE 5001
#define LINGWI_LANGCODE_SIZE 9
#define LINGWI_APINAME_SIZE 9
#define LINGWI_URL_SIZE (LINGWI_IO_SIZE + 128)
#define LINGWI_DATA_SIZE (LINGWI_IO_SIZE + LINGWI_LANGCODE_SIZE + 13)
#define LINGWI_ISO639_1_LANGCODE_COUNT 184
#define LINGWI_TRANSLATION_ENGINES_COUNT 1

// SECTION: Function declarations
// SECTION: Function: cURL
size_t callback(char* ptr, size_t size, size_t nmemb, char* data);

// SECTION: Function: Program arguments
char *get_system_language(char lang[8]);
int process_options(int ac, char** av, char* sl, char* tl, char* engine, char* apikey, char* text);

// SECTION: Function: Translation
int translate(char* engine, char* apikey, char* dest, char* src, char* sl, char* tl);
int translate_engine_google(char* dest, char* src, char* sl, char* tl);

// SECTION: Function: Validation
int lang_valid(char* lang);
int engine_valid(char* engine);
int fexist(char* fpath);

// SECTION: Function: Printing
int print(char* str);

// SECTION: Global variables
static struct option long_option_s[] = {
    { "source-language", required_argument, NULL, 's' },
    { "target-language", required_argument, NULL, 't' },
    { "engine", required_argument, NULL, 'e' },
    { "api-key", required_argument, NULL, 'a' },
    { "version", no_argument, NULL, 'v' },
    { "help", no_argument, NULL, 'h' },
    { "list-api", no_argument, NULL, 'l' },
    { 0, 0, 0, 0 }
};

// Source:
// - https://en.wikipedia.org/wiki/ISO_639
// - https://www.iso.org/iso-639-language-code
// - https://cloud.google.com/translate/docs/languages
// Array generated via ChatGPT
static const char* ISO_639_1_LANGUAGE_CODES[LINGWI_ISO639_1_LANGCODE_COUNT] = {
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
    "za", "zh", "zu"
};

static const char* TRANSLATION_ENGINES[LINGWI_TRANSLATION_ENGINES_COUNT] = {
    /* Add your translation engines here ... */
    "google"
};

int main(int ac, char** av) {
    char input[LINGWI_IO_SIZE] = { 0 }; 
    char output[LINGWI_IO_SIZE] = { 0 };

    char sl[LINGWI_LANGCODE_SIZE] = "auto";
    char tl[LINGWI_LANGCODE_SIZE];
    char engine[LINGWI_APINAME_SIZE] = "google";
    char apikey[128];

	get_system_language(tl);
    process_options(ac, av, sl, tl, engine, apikey, input);
    translate(engine, apikey, output, input, sl, tl);

    print(output);

    return (EXIT_SUCCESS);
}

size_t callback(char* ptr, size_t size, size_t nmemb, char* data) {
    size_t bytes = size * nmemb;

    strcpy(data, ptr);

    return (bytes);
}

char *get_system_language(char lang[8]) {
	char *envlang = getenv("LANG");

	return (strncpy(lang, envlang, strchr(envlang, '_') - envlang));
}

int process_options(int ac, char** av, char* sl, char* tl, char* engine, char* apikey, char* text) {
    // Processing program options
    char opt;
    while ((opt = getopt_long(ac, av, ":s:t:e:a:vVhl", long_option_s, NULL)) != -1) {
        switch (opt) {
            case 's': {
                if(!lang_valid(optarg)) {
                    fprintf(stderr, "[ ERR ] Invalid language code: %s\n", optarg);
                    exit(EXIT_FAILURE);
                }

                strcpy(sl, optarg);
            } break;

            case 't': {
                if(!lang_valid(optarg)) {
                    fprintf(stderr, "[ ERR ] Invalid language code: %s\n", optarg);
                    exit(EXIT_FAILURE);
                }

                strcpy(tl, optarg);
            } break;

            case 'e': {
                if(!engine_valid(optarg)) {
                    fprintf(stderr, "[ ERR ] Invalid engine: %s\n", optarg);
                    exit(EXIT_FAILURE);
                }
                
                strcpy(engine, optarg);
            } break;

            case 'a': {
                strcpy(apikey, optarg);
            } break;

            case 'h':
                fprintf(stdout, "\033[1mlingwi\033[0m - translation command-line utility\n");
                fprintf(stdout, "  \033[1m-s\033[0m, \033[1m--source-language\033[0m    select a source language (translation from...) (DEFAULT: auto)\n");
                fprintf(stdout, "  \033[1m-t\033[0m, \033[1m--target-language\033[0m    select a target language (translation to...) (DEFAULT: {system language (check 'env')})\n");
                fprintf(stdout, "  \033[1m-e\033[0m, \033[1m--engine\033[0m             select a translation engine (DEFAULT: google)\n");
                fprintf(stdout, "  \033[1m-a\033[0m, \033[1m--api-key\033[0m            select an api key (DEFAULT: none)\n");
                fprintf(stdout, "  \033[1m-v\033[0m, \033[1m--version\033[0m            display a version (%s)\n", LINGWI_VERSION);
                fprintf(stdout, "  \033[1m-h\033[0m, \033[1m--help\033[0m               display a help message\n");
                fprintf(stdout, "  \033[1m-l\033[0m, \033[1m--list-api\033[0m           list all the available API's\n\n");
                fprintf(stdout, "\033[1mAll language codes:\033[0m\n   https://en.wikipedia.org/wiki/List_of_ISO_639_language_codes\n"); 
                
                exit(EXIT_SUCCESS);

            case 'v':
                fprintf(stdout, "%s\n", LINGWI_VERSION);
                exit(EXIT_SUCCESS);

			case 'l':
				fprintf(stdout, "\033[1mlingwi translation APIs:\033[0m\n");
				fprintf(stdout, "> Google:\n");
				fprintf(stdout, "	- api-key: optional\n");
				fprintf(stdout, "	- token limit: 5000\n");
				
				exit(EXIT_SUCCESS);	

            default:
                exit(EXIT_FAILURE);
        }
    }

    // Processing program inputs
    char str[LINGWI_IO_SIZE];
    int is_str_processed = 0;
    for (int i = optind; i < ac; i++) {
        strcpy(str, av[i]);
        is_str_processed = 1;
    }
    
    // There was no input string provided
    // Thus we can check if input wasn't actually piped to the program
    if (!is_str_processed) {
        if (!isatty(fileno(stdin))) {
            int i = 0;
            while ((text[i++] = getchar()) != -1);
            text[i - 1] = 0;
        }

        else {
            fprintf(stderr, "%s [OPTIONS] [INPUT] ... \n", av[0]);
            exit(EXIT_FAILURE);
        }
    }

    else {
        // If the input string is a file name ...
        // Treat the input as a file name, thus the final input will be the content of this file
        if (fexist(str)) {
            FILE* f = fopen(str, "rb");
            if (!f) {
                fprintf(stderr, "Invalid file");
                exit(EXIT_FAILURE);
            }

            fread(text, sizeof(char), LINGWI_IO_SIZE, f);
            fclose(f);

            if (text == NULL || *text == 0) {
                fprintf(stderr, "[ ERR ] Empty file input\n");
                exit(EXIT_FAILURE);
            }

        } else { // Otherwise the input is a string, thus we just copy the 'str' to 'text'
            strcpy(text, str);
        
            if (*text == 0 || text == NULL) {
                fprintf(stderr, "[ ERR ] Invalid text input\n");
                exit(EXIT_FAILURE);
            }
        }
    } 
    return (1);
}

int translate(char* engine, char* apikey, char* dest, char* src, char* sl, char* tl) {
    if (strcmp(engine, TRANSLATION_ENGINES[0]) == 0) { // Engine: google
		(void) apikey;
        translate_engine_google(dest, src, sl, tl);
    } else {
        fprintf(stderr, "[ ERR ] Invalid engine\n");
    }

    return (1);
}

int translate_engine_google(char* dest, char* src, char* sl, char* tl) {
    // Setup
    CURL* curl;
	CURLcode ccode;
	char cinput[LINGWI_DATA_SIZE];

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "[ ERR ] Could not initialize cURL\n");
        exit(EXIT_FAILURE);
    }

    char* text = curl_easy_escape(curl, src, 0);
    if (!text) {
        fprintf(stderr, "[ ERR ] Modified translation string is invalid\n");

        curl_easy_cleanup(curl);
        exit(EXIT_FAILURE);
    }

    char url[LINGWI_URL_SIZE];
    snprintf(
            url,
            LINGWI_URL_SIZE,
            "https://translate.googleapis.com/translate_a/t?client=gtx&sl=%s&tl=%s&dt=t&q=%s",
            sl,
            tl,
            text
    );

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, cinput);
    curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L);

    // Translation
    ccode = curl_easy_perform(curl);
    
    if (ccode != CURLE_OK) {
        fprintf(stderr, "[ ERR ] %s\n", curl_easy_strerror(ccode));

        curl_easy_cleanup(curl);
		curl_free(text);
        exit(EXIT_FAILURE);
    }

    curl_easy_cleanup(curl);
	curl_free(text);

    // Parsing
    if (!sscanf(cinput, "[[\"%5000[^\"]\", \"%8[a-z]\"]]", dest, tl) || !dest) {
        fprintf(stderr, "[ ERR ] Parsing failed\n");
        exit(EXIT_FAILURE);
    }

    return (1);
}

int lang_valid(char* lang) {
    for (int i = 0; i < LINGWI_ISO639_1_LANGCODE_COUNT; i++) {
        if (strcmp(lang, ISO_639_1_LANGUAGE_CODES[i]) == 0)
            return (1);
    }    

    return (0);
}

int engine_valid(char* engine) {
    for (int i = 0; i < LINGWI_TRANSLATION_ENGINES_COUNT; i++) {
        if (strcmp(engine, TRANSLATION_ENGINES[i]) == 0)
            return (1);
    }

    return (0);
}

int fexist(char* fpath) {
    if (access(fpath, F_OK) == 0)
        return (1);
    return (0);
}

int print(char* str) {
    int i;
    for (i = 0; i < (int) strlen(str); i++) {
        if (str[i] == '\\' && str[i + 1] == 'n') {
            fputc('\n', stdout);
            i++;
        
            continue ;
        } 
        
        else if (str[i] == '\\' && str[i] == '\\' && str[i + 2] == 'n') {
            fputc('\n', stdout);
            i += 2;
        
            continue ;
        }

        fputc(str[i], stdout);
    }

    if (str[i - 2] != '\\' && str[i - 1] != 'n')
        fputc('\n', stdout);

    return (1);
}

// + ------------------------------------------------------------------------------ +
// | LICENCE                                                                        |
// + ------------------------------------------------------------------------------ +
// | Permission is hereby granted, free of charge, to any person obtaining a copy   |
// | of this software and associated documentation files (the "Software"), to deal  |
// | in the Software without restriction, including without limitation the rights   |
// | to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      |
// | copies of the Software, and to permit persons to whom the Software is          |
// | furnished to do so, subject to the following conditions:                       |
// |                                                                                |
// | The above copyright notice and this permission notice shall be included in all |
// | copies or substantial portions of the Software.                                |
// |                                                                                |
// | THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,                |
// | EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF             |
// | MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.         |
// | IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,    |
// | DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR          |
// | OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE  |
// | OR OTHER DEALINGS IN THE SOFTWARE.                                             |
// + ------------------------------------------------------------------------------ +
