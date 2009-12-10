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

#include "File.h"
#include "Decoder.h"

FFMpegDecoder::FFMpegDecoder()
  : Decoder(), 
    m_format(0),
    m_codec_ctx(0),
    m_codec(0),
    m_frame(0),
    m_audio_stream(0),
    m_buffer(0),
    m_file_size(0)
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

  m_position = 0;

  if (av_open_input_file(&m_format, file, 0, 0, 0) != 0) {
    debug("couldn't open media file for playback: %s\n", file);
    m_format = 0;
    return false;
  }

  debug("opened %s (%s)\n", file, m_format->iformat->long_name);  

  m_file_size = File::size(file);

  for (int i=0; i < m_format->nb_streams; i++) {
    if (m_format->streams[i]->codec->codec_type == CODEC_TYPE_AUDIO) {
      m_audio_stream = m_format->streams[i];
      break;
    }
  }
  if (!m_audio_stream) {
    debug("no audio streams found in %s\n", file);
    close();
    return false;
  }

  m_codec_ctx = m_audio_stream->codec;
  m_codec = avcodec_find_decoder(m_codec_ctx->codec_id);
  if (!m_codec) {
    debug("couldn't find a decoder for %s\n", file); 
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
    debug("couldn't allocate frame while opening %s\n", file);
    close();
    return false;
  }

  m_buffer = (unsigned char *)av_malloc(AVCODEC_MAX_AUDIO_FRAME_SIZE);
  if (!m_buffer) {
    debug("couldn't allocate playback buffer while opening %s\n", file);
    m_buffer_size = 0;
    close();
    return false;
  }
  m_buffer_size = AVCODEC_MAX_AUDIO_FRAME_SIZE;

  return true;
}

void FFMpegDecoder::close()
{
  m_audio_stream = 0;
  m_file_size = 0;

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

  if (!m_format) return -1;

  if (av_read_frame(m_format, &packet) < 0)
    return -1;

  if (m_audio_stream->duration > 1) {
    m_position = packet.pts * 1.0 / m_audio_stream->duration;
  }
  else {
    m_position = m_audio_stream->parser->frame_offset * 1.0 / m_file_size;
  }

  debug("pos: %f\n", m_position);

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

void FFMpegDecoder::seek(float percent) 
{
  if (m_audio_stream && m_audio_stream->duration > 1) {
    av_seek_frame(m_format, m_audio_stream->index, (int64_t)(percent * m_audio_stream->duration), 0);
  }
  else {
    av_seek_frame(m_format, m_audio_stream->index, (int64_t)(percent * m_file_size), AVSEEK_FLAG_BYTE);
  }
}

int FFMpegDecoder::length()
{
  if (m_audio_stream && m_audio_stream->duration > 1) {
    return m_audio_stream->time_base.num * m_audio_stream->duration / m_audio_stream->time_base.den;
  }
  return 0;
}

const char *FFMpegDecoder::artist()
{
  if (m_format && m_format->metadata) {
    AVMetadataTag *tag;
    if ((tag=av_metadata_get(m_format->metadata, "TPE1", NULL, AV_METADATA_IGNORE_SUFFIX)) ||
        (tag=av_metadata_get(m_format->metadata, "artist", NULL, AV_METADATA_IGNORE_SUFFIX)) ||
        (tag=av_metadata_get(m_format->metadata, "author", NULL, AV_METADATA_IGNORE_SUFFIX)))
      return tag->value;
  }
  return NULL;
}

const char *FFMpegDecoder::album()
{
  if (m_format && m_format->metadata) {
    AVMetadataTag *tag;
    if ((tag=av_metadata_get(m_format->metadata, "TALB", NULL, AV_METADATA_IGNORE_SUFFIX)) ||
        (tag=av_metadata_get(m_format->metadata, "album", NULL, AV_METADATA_IGNORE_SUFFIX)))
      return tag->value;
  }
  return NULL;
}

const char *FFMpegDecoder::title()
{
  if (m_format && m_format->metadata) {
    AVMetadataTag *tag;
    if ((tag=av_metadata_get(m_format->metadata, "TIT2", NULL, AV_METADATA_IGNORE_SUFFIX)) ||
        (tag=av_metadata_get(m_format->metadata, "title", NULL, AV_METADATA_IGNORE_SUFFIX)))
      return tag->value;
  }
  return NULL;
}

const char *FFMpegDecoder::genre()
{
  if (m_format && m_format->metadata) {
    AVMetadataTag *tag;
    if ((tag=av_metadata_get(m_format->metadata, "TCON", NULL, AV_METADATA_IGNORE_SUFFIX)) ||
        (tag=av_metadata_get(m_format->metadata, "genre", NULL, AV_METADATA_IGNORE_SUFFIX)))
      return tag->value;
  }
  return NULL;
}
