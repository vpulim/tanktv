#ifndef AUDIO_H
#define AUDIO_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <pthread.h>
#include <mpg123.h>
#include "config.h"

#define GError void

struct block_param {
  char *name;
  char *value;
  int line;
  bool used;
};

struct config_param {
  char *value;
  unsigned int line;
  
  struct block_param *block_params;
  unsigned num_block_params;
};

struct tag;

struct audio_format {
  uint32_t sample_rate;
  uint8_t bits;
  uint8_t channels;
  uint8_t reverse_endian;
};

struct audio_output_plugin {
  const char *name;
  bool (*test_default_device)(void);
  void *(*init)(const struct audio_format *audio_format, const struct config_param *param, GError **error);
  void (*finish)(void *data);
  bool (*open)(void *data, struct audio_format *audio_format, GError **error);
  void (*close)(void *data);
  void (*send_tag)(void *data, const struct tag *tag);
  size_t (*play)(void *data, const void *chunk, size_t size,
		 GError **error);
  void (*cancel)(void *data);
  bool (*pause)(void *data);
  const struct mixer_plugin *mixer_plugin;
};

class Audio
{
 private:
  enum state_t { STOPPED, PLAYING, PAUSED };
  void *m_audio;
  bool m_opened;
  struct audio_output_plugin *m_plugin;
  void *m_dl;
  char m_file[1024];
  struct audio_format m_format;  
  volatile enum state_t m_state;
  volatile int m_elapsed;
  volatile int m_remaining;
  volatile int m_seekto;
  pthread_t m_thread;
  mpg123_handle *m_mpg123;
  mpg123_id3v1 *m_v1;
  mpg123_id3v2 *m_v2;
  char **m_icy;

 private:
  static void *play_thread(void *arg);
  void stop();

 public:
  Audio();
  ~Audio();
  bool open(const char *file);
  void close();
  void play();
  void pause();
  void rewind();
  void forward();
  const char *nowPlaying() { return m_file; };
  bool isPlaying() { return m_state == PLAYING; };
  bool isPaused() { return m_state == PAUSED; };
  bool isStopped() { return m_state == STOPPED; };
  int elapsed() { return m_elapsed; }
  int remaining() { return m_remaining; }
  const char *title();
  const char *album();
  const char *artist();
  const char *genre();
};

#endif
