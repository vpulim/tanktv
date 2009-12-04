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
  char m_ext[10];
  bool m_isdir;

 private:
  static const char *extension(const char *name, char *dst);
  static bool isAudio(const char *ext);
  static bool isVideo(const char *ext);

 public:
  static void listDirectory(const char *dir, std::vector<File> &files, bool mediaOnly=true);
  static int size(const char *file);
  const char *name() const { return m_name; }
  const char *path() const { return m_path; }
  const char *extension();
  static const char *extension(const char *name);
  bool isAudio();
  bool isVideo();
  bool isDirectory() { return m_isdir; }
};

bool operator< (const File& f, const File& g);

#endif
