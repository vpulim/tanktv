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

#include "Decoder.h"

MP4Decoder::MP4Decoder()
  : Decoder(),
    m_file(0),
    m_mp4(0),
    m_track(-1),
    m_num_samples(0),
    m_sample(0)
{
  m_aac = NeAACDecOpen();  
  if (m_aac) {
    NeAACDecConfigurationPtr config = NeAACDecGetCurrentConfiguration(m_aac);
    config->outputFormat = FAAD_FMT_16BIT;
    NeAACDecSetConfiguration(m_aac, config);
    
    m_buffer_size = 100000;
    m_buffer = (unsigned char *)malloc(m_buffer_size);
    if (!m_buffer) {
      debug("couldn't allocate audio buffer\n");
      m_buffer_size = 0;
    }
  }

  m_callback.read = mp4_read;
  m_callback.write = 0;
  m_callback.seek = mp4_seek;
  m_callback.truncate = 0;
  m_callback.user_data = 0;
}

MP4Decoder::~MP4Decoder()
{
  NeAACDecClose(m_aac);

  if (m_buffer) {
    free(m_buffer);
    m_buffer = 0;
    m_buffer_size = 0;
  }
}

bool MP4Decoder::valid()
{
  return m_aac != 0;
}

bool MP4Decoder::open(const char *file)
{
  if (!(m_file = fopen(file, "r"))) {
    debug("couldn't open mp4 file: %s\n", file); return false;
  }

  m_callback.user_data = m_file;

  if (!(m_mp4 = mp4ff_open_read(&m_callback))) {
    debug("not an mp4 stream\n"); 
    close();
    return false;
  }

  int num_tracks = mp4ff_total_tracks(m_mp4);
  m_track = -1;
  for (int i=0; i < num_tracks; i++) {
    unsigned char *buff = NULL;
    unsigned int buff_size = 0;
    int err;
    unsigned long rate;
    unsigned char channels;

    if (mp4ff_get_track_type(m_mp4, i) != 1) continue;  
    mp4ff_get_decoder_config(m_mp4, i, &buff, &buff_size);
    if (!buff) continue;
    err = NeAACDecInit2(m_aac, buff, (unsigned long)buff_size, &rate, &channels);
    free(buff);
    if (err >= 0) {
      m_track = i;
      m_rate = rate;
      m_channels = channels;
      break;
    }
  }
  if (m_track < 0) {
    debug("no audio tracks found\n");
    close();
    return false;
  }

  m_num_samples = mp4ff_num_samples(m_mp4, m_track);
  m_sample = 0;
  return true;
}

void MP4Decoder::close()
{
  m_num_samples = m_sample = 0;
  m_track = -1;
  if (m_file) {
    fclose(m_file);
    m_file = 0;
  }

  if (m_mp4) {
    mp4ff_close(m_mp4);
    m_mp4 = 0;
  }
}

int MP4Decoder::read(unsigned char **buffer)
{
  if (!m_mp4 || m_track < 0) return -1;

  if (!mp4ff_read_sample_v2(m_mp4, m_track, m_sample, m_buffer))
    return -1;

  int32_t buffer_size = mp4ff_read_sample_getsize(m_mp4, m_track, m_sample);
  NeAACDecFrameInfo frame;

  m_sample++;

  *buffer = (unsigned char *)NeAACDecDecode(m_aac, &frame, m_buffer, buffer_size);
  if (frame.error) {
    debug("bad decode: %s\n", NeAACDecGetErrorMessage(frame.error));
    return 0;
  }    
  return frame.samples * 2;
}

uint32_t MP4Decoder::mp4_read(void *user_data, void *buffer, uint32_t length)
{
  FILE* f = (FILE*)user_data; 
  if (!f) return 0;
  return length * fread(buffer, length, 1, f); 
}

uint32_t MP4Decoder::mp4_seek(void *user_data, uint64_t position)
{
  FILE* f = (FILE*)user_data; 
  if (!f) return 0;
  return fseek(f, position, SEEK_SET);
}

