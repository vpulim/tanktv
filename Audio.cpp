#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dlfcn.h>
#include "Audio.h"
#include "Utils.h"

Audio::Audio()
  : m_audio(NULL),
    m_plugin(NULL),
    m_dl(NULL),
    m_opened(false),
    m_state(STOPPED),
    m_thread(0),
    m_decoder(0),
    m_elapsed(0),
    m_remaining(0),
    m_seek_percent(-1)
{
  char *error;

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

  m_decoder = new MP3Decoder();
  if (!m_decoder->valid()) {
    delete m_decoder;
    m_decoder = 0;
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
  if (m_decoder) delete m_decoder;
}

bool Audio::open(const char *path, const char *artist, const char *album, const char *title, const char *genre, int length)
{
  if (!m_decoder) return false;

  stop();

  debug("playing mp3: %s\n", path);

  m_decoder->open(path);

  m_length = length;
  safe_strcpy(m_title, title);
  safe_strcpy(m_album, album);
  safe_strcpy(m_artist, artist);
  safe_strcpy(m_genre, genre);

#ifdef NMT
  if (!m_opened) {
    m_format.sample_rate = m_decoder->rate();
    m_format.channels = m_decoder->channels();
    m_format.bits = m_decoder->bits();
    m_plugin->open(m_audio, &m_format, NULL);
    m_opened = true;
  }
  else if (m_format.sample_rate != m_decoder->rate() || m_format.channels != m_decoder->channels()) {
    m_plugin->close(m_audio);
    m_format.sample_rate = m_decoder->rate();
    m_format.channels = m_decoder->channels();
    m_format.bits = m_decoder->bits();
    m_plugin->open(m_audio, &m_format, NULL);
  }
#endif

  safe_strcpy(m_file, path);

  m_state = PLAYING;
  m_elapsed = m_remaining = 0;
  m_seek_percent = -1;
  pthread_create(&m_thread, NULL, play_thread, this);
  return true;
}

void Audio::stop()
{
  if (m_state != STOPPED) {
    m_state = STOPPED;
    m_elapsed = m_remaining = 0;
    if (m_thread) pthread_join(m_thread, 0);
    m_thread = 0;
    m_seek_percent = -1;
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

  if (m_decoder) m_decoder->close();
}

void *Audio::play_thread(void *arg)
{
  Audio *audio = (Audio *)arg;
  unsigned char *buffer;
  size_t bytes;

  if (audio->m_decoder) {
    do {
      if (audio->m_state == PAUSED) {
	usleep(100);
	continue;
      }
      
      if (audio->m_seek_percent >= 0) {
	audio->m_decoder->seek(audio->m_seek_percent);
	audio->m_seek_percent = -1;
      }
      
      if (!audio->m_decoder->read(&buffer, &bytes)) break;
      
      if (audio->m_length) {
	audio->m_elapsed = (int)(audio->m_decoder->position() * audio->m_length);
	audio->m_remaining = audio->m_length - audio->m_elapsed;
      }
      audio->m_plugin->play(audio->m_audio, buffer, bytes, NULL);
    } while (audio->m_state != STOPPED);
  }

  audio->m_thread = 0;
  audio->close();
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
  if (isStopped() || m_elapsed == 0 || !m_decoder) return;

  m_seek_percent = m_decoder->position() - 0.02;
  if (m_seek_percent < 0) m_seek_percent = 0;
}

void Audio::forward() {
  if (isStopped() || m_remaining == 0 || !m_decoder) return;

  m_seek_percent = m_decoder->position() + 0.02;
  if (m_seek_percent > 1) m_seek_percent = 1;
}

const char *Audio::title()
{
  if (m_title && m_title[0]) return m_title;
  if (m_decoder) return m_decoder->title();
  return NULL;
}

const char *Audio::album()
{
  if (m_album && m_album[0]) return m_album;
  if (m_decoder) return m_decoder->album();
  return NULL;
}

const char *Audio::artist()
{
  if (m_artist && m_artist[0]) return m_artist;
  if (m_decoder) return m_decoder->artist();
  return NULL;
}

const char *Audio::genre()
{
  if (m_genre && m_genre[0]) return m_genre;
  if (m_decoder) return m_decoder->genre();
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
