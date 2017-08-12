#ifndef XML_COMPILER_SAXHANDLER_H_
#define XML_COMPILER_SAXHANDLER_H_

#include "xml_parser.h"

namespace xml_compiler {

class SAXHandler
{
 public:
  virtual void StartElement(const std::string& name, const XMLAttributeList& attrs) = 0;
  virtual void EndElement(const std::string& name, const XMLAttributeList& attrs) = 0;
  virtual void Text(const std::string& text) = 0;
};

template<typename EventListType> 
constexpr void parseEventList(const EventListType& eventlist, SAXHandler& saxhandler)
{
  for(auto i=0 ; i<eventlist.GetSize() ; ++i ) {
    auto event = eventlist.GetEvent(i);
    switch( event.GetType() ) {
      case EventType::kStartElement:
        saxhandler.StartElement(event.GetElement().GetName().str(), event.GetElement().GetAttributeList());
        break;
      case EventType::kEndElement:
        saxhandler.EndElement(event.GetElement().GetName().str(), event.GetElement().GetAttributeList());
        break;
      case EventType::kText:
        saxhandler.Text(event.GetElement().GetName().str());
        break;
    }
  }
}

constexpr auto ParseXML(const char* xml)
{
  XMLParser parser;
  return parser.parse(xml);
}

}  // xml_compiler

#endif /* XML_COMPILER_SAXHANDLER_H_ */
