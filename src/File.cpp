/*
  Copyright (c) 2009 Vinay Pulim

  This file is part of TankTV.

  TankTV is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  
  TankTV is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with TankTV.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <sys/stat.h>
#include <dirent.h>
#include <ctype.h>
#include "File.h"
#include "Utils.h"

File::File(const char *name, const char *path, bool isDirectory)
  : m_isdir(isDirectory)
{
  safe_strcpy(m_name, name);
  safe_strcpy(m_path, path);
}

int File::size(const char *file)
{
  struct stat st;
  stat(file, &st);
  return st.st_size;
}

bool operator< (const File& f, const File& g)
{
  return strcasecmp(f.name(), g.name()) < 0;
}

void File::listDirectory(const char *dir, std::vector<File> &files, bool mediaOnly)
{
  if (!dir || !dir[0]) return;
  
  DIR *dp;
  struct dirent *dirp;
  char path[1024], ext[10];
  int len = strlen(dir);
  struct stat st;

  if (len > sizeof(path) - 2) {
    fprintf(stderr, "directory name too long: %s\n", dir);
    return;
  }

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
      extension(dirp->d_name, ext);
      if (S_ISDIR(st.st_mode) || !mediaOnly || isAudio(ext) || isVideo(ext))
	files.push_back(File(dirp->d_name, path, S_ISDIR(st.st_mode)));
    }
  }
  closedir(dp);
}

const char *File::extension()
{
  return extension(m_name, m_ext);
}

const char *File::extension(const char *name, char *dst)
{
  char *s = (char *)name, *ext = NULL;
  while (*s) if (*s++ == '.') ext = s;
  if (!ext) return NULL;
  s = dst;
  while (*ext) *s++ = tolower(*ext++);
  *s = 0;
  return dst;
}

const char *File::extension(const char *name)
{
  char *ext = strrchr((char *)name, '.');
  if (ext) return (char *)(ext+1);
  return NULL;
}

bool File::isAudio()
{
  return isAudio(extension());
}


bool File::isVideo()
{
  return isVideo(extension());
}

bool File::isAudio(const char *ext)
{
  return !(strcmp(ext, "mp3") &&
	   strcmp(ext, "m4a") &&
//	   strcmp(ext, "m4p") &&
//	   strcmp(ext, "ogg") &&
//	   strcmp(ext, "flac") &&
//	   strcmp(ext, "wma") &&
//	   strcmp(ext, "wav") &&
//	   strcmp(ext, "oga") &&
//	   strcmp(ext, "m4b") &&
//	   strcmp(ext, "aac") &&
//	   strcmp(ext, "3g2") &&
           true);
}

bool File::isVideo(const char *ext)
{
  return !(strcmp(ext, "avi") &&
	   strcmp(ext, "mpg") &&
	   strcmp(ext, "mkv") &&
	   strcmp(ext, "mov") &&
	   strcmp(ext, "mp4") &&
	   strcmp(ext, "ts") &&
	   strcmp(ext, "asf") &&
	   strcmp(ext, "wmv") &&
	   strcmp(ext, "rm") &&
	   strcmp(ext, "m4v") &&
	   strcmp(ext, "m2ts") &&
	   strcmp(ext, "vob") &&
	   strcmp(ext, "mpeg") &&
	   strcmp(ext, "m2v") &&
	   strcmp(ext, "m1v") &&
	   strcmp(ext, "m2p") &&
	   strcmp(ext, "tp") &&
	   strcmp(ext, "mts"));
}
