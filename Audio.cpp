#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dlfcn.h>
#include "Audio.h"

#define PLUGIN "/share/Apps/TankTV/lib/smp86xx_plugin.so"
#define GError void

Audio::Audio()
  : m_audio(NULL),
    m_plugin(NULL),
    m_dl(NULL),
    m_opened(false),
    m_state(STOPPED),
    m_thread(0),
    m_mpg123(0),
    m_elapsed(0),
    m_remaining(0),
    m_seekto(-1)
{
  char *error;
  int errno;

  struct block_param block_params[] = {
    { "engine", "0", 1, false },
    { "decoder", "1", 1, false },
    { "ramdac", "1", 1, false },
    { "line_count", "2048", 1, false },
    { "fifo_us", "500000", 1, false },
    { "mixer_type", "software", 1, false }
    //    { "hdmi", "sii9134", 1, false },
  };
  struct config_param param = { "", 1, block_params, 6 };

  debug("initializing audio...\n");

  strcpy(m_file, "");

  if ((errno=mpg123_init()) != MPG123_OK || (m_mpg123=mpg123_new(NULL, &errno)) == NULL) {
    debug("couldn't initialize mpg123: %s\n", mpg123_plain_strerror(errno));
    mpg123_exit();
    return;    
  }

#ifdef NMT
  m_dl = dlopen(PLUGIN, RTLD_LAZY|RTLD_GLOBAL);
  // m_dl = dlopen(PLUGIN, RTLD_LAZY);
  if (!m_dl) {
    debug("couldn't load %s: %s\n", PLUGIN, dlerror());
    return;
  }

  m_plugin = (struct audio_output_plugin *)dlsym(m_dl, "smp86xx");
  if ((error= dlerror())) {
    debug("couldn't initialize plugin: %s\n", error);
    return;
  }

  debug("initializing plugin...\n");
  m_audio = m_plugin->init(NULL, &param, NULL);
#endif
}

Audio::~Audio()
{
  debug("finishing audio...\n");
  close();
  if (m_audio) m_plugin->finish(m_audio);
  if (m_dl) dlclose(m_dl);
  if (m_mpg123) {
    mpg123_delete(m_mpg123);
    mpg123_exit();
  }
}

bool Audio::open(const char *file)
{
  long rate;
  int channels, encoding;
  int meta;
  char title[31];

  stop();

  debug("playing mp3: %s\n", file);

  if (mpg123_open(m_mpg123, (char *)file) != MPG123_OK) {
    debug("couldn't open mp3 file: %s\n", mpg123_strerror(m_mpg123));
    return false;
  }

  if (mpg123_getformat(m_mpg123, &rate, &channels, &encoding) != MPG123_OK) {
    debug("couldn't get mp3 format: %s\n", mpg123_strerror(m_mpg123));
    return false;
  }

  if (encoding != MPG123_ENC_SIGNED_16) {
    debug("only signed 16-bit encoding supported!\n");
    return false;
  }

  m_v1 = 0;
  m_v2 = 0;
  if ((meta=mpg123_meta_check(m_mpg123))) {
    if (meta & MPG123_ID3) {
      mpg123_id3(m_mpg123, &m_v1, &m_v2);
    }
  }

  mpg123_format_none(m_mpg123);
  mpg123_format(m_mpg123, rate, channels, encoding);

#ifdef NMT
  if (!m_opened) {
    m_format.sample_rate = rate;
    m_format.channels = channels;
    m_format.bits = 16;
    m_plugin->open(m_audio, &m_format, NULL);
    m_opened = true;
  }
  else if (m_format.sample_rate != rate || m_format.channels != channels) {
    m_plugin->close(m_audio);
    m_format.sample_rate = rate;
    m_format.channels = channels;
    m_format.bits = 16;
    m_plugin->open(m_audio, &m_format, NULL);
  }
#endif

  strncpy(m_file, file, sizeof(m_file)-1);

  m_state = PLAYING;
  m_elapsed = m_remaining = 0;
  m_seekto = -1;
  pthread_create(&m_thread, NULL, play_thread, this);
  return true;
}

