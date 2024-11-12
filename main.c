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

// SECTION: Inclusions
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <getopt.h>
#include <curl/curl.h>

// SECTION: Macro definitions
#define LINGWI_VERSION "1.0"
#define LINGWI_IO_SIZE 5000
#define LINGWI_URL_SIZE (LINGWI_IO_SIZE + 128)

// SECTION: Function declarations
size_t callback(char* ptr, size_t size, size_t nmemb, char* data);
int process_options(int ac, char** av, char* sl, char* tl, char* engine, char* text);

int translate(char* engine, char* dest, char* src, char* sl, char* tl);
int translate_engine_google(char* dest, char* src, char* sl, char* tl);

int parse(char* str);

// SECTION: Global variables
static struct option long_option_s[] = {
    { "source-language", required_argument, NULL, 's' },
    { "target-language", required_argument, NULL, 't' },
    { "engine", required_argument, NULL, 'e' },
    { "version", no_argument, NULL, 'v' },
    { "help", no_argument, NULL, 'h' },
    { 0, 0, 0, 0 }
};

int main(int ac, char** av) {
    char input[LINGWI_IO_SIZE]; 
    char output[LINGWI_IO_SIZE];

    char sl[8] = "auto";
    char tl[8] = "en";
    char engine[8] = "google";

    process_options(ac, av, sl, tl, engine, input);
    translate(engine, output, input, sl, tl);
    parse(output);

    fprintf(stdout, "%s\n", output);

    return 0;
}

size_t callback(char* ptr, size_t size, size_t nmemb, char* data) {
    size_t bytes = size * nmemb;

    strcpy(data, ptr);

    return bytes;
}

int process_options(int ac, char** av, char* sl, char* tl, char* engine, char* text) {
    // Processing program options
    char opt;
    while((opt = getopt_long(ac, av, "stvh", long_option_s, NULL)) != -1) {
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

            case 'h':
                fprintf(stdout, "\033[1mlingwi\033[0m - translation command-line utility\n");
                fprintf(stdout, "  \033[1m--s\033[0m, \033[1m--source-language\033[0m - select a source language (translation from...) (DEFAULT: auto)\n");
                fprintf(stdout, "  \033[1m--t\033[0m, \033[1m--target-language\033[0m - select a target language (translation to...) (DEFAULT: en)\n");
                fprintf(stdout, "  \033[1m--e\033[0m, \033[1m--engine\033[0m - select a translation engine (DEFAULT: google)\n");
                fprintf(stdout, "  \033[1m--v\033[0m, \033[1m--version\033[0m - display a version (%s)\n", LINGWI_VERSION);
                fprintf(stdout, "  \033[1m--h\033[0m, \033[1m--help\033[0m - display a help message\n");
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

int translate(char* engine, char* dest, char* src, char* sl, char* tl) {
    if(strcmp(engine, "google") == 0) {
        translate_engine_google(dest, src, sl, tl);
    } else {
        fprintf(stderr, "[ ERR ] Invalid engine\n");
    }

    return 1;
}

int translate_engine_google(char* dest, char* src, char* sl, char* tl) {
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

    ccode = curl_easy_perform(curl);
    assert(ccode == CURLE_OK);
    curl_easy_cleanup(curl);
    
    return 1;
}

int parse(char* str) {
    // Parsing works pretty simple:
    // 1. Get the start of the translated string (the first appearing " char)
    // 2. Get the end of the translated string (the closing " char)
    // 3. Copy the text in between two pointers to the str string via strncpy)

    char* strs = str;
    char* stre = NULL;

    for(; strs && *strs != '\"'; strs++) ;
    stre = ++strs;
    for(; stre && *stre != '\"'; stre++) ;

    int nslen = stre - strs;
    strncpy(str, strs, nslen);
    str[nslen] = 0;

    return 1;
}
