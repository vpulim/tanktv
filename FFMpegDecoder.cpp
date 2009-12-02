#include "Decoder.h"

FFMpegDecoder::FFMpegDecoder()
  : Decoder(), 
    m_format(0),
    m_codec_ctx(0),
    m_codec(0),
    m_frame(0),
    m_buffer(0)
{
  av_register_all();
}

FFMpegDecoder::~FFMpegDecoder()
{
  close();
}

bool FFMpegDecoder::valid()
{
  return true;
}

bool FFMpegDecoder::open(const char *file)
{
  close();

  if (av_open_input_file(&m_format, file, 0, 0, 0) != 0) {
    debug("couldn't open media file for playback: %s\n", file);
    m_format = 0;
    return false;
  }

  debug("opened %s (%s)\n", file, m_format->iformat->long_name);  

  m_codec_ctx = m_format->streams[0]->codec;
  m_codec = avcodec_find_decoder(m_codec_ctx->codec_id);
  if (!m_codec) {
    debug("couldn't find a decoder!\n"); 
    close();
    return false;
  }

  if (avcodec_open(m_codec_ctx, m_codec) < 0) {
    debug("couldn't open codec: %s\n", m_codec->name);
    close();
    return false;
  }

  if (m_codec_ctx->sample_rate) m_rate = m_codec_ctx->sample_rate;
  if (m_codec_ctx->channels) m_channels = m_codec_ctx->channels;

  m_frame = avcodec_alloc_frame();
  if (!m_frame) {
    debug("couldn't allocate frame!\n");
    close();
    return false;
  }

  m_buffer = (unsigned char *)av_malloc(AVCODEC_MAX_AUDIO_FRAME_SIZE);
  if (!m_buffer) {
    debug("couldn't allocate playback buffer!\n");
    m_buffer_size = 0;
    close();
    return false;
  }
  m_buffer_size = AVCODEC_MAX_AUDIO_FRAME_SIZE;

  debug("finished opening\n");

  return true;
}

void FFMpegDecoder::close()
{
  if (m_buffer) {
    av_free(m_buffer);
    m_buffer = 0 ;
  }
  
  if (m_frame) {
    av_free(m_frame);
    m_frame = 0;
  }

  if (m_codec_ctx) {
    avcodec_close(m_codec_ctx);
    m_codec_ctx = 0;
  }

  if (m_format) {
    av_close_input_file(m_format);
    m_format = 0;
  }
}

int FFMpegDecoder::read(unsigned char **buffer)
{
  AVPacket packet;
  int bytes_read;
  *buffer = m_buffer;

  if (av_read_frame(m_format, &packet) < 0)
    return -1;

  if (packet.stream_index == 0) {
    bytes_read = m_buffer_size;
    if (avcodec_decode_audio3(m_codec_ctx, (int16_t *)m_buffer, &bytes_read, &packet) < 0) {
      debug("error decoding frame\n");
      return 0;
    }
  }
  else
    bytes_read = 0;

  av_free_packet(&packet);
    
  if (m_codec_ctx->sample_rate) m_rate = m_codec_ctx->sample_rate;
  if (m_codec_ctx->channels) m_channels = m_codec_ctx->channels;
  return bytes_read;
}
