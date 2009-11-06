#include "Thread.h"

void *Thread::thread_function(void *arg)
{
  Thread *thread = (Thread *)arg;

  thread->run();

  return NULL;
}

Thread::Thread() 
  : m_running(false)
{
}

Thread::~Thread()
{
  if (m_running) {
    stop();
  }
}

void Thread::start()
{
  m_running = true;
  pthread_create(&m_id, NULL, thread_function, this);
}

void Thread::stop()
{
  void *exit_status;
  m_running = false;
  pthread_join(m_id, &exit_status);
}
