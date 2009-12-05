#include <stdlib.h>
#include <errno.h>
#include "Xml.h"

Xml::Xml(TiXmlNode *node)
  : m_node(node),
    m_doc(0)
{
}

Xml::Xml(const char *xml)
  : m_node(0),
    m_doc(0)
{
  m_doc = new TiXmlDocument();
  m_doc->Parse(xml);
  m_node = m_doc->RootElement();
}

Xml::~Xml()
{
  free_xpath_nodes();
  if (m_doc) delete m_doc;
}

void Xml::free_xpath_nodes()
{
  for (int i=0; i < m_xpath_nodes.size(); i++)
    delete m_xpath_nodes[i];
  m_xpath_nodes.clear();
}

const char *Xml::text()
{
  if (m_node && m_node->Type() == TiXmlNode::ELEMENT) {
    return ((TiXmlElement *)m_node)->GetText();
  }
  else if (m_node) {
    return m_node->Value();
  }
  return NULL;
}

XmlNodeList &Xml::find(const char *query)
{
  free_xpath_nodes();

  if (m_node) {
    TinyXPath::xpath_processor xpath(m_node, query);
    int num = xpath.u_compute_xpath_node_set();
    for (int i=0; i<num; i++) {
      m_xpath_nodes.push_back(new Xml(xpath.XNp_get_xpath_node(i)));
    }
  }

  return m_xpath_nodes;
}

Xml *Xml::find_one(const char *xpath)
{
  XmlNodeList nodes = find(xpath);
  if (nodes.size() > 0)
    return nodes[0];
  else
    return NULL;
}

const char *Xml::find_text(const char *xpath, const char *default_text)
{
  Xml *xml = find_one(xpath);
  return xml ? xml->text() : default_text;
}

long Xml::find_number(const char *xpath, long default_number)
{
  Xml *xml = find_one(xpath);
  if (xml) {
    errno = 0;
    long val = strtol(xml->text(), NULL, 10);
    return errno ? default_number : val;
  }
  return default_number;
}
