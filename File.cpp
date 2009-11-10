#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <sys/stat.h>
#include <dirent.h>
#include "File.h"
#include "Utils.h"

using namespace std;

File::File(const char *name, const char *path, bool isDirectory)
  : m_isdir(isDirectory)
{
  strncpy(m_name, name, sizeof(m_name)-1);
  strncpy(m_path, path, sizeof(m_path)-1);
}

bool operator< (const File& f, const File& g)
{
  return strcasecmp(f.name(), g.name()) < 0;
}

void File::listDirectory(const char *dir, vector<File> &files)
{
  if (!dir || !dir[0]) return;
  
  DIR *dp;
  struct dirent *dirp;
  char path[1024];
  int len = strlen(dir);
  struct stat st;

  strncpy(path, dir, sizeof(path)-2);
  if (dir[len-1] != '/') {
    strcat(path, "/");
    len++;
  }

  if((dp  = opendir(dir)) == NULL) {
    perror("listDirectory");
    return;
  }

  while ((dirp = readdir(dp)) != NULL) {
    if (dirp->d_name[0] != '.') {
      strncpy(path+len, dirp->d_name, sizeof(path)-len-1);
      stat(path, &st);
      files.push_back(File(dirp->d_name, path, S_ISDIR(st.st_mode)));
    }
  }
  closedir(dp);
}
