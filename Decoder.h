#ifndef DECODER_H
#define DECODER_H

#include <mpg123.h>
#include "config.h"

class Decoder
{
 protected:
  long m_rate;
  int m_channels;
  int m_bits;
  size_t m_buffer_size;
  unsigned char *m_buffer;
  float m_seekto;
  float m_position;
  
 public:
 Decoder()
   : m_rate(0), m_channels(0), m_bits(0), m_seekto(-1), m_position(0) {}
  virtual ~Decoder() {}
  virtual bool valid() { return false; }
  virtual bool open(const char *file) = 0;
  virtual void close() = 0;
  virtual bool read(unsigned char **buffer, size_t *bytes_read) = 0;
  virtual void seek(float percent) {}
  float position() { return m_position; }
  long rate() { return m_rate; }
  int channels() { return m_channels; }
  int bits() { return m_bits; }
  virtual const char *title() { return NULL; }
  virtual const char *album() { return NULL; }
  virtual const char *artist() { return NULL; }
  virtual const char *genre() { return NULL; }
};

class MP3Decoder : public Decoder
{
 private:
  mpg123_handle *m_mpg123;
  mpg123_id3v1 *m_v1;
  mpg123_id3v2 *m_v2;
  off_t m_total_frames;

 public:
  MP3Decoder();
  virtual ~MP3Decoder();
  virtual bool valid();
  virtual bool open(const char *file);
  virtual void close();
  virtual bool read(unsigned char **buffer, size_t *bytes_read);
  virtual void seek(float percent);
  virtual const char *title();
  virtual const char *album();
  virtual const char *artist();
  virtual const char *genre();  
};

#endif
