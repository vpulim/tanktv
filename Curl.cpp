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

#include "Curl.h"

void Curl::init()
{
  curl_global_init(CURL_GLOBAL_ALL);
}

void Curl::cleanup()
{
  curl_global_cleanup();
}

Curl::Curl()
{
  m_curl = curl_easy_init();
  curl_easy_setopt(m_curl, CURLOPT_ERRORBUFFER, m_error_buffer);
  curl_easy_setopt(m_curl, CURLOPT_FOLLOWLOCATION, 1);
}

Curl::~Curl()
{
  curl_easy_cleanup(m_curl);
}

bool Curl::get(const char *url, const char *path)
{
  FILE *file = fopen(path, "w");
  if (!file) return false;

  curl_easy_setopt(m_curl, CURLOPT_URL, url);
  curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, 0);
  curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, file);
  if (curl_easy_perform(m_curl)) {
    fclose(file);
    return false;
  }

  fclose(file);
  return true;
}

const char *Curl::get(const char *url) 
{
  m_response = "";
  curl_easy_setopt(m_curl, CURLOPT_URL, url);
  curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, write);
  curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, this);
  if (curl_easy_perform(m_curl)) {
    return NULL;
  }
  return m_response.c_str();
}

size_t Curl::write(void *buffer, size_t size, size_t nmemb, void *userp)
{
  Curl *curl = (Curl *)userp;
  curl->m_response += (char *)buffer;
  return size*nmemb;
}

const char *Curl::escape(const char *text)
{
  char *escaped = curl_easy_escape(m_curl, text, 0);
  m_escaped = escaped;
  curl_free(escaped);
  return m_escaped.c_str();
}