void Audio::stop()
{
  if (m_state != STOPPED) {
    m_state = STOPPED;
    if (m_thread) pthread_join(m_thread, 0);
    m_thread = 0;
    m_elapsed = m_remaining = 0;
    m_seekto = -1;
    strcpy(m_file, "");
  }
}

void Audio::close()
{
  if (m_state != STOPPED) {
    stop();
    if (m_opened && m_audio) {
      m_plugin->close(m_audio);
      m_opened = false;
    }
  }

  if (m_mpg123) {
    mpg123_close(m_mpg123);
  }
}

void *Audio::play_thread(void *arg)
{
  Audio *audio = (Audio *)arg;
  mpg123_handle *mh = audio->m_mpg123;  
  int errno;
  unsigned char *buffer = NULL;
  size_t buffer_size, bytes;
  off_t frames, frames_left;
  double secs, secs_left;

  buffer_size = mpg123_outblock(mh);
  buffer = (unsigned char *)malloc(buffer_size);

  do {
    if (audio->m_state == PAUSED) {
      usleep(100);
      continue;
    }

    if (audio->m_seekto >= 0) {
      frames = mpg123_timeframe(mh, audio->m_seekto);
      if (frames >= 0) {
	mpg123_seek_frame(mh, frames, SEEK_SET);
      }
      audio->m_seekto = -1;
    }

    errno = mpg123_read(mh, buffer, buffer_size, &bytes);
    mpg123_position(mh, 0, bytes, &frames, &frames_left, &secs, &secs_left);
    audio->m_elapsed = (int)(secs+0.5);
    audio->m_remaining = (int)(secs_left+0.5);

#ifdef NMT
    audio->m_plugin->play(audio->m_audio, buffer, buffer_size, NULL);
#endif

  } while (errno == MPG123_OK && audio->m_state != STOPPED);

  free(buffer);
}

void Audio::play()
{
  m_state = PLAYING;
}

void Audio::pause()
{
  m_state = PAUSED;
}

void Audio::rewind() {
  if (isStopped() || m_elapsed == 0) return;

  m_seekto = m_elapsed - 5;
  if (m_seekto < 0) m_seekto = 0;
}

void Audio::forward() {
  if (isStopped() || m_remaining == 0) return;

  m_seekto = m_elapsed + 5;
  if (m_remaining < 5) m_seekto = m_elapsed + m_remaining - 1;
}

const char *Audio::title()
{
  if (m_v2) return m_v2->title->p;
  if (m_v1) {
    m_v1->title[sizeof(m_v1->title)-1] = 0;
    return m_v1->title;
  }
  return NULL;
}

const char *Audio::album()
{
  if (m_v2) return m_v2->album->p;
  if (m_v1) {
    m_v1->album[sizeof(m_v1->album)-1] = 0;
    return m_v1->album;
  }
  return NULL;
}

const char *Audio::artist()
{
  if (m_v2) return m_v2->artist->p;
  if (m_v1) {
    m_v1->artist[sizeof(m_v1->artist)-1] = 0;
    return m_v1->artist;
  }
  return NULL;
}

const char *Audio::genre()
{
  if (m_v2) return m_v2->genre->p;
  if (m_v1) return NULL;
  return NULL;
}

extern "C" struct block_param *config_get_block_param(const struct config_param * param, const char *name)
{
  if (param == NULL)
    return NULL;

  for (unsigned i = 0; i < param->num_block_params; i++) {
    if (0 == strcmp(name, param->block_params[i].name)) {
      struct block_param *bp = &param->block_params[i];
      bp->used = true;
      return bp;
    }
  }

  return NULL;
}


extern "C" const char * config_get_block_string(const struct config_param *param, const char *name, const char *default_value)
{
  struct block_param *bp = config_get_block_param(param, name);

  if (bp == NULL)
    return default_value;
  
  return bp->value;
}

extern "C" unsigned config_get_block_unsigned(const struct config_param *param, const char *name, unsigned default_value)
{
  struct block_param *bp = config_get_block_param(param, name);
  long value;
  char *endptr;
  
  if (bp == NULL)
    return default_value;
  
  value = strtol(bp->value, &endptr, 0);
  
  return (unsigned)value;
}
