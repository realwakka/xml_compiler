#ifndef XML_PARSER_H_
#define XML_PARSER_H_

#include <functional>
#include <tuple>
#include <utility>
#include <string>

#include "conststr.h"

namespace xml_compiler {

enum class EventType
{
  kStartElement,
  kEndElement,
  kText
};

class XMLAttribute
{
 public:
  constexpr XMLAttribute() {}
  constexpr XMLAttribute(const conststr& key, const conststr& value) : key_(key), value_(value) {}

  constexpr conststr GetKey() const { return key_; }
  constexpr conststr GetValue() const { return value_; }

  constexpr void SetKey(conststr key) {key_ = key;}
  constexpr void SetValue(conststr value) {value_ = value;}

  constexpr void Clear()
  {
    key_ = conststr();
    value_ = conststr();
  }

 private:
  conststr key_;
  conststr value_;
};

class XMLAttributeList
{
 public:
  constexpr XMLAttributeList() : size_(0) {}
  constexpr void AddAttribute(const XMLAttribute& attr)
  {
    if( size_ < 16 ) {
      attributes_[size_] = attr;
      ++size_;
    }
  }

  constexpr XMLAttribute GetAttribute(std::size_t index) const
  {
    return index < size_? attributes_[index] : throw std::out_of_range("");    
  }

  constexpr std::size_t GetSize() const { return size_; }
  
 private:
  std::size_t size_;

  XMLAttribute attributes_[128];
};

class XMLElement
{
 public:
  constexpr XMLElement() {}
  constexpr void AddAttribute(const XMLAttribute& attr)
  {
    attr_list_.AddAttribute(attr);
  }

  constexpr void SetName( const conststr& name ) { name_ = name;}
  constexpr conststr GetName() const { return name_;}
  constexpr XMLAttributeList GetAttributeList() const { return attr_list_; }
  constexpr void Clear() { name_ = conststr(); attr_list_ = XMLAttributeList(); }

 private:
  conststr name_;
  XMLAttributeList attr_list_;

};

class Event
{
 public:
  constexpr Event() : Event(EventType::kStartElement){}
  constexpr Event(EventType type) : type_(type){}

  constexpr EventType GetType() const { return type_; }
  constexpr XMLElement GetElement() const { return element_; }
  

 public:
  EventType type_;
  XMLElement element_;
};

class EventList
{
 public:
  constexpr EventList() : size_(0),
                          typelist_{EventType::kStartElement,}
  {}

  constexpr std::size_t GetSize() const { return size_; }
  constexpr Event GetEvent(std::size_t i) const
  {
    return i < size_ ? list_[i] :throw std::out_of_range("");
  }
  constexpr void PushEvent(const Event& event);

  Event list_[255];
  EventType typelist_[255];
  int size_;

};



template<EventType Type>
class NewEvent;

template<>
class NewEvent<EventType::kStartElement>
{
 public:
  constexpr NewEvent() {}
  constexpr void CopyFrom(const Event& event)
  {
    element_ = event.element_;
  }

 public:
  XMLElement element_;
  
};

template<>
class NewEvent<EventType::kEndElement>
{
 public:
  constexpr NewEvent() {}
  constexpr void CopyFrom(const Event& event)
  {
    element_ = event.element_;
  }
 public:
  XMLElement element_;
};


template<>
class NewEvent<EventType::kText>
{
 public:
  constexpr NewEvent() {}

};


template<EventType Type>
constexpr void InsertNewEvent(const NewEvent<Type>& event)
{
  
}

template<>
constexpr void InsertNewEvent<EventType::kStartElement>(const NewEvent<EventType::kStartElement>& event)
{
  
}

template<>
constexpr void InsertNewEvent<EventType::kEndElement>(const NewEvent<EventType::kEndElement>& event)
{
  
}
template<>
constexpr void InsertNewEvent<EventType::kText>(const NewEvent<EventType::kText>& event)
{
  
}

template<std::size_t Size>
class FixedEventList
{
 public:
  constexpr FixedEventList(){}
  constexpr void CopyFrom(const EventList& event_list);

