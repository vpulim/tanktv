#ifndef UTILS_H
#define UTILS_H

#define min(x,y) ((x)<(y)?(x):(y))
#define max(x,y) ((x)>(y)?(x):(y))

#define safe_strcpy(dst, src) {strncpy(dst, (src) ? (src) : "", sizeof(dst)-1); (dst)[sizeof(dst)-1]=0;}

unsigned hash(const char *s);

#endif
