
  _   _                              _ 
 | | (_)  _ __     __ _  __      __ (_)
 | | | | | '_ \   / _` | \ \ /\ / / | |
 | | | | | | | | | (_| |  \ V  V /  | |
 |_| |_| |_| |_|  \__, |   \_/\_/   |_|
                  |___/                

-- Introduction ------------------------

lingwi is a command-line utility written in C programming language.
Is is based on cURL library for internet access, using third-party
translation APIs, like: Google Translation API.

Using lingwi is pretty simple:

$ lingwi [OPTIONS] [TEXT-INPUT] ...

Example usage:

$ lingwi -t pl "Hello, world!"
Witaj, świecie!
$ lingwi -t ja "Text in japan!"
テキストは日本で！
$ lingwi -s pl -t es "Witaj, lingwi!"
¡Hola lingú!

etc... etc...

-- Flags and Options -------------------

    -s, --source-language       select a source language (translation from...) (DEFAULT: en)
    -t, --target-language       select a target language (translation to...) (DEFAULT: en)
    -e, --engine                select a translation engine (DEFAULT: google)
    -a, --api-key               select an api key (DEFAULT: none)
    -v, --verbose               enable verbosity
    -V, --version               display a version
    -h, --help                  display a help message

-- Setup and Installation --------------

0. Dependencies:
    0.1 cURL / libcurl:
        - https://github.com/curl/curl
        - https://curl.se/
        - https://curl.se/libcurl/

1. Clone this repository:

$ git clone https://github.com/itsYakub/lingwi.git
$ cd lingwi/

2. There are several ways to build the project:
    2.1. Direct command:

    $ cc -Wall -Wextra -Werror main.c -o lingwi -lcurl

    2.2. Bash files:

    $ ./BUILD.sh
    $ ./INSTALL.sh

    2.3. Makefile:

    $ make all

-- Credits -----------------------------

1. cURL: 
    - https://curl.se/
2. Google API:
    - https://cloud.google.com/apis/?hl=en
    - https://cloud.google.com/translate/docs/reference/api-overview
3. ISO 639:
    - https://www.iso.org/iso-639-language-code
    - https://en.wikipedia.org/wiki/ISO_639
    - https://en.wikipedia.org/wiki/List_of_ISO_639_language_codes

The program was written by @itsYakub.
LLM like ChatGPT was used used to a small extent.

-- LICENCE -----------------------------

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
OR OTHER DEALINGS IN THE SOFTWARE.