  std::size_t GetSize() const { return Size; }
  Event GetEvent(std::size_t i) const { return list_[i]; }
  //constexpr void pushEvent(EventType type, const char* name, int name_len);
 private:
  Event list_[Size];
};

template<std::size_t Size>
constexpr void FixedEventList<Size>::CopyFrom(const EventList& event_list)
{
  for( auto i =0 ; i<Size ; ++i ) {
    list_[i] = event_list.list_[i];
  }
}


constexpr void EventList::PushEvent(const Event& event)
{
  if( size_ < 255 ) {
    list_[size_] = event;
    ++size_;
  }
}


class XMLParser
{
 public:
  typedef void (XMLParser::*CharFunc)(char);
  constexpr XMLParser() : char_func_(&XMLParser::CharOnContent),
                          stream_(nullptr),
                          stream_len_(0),
                          progress_(nullptr)
  {}
  constexpr EventList parse(const char* xml);

 private:
  constexpr void CharOnIdle(char ch);
  constexpr void CharOnMarkup(char ch);
  constexpr void CharOnContent(char ch);
  
  constexpr void CharOnStartMarkup(char ch);
  constexpr void CharOnEndMarkup(char ch);
  constexpr void CharOnEmptyMarkup(char ch);
  
  constexpr void CharOnAttributeKey(char ch);
  constexpr void CharOnAttributeValueBegin(char ch);
  constexpr void CharOnAttributeValue(char ch);
  constexpr void CharOnAttributeValueEnd(char ch);
  constexpr void OnMarkup();

  constexpr void CallStartElement();
  constexpr void CallEndElement();

  constexpr void PushToStream();
  constexpr void ClearStream();

