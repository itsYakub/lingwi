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
#include <curl/curl.h>

// SECTION: Macro definitions
#define LINGWI_VERSION "1.0"
#define LINGWI_IO_SIZE 5001
#define LINGWI_URL_SIZE (LINGWI_IO_SIZE + 128)
#define LINGWI_ISO639_1_LANGCODE_COUNT 184

// SECTION: Function declarations
size_t callback(char* ptr, size_t size, size_t nmemb, char* data);

int process_options(int ac, char** av, char* sl, char* tl, char* engine, char* apikey, char* text);

int translate(char* engine, char* apikey, char* dest, char* src, char* sl, char* tl);
int translate_engine_google(char* dest, char* src, char* sl, char* tl);

int lang_valid(char* lang);

// SECTION: Global variables
static struct option long_option_s[] = {
    { "source-language", required_argument, NULL, 's' },
    { "target-language", required_argument, NULL, 't' },
    { "engine", required_argument, NULL, 'e' },
    { "api-key", required_argument, NULL, 'a' },
    { "version", no_argument, NULL, 'v' },
    { "help", no_argument, NULL, 'h' },
    { 0, 0, 0, 0 }
};

// Source:
// - https://en.wikipedia.org/wiki/ISO_639
// - https://www.iso.org/iso-639-language-code
// Array generated via ChatGPT
static const char* ISO_639_1_LANGUAGE_CODES[LINGWI_ISO639_1_LANGCODE_COUNT] = {
    "aa", "ab", "ae", "af", "ak", "am", "an", "ar", "as", "av",
    "ay", "az", "ba", "be", "bg", "bh", "bi", "bm", "bn", "bo",
    "br", "bs", "ca", "ce", "ch", "co", "cr", "cs", "cu", "cv",
    "cy", "da", "de", "dv", "dz", "ee", "el", "en", "eo", "es",
    "et", "eu", "fa", "ff", "fi", "fj", "fo", "fr", "fy", "ga",
    "gd", "gl", "gn", "gu", "gv", "ha", "he", "hi", "ho", "hr",
    "ht", "hu", "hy", "hz", "ia", "id", "ie", "ig", "ii", "ik",
    "io", "is", "it", "iu", "ja", "jv", "ka", "kg", "ki", "kj",
    "kk", "kl", "km", "kn", "ko", "kr", "ks", "ku", "kv", "kw",
    "ky", "la", "lb", "lg", "li", "ln", "lo", "lt", "lu", "lv",
    "mg", "mh", "mi", "mk", "ml", "mn", "mr", "ms", "mt", "my",
    "na", "nb", "nd", "ne", "ng", "nl", "nn", "no", "nr", "nv",
    "ny", "oc", "oj", "om", "or", "os", "pa", "pi", "pl", "ps",
    "pt", "qu", "rm", "rn", "ro", "ru", "rw", "sa", "sc", "sd",
    "se", "sg", "si", "sk", "sl", "sm", "sn", "so", "sq", "sr",
    "ss", "st", "su", "sv", "sw", "ta", "te", "tg", "th", "ti",
    "tk", "tl", "tn", "to", "tr", "ts", "tt", "tw", "ty", "ug",
    "uk", "ur", "uz", "ve", "vi", "vo", "wa", "wo", "xh", "yi",
    "yo", "za", "zh", "zu"
};

int main(int ac, char** av) {
    char input[LINGWI_IO_SIZE] = { 0 }; 
    char output[LINGWI_IO_SIZE] = { 0 };

    char sl[8] = "en";
    char tl[8] = "en";
    char engine[8] = "google";
    char apikey[128];

    if(ac <= 1) {
        fprintf(stderr, "%s -[OPTIONS] [TEXT] ... \n", av[0]);
        return EXIT_FAILURE;
    }

    process_options(ac, av, sl, tl, engine, apikey, input);
    translate(engine, apikey, output, input, sl, tl);

    fprintf(stdout, "%s\n", output);

    return EXIT_SUCCESS;
}

size_t callback(char* ptr, size_t size, size_t nmemb, char* data) {
    size_t bytes = size * nmemb;

    strcpy(data, ptr);

    return bytes;
}

int process_options(int ac, char** av, char* sl, char* tl, char* engine, char* apikey, char* text) {
    // Processing program options
    char opt;
    while((opt = getopt_long(ac, av, "stavh", long_option_s, NULL)) != -1) {
        switch(opt) {
            case 's': {
                strcpy(sl, optarg);
            } break;

            case 't': {
                strcpy(tl, optarg);
            } break;

            case 'e': {
                strcpy(engine, optarg);
            } break;

            case 'a': {
                strcpy(apikey, optarg);
            } break;

            case 'h':
                fprintf(stdout, "\033[1mlingwi\033[0m - translation command-line utility\n");
                fprintf(stdout, "  \033[1m--s\033[0m, \033[1m--source-language\033[0m - select a source language (translation from...) (DEFAULT: en)\n");
                fprintf(stdout, "  \033[1m--t\033[0m, \033[1m--target-language\033[0m - select a target language (translation to...) (DEFAULT: en)\n");
                fprintf(stdout, "  \033[1m--e\033[0m, \033[1m--engine\033[0m - select a translation engine (DEFAULT: google)\n");
                fprintf(stdout, "  \033[1m--a\033[0m, \033[1m--api-key\033[0m - select an api key (DEFAULT: none)\n");
                fprintf(stdout, "  \033[1m--v\033[0m, \033[1m--version\033[0m - display a version (%s)\n", LINGWI_VERSION);
                fprintf(stdout, "  \033[1m--h\033[0m, \033[1m--help\033[0m - display a help message\n\n");
                fprintf(stdout, "\033[1mExample language codes:\033[0m\n   | en | es | ja | de | ru | pl | bo |\n"); 
                fprintf(stdout, "\033[1mAll language codes:\033[0m\n   https://en.wikipedia.org/wiki/List_of_ISO_639_language_codes\n"); 
                
                exit(EXIT_SUCCESS);

            case 'v':
                fprintf(stdout, "%s\n", LINGWI_VERSION);
                exit(EXIT_SUCCESS);

            default:
                exit(EXIT_FAILURE);
        }
    }

    // Processing program inputs
    for(int i = optind; i < ac; i++) {
        strcat(text, av[i]);
    }

    assert(*text != 0);

    return 1;
}

int translate(char* engine, char* apikey, char* dest, char* src, char* sl, char* tl) {
    assert(lang_valid(sl) == 1);
    assert(lang_valid(tl) == 1);

    if(strcmp(engine, "google") == 0) {
        (void) apikey;
        translate_engine_google(dest, src, sl, tl);
    } else {
        fprintf(stderr, "[ ERR ] Invalid engine\n");
    }

    return 1;
}

int translate_engine_google(char* dest, char* src, char* sl, char* tl) {
    // Setup
    CURL* curl;
    CURLcode ccode;

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    assert(curl != NULL);

    char* text = curl_easy_escape(curl, src, 0);
    assert(text != NULL);

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
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, dest);

    // Translation
    ccode = curl_easy_perform(curl);
    assert(ccode == CURLE_OK);
    curl_easy_cleanup(curl);
   
    // Parsing
    // TODO: Modifing the source language leaves the '[' and '"' signs
    sscanf(dest, "[\"%5000[^\"]", dest);

    return 1;
}

int lang_valid(char* lang) {
    for(int i = 0; i < LINGWI_ISO639_1_LANGCODE_COUNT; i++) {
        if(strcmp(lang, ISO_639_1_LANGUAGE_CODES[i]) == 0)
            return 1;
    }    

    return 0;
}
