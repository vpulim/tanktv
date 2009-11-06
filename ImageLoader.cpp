#include <stdio.h>
#include <unistd.h>
#include "ImageLoader.h"

ImageLoader::ImageLoader() 
  : Thread()
{
}

void ImageLoader::run()
{
  while (m_running) {
    sleep(5);
    printf("tick\n");
  }
}