 private:
  CharFunc char_func_;
  //void (XMLParser::*char_func_)(char ch);
  const char* progress_;
  const char* stream_;
  int stream_len_;
  EventList list_;
  XMLElement element_;
  XMLAttribute attr_;
};


constexpr void XMLParser::PushToStream()
{
  if( stream_len_ == 0 ) {
    stream_ = progress_;
  }

  ++stream_len_;
}

constexpr void XMLParser::ClearStream()
{
  stream_ = nullptr;
  stream_len_ = 0;
}

constexpr EventList XMLParser::parse(const char* xml)
{
  auto index = 0;
  progress_ = xml;
  
  while( true ) {
    auto buf = progress_[0];
    if( buf ) {
      (this->*char_func_)(buf);
    } else {
      break;
    }
    ++progress_;
  }

  return list_;
}

// template<std::size_t Index, EventList& list>
// constexpr auto GetEventType()
// {
//   //constexpr EventList list2 = list;
//   return NewEvent<list.list_[Index].type_>();
// }

// template <std::size_t... Is>
// constexpr auto CreateTupleImpl(const EventList& list, std::index_sequence<Is...> ) {
//   return std::make_tuple(list.list_[Is].type_...);
//   //return std::make_tuple(GetEventType<Is>(list)...);
// }

// template <std::size_t N>
// constexpr auto CreateTuple(const EventList& list)
// {
//   return CreateTupleImpl(list, std::make_index_sequence<N>{} );
// }


// template<class Tuple, std::size_t... Is>
// constexpr auto CreateTuple2Impl(const Tuple& t, std::index_sequence<Is...>)
// {
//   return std::make_tuple(NewEvent<std::get<Is>(t)>()...);
// }

// template<class... Args>
// constexpr auto CreateTuple2( const std::tuple<Args...>& t )
// {
//   return CreateTuple2Impl(t, std::index_sequence_for<Args...>{});
// }

constexpr void XMLParser::CallStartElement()
{
  Event event;
  event.element_ = element_;
  list_.PushEvent(event);

  ClearStream();
  element_.Clear();
  attr_.Clear();
}

constexpr void XMLParser::CallEndElement()
{
  //auto element = ParseElement(stream_, stream_len_);

  Event event(EventType::kEndElement);
  event.element_ = element_;
  list_.PushEvent(event);

  ClearStream();
  element_.Clear();
  attr_.Clear();
}



constexpr void XMLParser::CharOnIdle(char ch)
{
  switch(ch) {
    case '<':
      char_func_ = &XMLParser::CharOnMarkup;
      break;
    case '/':
      break;
    case '>':
      break;
    case ' ':
    case '\n':
    case '\t':
      break;
      
    default:
      ++stream_len_;
      break;
      
  }
}

constexpr void XMLParser::CharOnMarkup(char ch)
{
  switch(ch) {
    case '/':
      stream_ = progress_ + 1;
      char_func_ = &XMLParser::CharOnEndMarkup;
      break;
    case '!':
      
      break;
      
    case ' ':
    case '\n':
    case '\t':
      break;
      
    default:
      stream_ = progress_;
      CharOnStartMarkup(ch);
      char_func_ = &XMLParser::CharOnStartMarkup;
      break;
  }
}

constexpr void XMLParser::CharOnStartMarkup(char ch)
{
  switch(ch) {
    case '/':
      // //empty? no!
      // CallStartElement();
      // CallEndElement();
      break;
    case '>':
      element_.SetName(conststr(stream_, stream_len_));
      ClearStream();
      
      char_func_ = &XMLParser::CharOnContent;
      CallStartElement();
      break;
    case ' ':
      char_func_ = &XMLParser::CharOnAttributeKey;
      element_.SetName(conststr(stream_, stream_len_));
      
      ClearStream();
      break;
    default:
      //element name
      ++stream_len_;
      break;
  }
}

constexpr void XMLParser::CharOnEmptyMarkup(char ch)
{
  char_func_ = &XMLParser::CharOnContent;
  
}

constexpr void XMLParser::CharOnEndMarkup(char ch)
{
  switch(ch) {
    case '>': {
      element_.SetName(conststr(stream_, stream_len_));
      ClearStream();

      CallEndElement();
      char_func_ = &XMLParser::CharOnContent;
      break;
    }
    case ' ':
    case '\n':
    case '\t':
      break;
      
    default:
      ++stream_len_;
      break;
  }
}

constexpr void XMLParser::CharOnContent(char ch)
{
  switch(ch) {
    case '<':
      char_func_ = &XMLParser::CharOnMarkup;
      OnMarkup();
      break;
    case ' ':
    case '\n':
    case '\t':
      break;
      
    default:
      PushToStream();
      break;
  }
}



constexpr void XMLParser::CharOnAttributeKey(char ch)
{
  switch(ch) {
    case '=':
      char_func_ = &XMLParser::CharOnAttributeValueBegin;
      attr_.SetKey(conststr(stream_, stream_len_));
      ClearStream();
      break;
    default:
      if( stream_len_ == 0 ) {
        stream_ = progress_;
      }
      ++stream_len_;
      break;
  }
}

constexpr void XMLParser::CharOnAttributeValueBegin(char ch)
{
  switch(ch) {
    case '"':
    case '\'':
      char_func_ = &XMLParser::CharOnAttributeValue;
      break;
    default:
      break;
  }
}

constexpr void XMLParser::CharOnAttributeValue(char ch)
{
  switch(ch) {
    case '"':
    case '\'':
      char_func_ = &XMLParser::CharOnAttributeValueEnd;
      attr_.SetValue(conststr(stream_, stream_len_));
      element_.AddAttribute(attr_);

      ClearStream();
      attr_ = XMLAttribute();
      break;
    default:
      if( stream_len_ == 0 ) {
        stream_ = progress_;
      }
      ++stream_len_;
      break;
  }
}

constexpr void XMLParser::CharOnAttributeValueEnd(char ch)
{
  switch(ch) {
    case ' ': //next attr
      char_func_ = &XMLParser::CharOnAttributeKey;
      break;
    case '>':
      char_func_ = &XMLParser::CharOnContent;
      CallStartElement();
      break;
    default:
      break;
  }
}


constexpr void XMLParser::OnMarkup()
{
  if( stream_len_ > 0 ) {
    // TextElement;
    list_.PushEvent(EventType::kText, stream_, stream_len_);
    //list_.PushEvent(Event(EventType::kText));
    ClearStream();
    // stream_ += stream_len_;
    // stream_len_ = 0;
  }
  
}



}  // speedup

#endif /* XML_PARSER_H_ */

