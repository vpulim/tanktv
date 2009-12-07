#ifndef CONFIG_H
#define CONFIG_H

#define DEBUG 1
#define NMT 1

#include <stdio.h>

#ifdef DEBUG
#define debug printf
#define BOLD_FONT "data/bold_small.ttf"
#define REGULAR_FONT "data/regular_small.ttf"

#else
#define debug(x,...)
#define BOLD_FONT "data/bold.ttf"
#define REGULAR_FONT "data/regular.ttf"
#endif

#endif

