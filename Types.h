#ifndef TYPES_H
#define TYPES_H

#include <string.h>

struct cmp_str {
  bool operator()(const char *a,const  char *b) {
    return strcmp(a, b) < 0;
  }
};

struct cmp_strcase {
  bool operator()(const char *a,const  char *b) {
    return strcasecmp(a, b) < 0;
  }
};

#endif
