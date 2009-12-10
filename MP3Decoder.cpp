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
  along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "Decoder.h"

MP3Decoder::MP3Decoder()
  : Decoder(), 
    m_mpg123(0),
    m_total_frames(0)
{
  int err;

  if ((err=mpg123_init()) != MPG123_OK || (m_mpg123=mpg123_new(NULL, &err)) == NULL) {
    debug("couldn't initialize mpg123: %s\n", mpg123_plain_strerror(err));
    mpg123_exit();
    m_mpg123 = 0;
    return;    
  }  
}

MP3Decoder::~MP3Decoder()
{
  if (m_mpg123) {
    mpg123_delete(m_mpg123);
    mpg123_exit();
  }
}

bool MP3Decoder::valid()
{
  return (m_mpg123 != 0);
}

bool MP3Decoder::open(const char *file)
{
  int encoding;
  int meta;

  if (mpg123_open(m_mpg123, (char *)file) != MPG123_OK) {
    debug("couldn't open mp3 file: %s\n", mpg123_strerror(m_mpg123));
    return false;
  }
  
  if (mpg123_getformat(m_mpg123, &m_rate, &m_channels, &encoding) != MPG123_OK) {
    debug("couldn't get mp3 format: %s\n", mpg123_strerror(m_mpg123));
    return false;
  }
  
  if (encoding != MPG123_ENC_SIGNED_16) {
    debug("only signed 16-bit encoding supported!\n");
    return false;
  }

  m_buffer_size = mpg123_outblock(m_mpg123);
  m_buffer = (unsigned char *)malloc(m_buffer_size);
  if (!m_buffer) {
    debug("couldn't allocate audio buffer\n");
    m_buffer_size = 0;
    return false;
  }

  m_bits = 16;
  
  mpg123_format_none(m_mpg123);
  mpg123_format(m_mpg123, m_rate, m_channels, encoding);

  m_v1 = 0;
  m_v2 = 0;
  if ((meta=mpg123_meta_check(m_mpg123))) {
    if (meta & MPG123_ID3) {
      mpg123_id3(m_mpg123, &m_v1, &m_v2);
    }
  }

  mpg123_position(m_mpg123, 0, 0, NULL, &m_total_frames, NULL, NULL);  
  m_position = 0;
}

void MP3Decoder::close()
{
  if (m_mpg123) {
    mpg123_close(m_mpg123);
  }

  if (m_buffer) {
    free(m_buffer);
    m_buffer = 0;
    m_buffer_size = 0;
  }
}

int MP3Decoder::read(unsigned char **buffer)
{
  off_t frames, frames_left;
  size_t bytes_read;
  
  *buffer = m_buffer;

  if (mpg123_read(m_mpg123, m_buffer, m_buffer_size, &bytes_read) != MPG123_OK)
    return -1;

  mpg123_position(m_mpg123, 0, bytes_read, &frames, &frames_left, NULL, NULL);  
  m_total_frames = frames + frames_left;
  m_position = frames * 1.0 / m_total_frames;

  return (int)bytes_read;
}

void MP3Decoder::seek(float percent) 
{
  if (m_mpg123 && percent >= 0 && percent <= 1) {
    mpg123_seek_frame(m_mpg123, (int)(percent * m_total_frames + 0.5), SEEK_SET);
  }  
}

const char *MP3Decoder::title()
{
  if (m_v2) return m_v2->title->p;
  if (m_v1) {
    m_v1->title[sizeof(m_v1->title)-1] = 0;
    return m_v1->title;
  }
  return NULL;
}

const char *MP3Decoder::album()
{
  if (m_v2) return m_v2->album->p;
  if (m_v1) {
    m_v1->album[sizeof(m_v1->album)-1] = 0;
    return m_v1->album;
  }
  return NULL;
}

const char *MP3Decoder::artist()
{
  if (m_v2) return m_v2->artist->p;
  if (m_v1) {
    m_v1->artist[sizeof(m_v1->artist)-1] = 0;
    return m_v1->artist;
  }
  return NULL;
}

const char *MP3Decoder::genre()
{
  if (m_v2) return m_v2->genre->p;
  if (m_v1) return NULL;
  return NULL;
}
