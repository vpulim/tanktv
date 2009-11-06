#ifndef THREAD_H
#define THREAD_H

#include <pthread.h>

class Thread
{
 protected:
  bool m_running;
  pthread_t m_id;

 private:
  static void *thread_function(void *arg);

 protected:
  virtual void run() = 0;

 public:
  Thread();
  ~Thread();
  virtual void start();
  virtual void stop();
};

#endif
