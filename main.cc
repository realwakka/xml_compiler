#include <iostream>
#include <cstring>

#include <dlfcn.h>

#include "saxhandler.h"

typedef void ParseFunc(xml_compiler::SAXHandler&);

class MySAXHandler : public xml_compiler::SAXHandler
{
  void StartElement(const std::string& name, const xml_compiler::XMLAttributeList& attrs) override
  {
    std::cout << "StartElement: " << name << std::endl;
    for(auto i=0 ; i<attrs.GetSize() ; ++i ) {
      auto attr = attrs.GetAttribute(i);
      std::cout << "key: " << attr.GetKey().str() << std::endl;
      std::cout << "value: " << attr.GetValue().str() << std::endl;
    }
  }
  void EndElement(const std::string& name, const xml_compiler::XMLAttributeList& attrs) override
  {
    std::cout << "EndElement: " << name << std::endl;
    for(auto i=0 ; i<attrs.GetSize() ; ++i ) {
      auto attr = attrs.GetAttribute(i);
      std::cout << "key: " << attr.GetKey().str() << std::endl;
      std::cout << "value: " << attr.GetValue().str() << std::endl;
    }
  }
  void Text(const std::string& text) override
  {
    std::cout << "Text: " << text;
  }
};


int main()
{
  void* handle = dlopen("./libdefault_parser.so", RTLD_LAZY);
  auto parse_func = (ParseFunc*)dlsym(handle, "parseDefaultData");
  
  MySAXHandler handler;
  
  parse_func(handler);
  dlclose(handle);
  return 0;
}
                       
