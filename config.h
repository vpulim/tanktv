#ifndef CONFIG_H
#define CONFIG_H

#define DEBUG 1
#define NMT 1

#include <stdio.h>

#ifdef DEBUG
#define debug printf
#else
#define debug(x,...)
#endif

#endif

