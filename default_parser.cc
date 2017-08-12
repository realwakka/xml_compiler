#Include "default_parser.h"

#include "xml_parser.h"
#include "saxhandler.h"

namespace xml_compiler {

constexpr char default_document_xml[] = R"(<w:document mc:Ignorable="w14 w15 wp14"><w:body><w:p w:rsidR="00501590" w:rsidRDefault="00501590"><w:bookmarkStart w:id="0" w:name="_GoBack"/><w:bookmarkEnd w:id="0"/></w:p><w:sectPr w:rsidR="00501590"><w:pgSz w:w="11906" w:h="16838"/><w:pgMar w:top="1701" w:right="1440" w:bottom="1440" w:left="1440" w:header="851" w:footer="992" w:gutter="0"/><w:cols w:space="425"/><w:docGrid w:linePitch="360"/></w:sectPr></w:body></w:document>)";

// constexpr char default_document_xml[] = R"(<w:document mc:Ignorable="w14 w15 wp14"></w:document>)";


extern "C" void parseDefaultData(SAXHandler& saxhandler)
{
  constexpr auto eventlist = ParseXML(default_document_xml);
  parseEventList(eventlist, saxhandler);
}

}  // xml_compiler
