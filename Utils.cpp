#include "Utils.h"

unsigned hash(const char *s)
{
  unsigned h = 0;
  while (*s) 
    h = h * 101 + (unsigned int)((unsigned char *) *s++);
  return h;
}
