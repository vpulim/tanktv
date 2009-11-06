#ifndef FILE_H
#define FILE_H

#include <vector>
#include "config.h"

class File
{
 public:
  File(const char *name, const char *path, bool isDirectory=false);
  char m_name[256];
  char m_path[1024];
  bool m_isdir;

 public:
  static void listDirectory(const char *dir, std::vector<File> &files);
  const char *name() const { return m_name; }
  const char *path() const { return m_path; }
  bool isDirectory() { return m_isdir; }
};

bool operator< (const File& f, const File& g);

#endif
