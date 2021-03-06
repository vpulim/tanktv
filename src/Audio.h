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

#ifndef AUDIO_H
#define AUDIO_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <pthread.h>
#include <map>
#include <vector>
#include "config.h"
#include "Decoder.h"
#include "Types.h"

#define PLUGIN "/share/Apps/TankTV/lib/smp86xx_plugin.so"
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

typedef struct {
  unsigned char *data;
  size_t size;
} audio_buffer;

typedef std::map<const char *, Decoder *, cmp_strcase> decoder_map;
typedef std::map<const char *, audio_buffer *, cmp_str> sound_map;

class Audio
{
 private:
  enum state_t { STOPPED, PLAYING, PAUSED };
  void *m_audio;
  std::vector<Decoder *> m_decoders;
  decoder_map m_decoder_map;
  sound_map m_sound_cache;
  bool m_opened;
  struct audio_output_plugin *m_plugin;
  void *m_dl;
  char m_file[1024];
  struct audio_format m_format;  
  volatile enum state_t m_state;
  volatile int m_elapsed;
  volatile int m_remaining;
  volatile float m_seek_percent;
  pthread_t m_thread;
  Decoder *m_decoder;
  char m_title[256];
  char m_album[256];
  char m_artist[256];
  char m_genre[256];
  int m_length;

 private:
  void format_changed();
  void format_changed(uint32_t rate, uint8_t channels, uint8_t bits);
  static void *play_thread(void *arg);
  void stop();

 public:
  Audio();
  ~Audio();
  void playSound(const char *path);
  bool open(const char *path, const char *artist, const char *album, const char *title, const char *genre, int length=0);
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
