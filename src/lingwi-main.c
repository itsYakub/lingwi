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

#include <stdio.h>

i32	main(i32 ac, char **av) {
	char	*input;
	char	*output;

	if (ac < 2) {
		fprintf(stderr, "$ lingwi [OPTIONS] [INPUT] ...\n");
		return (1);		
	}
	lingwi_default_settings();
	if (!lingwi_opt(ac, av, &input)) {
		return (1);
	}
	if (lingwi_translate(&input, &output)) {
		fprintf(stdout, "%s\n", output);
	}
	LINGWI_FREE(input);
	LINGWI_FREE(output);
	return (0);
}
