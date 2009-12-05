#ifndef CURL_H
#define CURL_H

#include <string>
#include <curl/curl.h>
#include "config.h"

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
