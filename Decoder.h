#ifndef DECODER_H
#define DECODER_H

#include "config.h"
#include <mpg123.h>
/*
extern "C" { 
#include <libavformat/avformat.h> 
}
*/
#define HAVE_STDINT_H
#include <mp4ff.h>
#include <neaacdec.h>

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
  virtual const char *name() = 0;
  virtual bool valid() { return false; }
  virtual bool open(const char *file) = 0;
  virtual void close() = 0;
  virtual int read(unsigned char **buffer) = 0;
  virtual void seek(float percent) {}
  float position() { return m_position; }
  long rate() { return m_rate; }
  int channels() { return m_channels; }
  int bits() { return m_bits; }
  virtual int length() { return 0; }
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
  virtual const char *name() { return "MP3 Decoder"; }
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

/*
class FFMpegDecoder : public Decoder
{
 private:
  AVFormatContext *m_format;
  AVCodecContext *m_codec_ctx;
  AVCodec *m_codec;
  AVStream *m_audio_stream;
  AVFrame *m_frame;
  uint8_t *m_buffer;
  long m_file_size;

 public:
  FFMpegDecoder();
  virtual ~FFMpegDecoder();
  virtual const char *name() { return "FFMpeg Decoder"; }
  virtual bool valid();
  virtual bool open(const char *file);
  virtual void close();
  virtual int read(unsigned char **buffer);
  virtual void seek(float percent);
  virtual int length();
  virtual const char *title();
  virtual const char *album();
  virtual const char *artist();
  virtual const char *genre();  
};
*/

class MP4Decoder : public Decoder
{
 private:
  FILE *m_file;
  NeAACDecHandle m_aac;
  mp4ff_callback_t m_callback;
  mp4ff_t *m_mp4;
  int m_track;
  long m_num_samples;
  long m_sample;

 private:
  static uint32_t mp4_read(void *user_data, void *buffer, uint32_t length);
  static uint32_t mp4_seek(void *user_data, uint64_t position);

 public:
  MP4Decoder();
  virtual ~MP4Decoder();
  virtual const char *name() { return "MP4 Decoder"; }
  virtual bool valid();
  virtual bool open(const char *file);
  virtual void close();
  virtual int read(unsigned char **buffer);
};

#endif
