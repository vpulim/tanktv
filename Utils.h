#ifndef UTILS_H
#define UTILS_H

#include <string>

#define min(x,y) ((x)<(y)?(x):(y))
#define max(x,y) ((x)>(y)?(x):(y))

unsigned hash(const char *s);
bool compare_no_case( const std::string& s1, const std::string& s2 );

#endif
