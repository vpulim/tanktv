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

#ifndef CURL_H
#define CURL_H

#include <string>
#include <curl/curl.h>

class Curl
{
 private:
  std::string m_response;
  std::string m_escaped;
  CURL *m_curl;
  char m_error_buffer[CURL_ERROR_SIZE+1];

 private:
  static size_t write(void *buffer, size_t size, size_t nmemb, void *userp);
  
 public:
  Curl();
  ~Curl();
  static void init();
  static void cleanup();
  const char *get(const char *url);
  bool get(const char *url, const char *path);
  const char *error() { return m_error_buffer; }
  const char *escape(const char *text);
};

#endif
