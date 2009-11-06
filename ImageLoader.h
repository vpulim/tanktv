#ifndef IMAGELOADER_H
#define IMAGELOADER_H

#include "Thread.h"

class ImageLoader : public Thread
{
 protected:
  virtual void run();

 public:
  ImageLoader();
};

#endif
