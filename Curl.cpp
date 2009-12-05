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

