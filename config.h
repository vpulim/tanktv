#ifndef CONFIG_H
#define CONFIG_H

#define VERSION "0.0.2"

//#define DEBUG
#define NMT

#define APP_NAME "TankTV"

#include <stdio.h>

#ifdef DEBUG
#define debug printf
#define BOLD_FONT "data/bold.ttf"
#define REGULAR_FONT "data/regular.ttf"

#else
#define debug(x,...)
#define BOLD_FONT "data/bold.ttf"
#define REGULAR_FONT "data/regular.ttf"
#endif

#endif

