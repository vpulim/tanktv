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
