#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dlfcn.h>
#include "Audio.h"
#include "File.h"
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
    // { "hdmi", "sii9134", 1, false },
  };
  struct config_param param = { "", 1, block_params, 6 };

  debug("initializing audio...\n");

  strcpy(m_file, "");

  Decoder *decoder;
  decoder = new MP3Decoder;
  m_decoders.push_back(decoder);
  m_decoder_map["mp3"] = decoder;

  decoder = new MP4Decoder();
  m_decoders.push_back(decoder);
  m_decoder_map["mp4"] = decoder;
  m_decoder_map["m4a"] = decoder;

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

  m_format.bits = 16;
  m_format.channels = 0;
  m_format.sample_rate = 0;
}

Audio::~Audio()
{
  debug("finishing audio...\n");
  close();
  if (m_audio) m_plugin->finish(m_audio);
  if (m_dl) dlclose(m_dl);

  for (int i=0; i < m_decoders.size(); i++)
    delete m_decoders[i];
}

bool Audio::open(const char *path, const char *artist, const char *album, const char *title, const char *genre, int length)
{
  stop();

  const char *ext = File::extension(path);
  if (!ext) return false;

  if (m_decoder_map.find(ext) == m_decoder_map.end()) {
    debug("couldn't find decoder for file extension: %s\n", ext);
    return false;
  }

  m_decoder = m_decoder_map[ext];

  debug("playing %s using: %s\n", path, m_decoder->name());

  m_decoder->open(path);

  m_length = length;
  safe_strcpy(m_title, title);
  safe_strcpy(m_album, album);
  safe_strcpy(m_artist, artist);
  safe_strcpy(m_genre, genre);

  if (m_decoder->channels() && m_decoder->rate() && 
      (m_format.sample_rate != m_decoder->rate() || m_format.channels != m_decoder->channels())) {
    format_changed();
  }

  safe_strcpy(m_file, path);

  m_state = PLAYING;
  m_elapsed = m_remaining = 0;
  m_seek_percent = -1;
  pthread_create(&m_thread, NULL, play_thread, this);
  return true;
}

void Audio::format_changed()
{
  debug("format changed. channels=%d, rate=%d, bits=%d\n", 
        m_decoder->channels(), m_decoder->rate(), m_decoder->bits());
#ifdef NMT
  if (m_opened) m_plugin->close(m_audio);
#endif
  m_format.sample_rate = m_decoder->rate();
  m_format.channels = m_decoder->channels();
  m_format.bits = m_decoder->bits();
#ifdef NMT
  m_plugin->open(m_audio, &m_format, NULL);
#endif
  m_opened = true;
}

void Audio::stop()
{
  m_seek_percent = -1;
  strcpy(m_file, "");
  m_state = STOPPED;
  m_elapsed = m_remaining = 0;
  if (m_thread) pthread_join(m_thread, 0);
  m_thread = 0;
  if (m_decoder) {
    m_decoder->close();
    m_decoder = 0;
  }
}

void Audio::close()
{
  stop();

  if (m_opened && m_audio) {
    m_plugin->close(m_audio);
    m_opened = false;
  }
}

void *Audio::play_thread(void *arg)
{
  Audio *audio = (Audio *)arg;
  unsigned char *buffer;
  int bytes;
  int length;

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
      
      if ((bytes = audio->m_decoder->read(&buffer)) < 0) break;

      length = audio->m_decoder->length();
      if (!length) length = audio->m_length;
      if (length) {
	audio->m_elapsed = (int)(audio->m_decoder->position() * length);
	audio->m_remaining = length - audio->m_elapsed;
      }
      
      if (audio->m_decoder->channels() && audio->m_decoder->rate() && 
          (audio->m_format.sample_rate != audio->m_decoder->rate() ||
           audio->m_format.channels != audio->m_decoder->channels())) {
        audio->format_changed();
      }

      if (bytes) audio->m_plugin->play(audio->m_audio, buffer, (size_t)bytes, NULL);
    } while (audio->m_state != STOPPED);
  }

  audio->m_thread = 0;
  audio->stop();
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
  if (m_decoder && m_decoder->title()) return m_decoder->title();
  return strrchr(m_file, '/')+1;
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
