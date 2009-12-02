#ifndef DECODER_H
#define DECODER_H

#include <mpg123.h>
extern "C" { 
#include <libavformat/avformat.h> 
}
#include "config.h"

class Decoder
{
 protected:
  long m_rate;
  int m_channels;
  int m_bits;
  int m_buffer_size;
  unsigned char *m_buffer;
  float m_seekto;
  float m_position;
  
 public:
 Decoder()
   : m_rate(44100), m_channels(2), m_bits(16), m_seekto(-1), m_position(0) {}
  virtual ~Decoder() {}
  virtual bool valid() { return false; }
  virtual bool open(const char *file) = 0;
  virtual void close() = 0;
  virtual int read(unsigned char **buffer) = 0;
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
  virtual int read(unsigned char **buffer);
  virtual void seek(float percent);
  virtual const char *title();
  virtual const char *album();
  virtual const char *artist();
  virtual const char *genre();  
};

class FFMpegDecoder : public Decoder
{
 private:
  AVFormatContext *m_format;
  AVCodecContext *m_codec_ctx;
  AVCodec *m_codec;
  AVFrame *m_frame;
  uint8_t *m_buffer;

 public:
  FFMpegDecoder();
  virtual ~FFMpegDecoder();
  virtual bool valid();
  virtual bool open(const char *file);
  virtual void close();
  virtual int read(unsigned char **buffer);
};

#endif
