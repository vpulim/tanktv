#ifndef CONFIG_H
#define CONFIG_H

#define DEBUG 1
#define NMT 1

#include <stdio.h>

#ifdef DEBUG
#define debug printf
#define BOLD_FONT "fonts/bold_small.ttf"
#define REGULAR_FONT "fonts/regular_small.ttf"

#else
#define debug(x,...)
#define BOLD_FONT "fonts/bold.ttf"
#define REGULAR_FONT "fonts/regular.ttf"
#endif

#endif

