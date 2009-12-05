#ifndef XML_H
#define XML_H

#include <vector>
#include <xpath_processor.h>

typedef std::vector<class Xml *> XmlNodeList;

class Xml
{
 private:
  TiXmlNode *m_node;
  TiXmlDocument *m_doc;
  XmlNodeList m_xpath_nodes;

 private:
  Xml(TiXmlNode *node);
  void free_xpath_nodes();
  
 public:
  Xml(const char *xml);
  ~Xml();
  const char *text();
  XmlNodeList &find(const char *xpath);
  Xml *find_one(const char *xpath);
  const char *find_text(const char *xpath, const char *default_text="");
  long find_number(const char *xpath, long default_number=0);
};

#endif
