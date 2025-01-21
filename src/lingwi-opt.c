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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

static struct option LINGWI_LONGOPT[] = {
    { "source-language",	required_argument,	NULL,	's' },
    { "target-language",	required_argument,	NULL,	't' },
    { "engine",				required_argument,	NULL,	'e' },
    { "api-key",			required_argument,	NULL,	'a' },
    { "version",			no_argument,		NULL,	'v' },
    { "help",				no_argument,		NULL,	'h' },
    { "list-api",			no_argument,		NULL,	'l' },
    { 0,					0,					0,		0	}
};

static char	*__lingwi_input(i32 ac, char **av);

LINGWI_API i32	lingwi_opt(i32 ac, char **av, char *input) {
	char	opt;

	while ((opt = getopt_long(ac, av, ":s:t:e:a:vhl", LINGWI_LONGOPT, NULL)) != -1) {
		switch (opt) {
			case ('s'): {
				if (!lingwi_set_slang(optarg)) {
					fprintf(stderr, "ERR.: Invalid langcode: %s\n", optarg);
					return (0);
				}
			} break;
			case ('t'): {
				if (!lingwi_set_tlang(optarg)) {
					fprintf(stderr, "ERR.: Invalid langcode: %s\n", optarg);
					return (0);
				}
			} break;
			case ('e'): {
				if (!lingwi_set_engine(optarg)) {
					fprintf(stderr, "ERR.: Invalid engine: %s\n", optarg);
					return (0);
				}
			} break;
			case ('a'): {
				lingwi_set_apikey(optarg);
			} break;
			case ('v'): {
				fprintf(stdout, "%s\n", LINGWI_VERSION);
				exit(EXIT_SUCCESS);
			} break;
			case ('h'): {
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
			} break;
			case ('l'): {
				fprintf(stdout, "\033[1mlingwi translation APIs:\033[0m\n");
				fprintf(stdout, "> Google:\n");
				fprintf(stdout, "	- api-key: optional\n");
				fprintf(stdout, "	- token limit: 5000\n");
				exit(EXIT_SUCCESS);
			} break;
			default: {
				fprintf(stderr, "ERR.: Invalid option: %c\n", opt);
				return (0);
			}
		}
	}
	input = __lingwi_input(ac, av);
	if (!input) {
		fprintf(stderr, "ERR.: No input provided\n");
		return (0);
	}
	return (1);
}

static char	*__lingwi_input(i32 ac, char **av) {
	char	str[1024];
	i32		str_proc;

	str_proc = 0;
	for (size_t i = 0; i < optind; i++) {
		lingwi_strlcpy(str, av[i], 1024);
		if (*str) {
			str_proc = 1;
		}
	}
	return (lingwi_strdup(str));	
}
