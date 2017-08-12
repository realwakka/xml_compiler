#ifndef XML_COMPILER_CONSTSTR_H_
#define XML_COMPILER_CONSTSTR_H_

#include <tuple>

namespace xml_compiler {

class conststr {
 private:
  const char* p_;
  std::size_t sz_;
  
 public:
  constexpr conststr() : p_(nullptr), sz_(0) {}
  constexpr conststr(const char* p, std::size_t sz) : p_(p), sz_(sz) {}
  
  template<std::size_t N>
  constexpr conststr(const char(&a)[N]): p_(a), sz_(N - 1) {}

  constexpr char operator[](std::size_t n) const
  {
    return n < sz_? p_[n] : throw std::out_of_range("");
  }
  constexpr std::size_t size() const { return sz_; }
  constexpr conststr substr(std::size_t begin, std::size_t end) const
  {
    conststr str;
    str.p_ = p_ + begin;
    str.sz_ = end-begin;
    return str;
  }

  std::string str() const
  {
    std::string str;
    for( auto i=0 ; i<sz_ ; ++i )
      str += p_[i];
    return str;
  }

};

constexpr std::tuple<conststr, conststr> strtok(const conststr& str, char delimiter)
{
  int begin = -1;
  for( auto i=0 ; i<str.size() ; ++i ) {
    if( str[i] == delimiter ) {
      if( begin == -1 ) {
        continue;
      } else {
        return std::make_tuple(str.substr(begin, i), str.substr(i, str.size()));
      }
    } else {
      if( begin >= 0 ) {
        continue;
      } else {
        begin = i;
      }
    }
  }

  if( begin < 0 )
    return std::make_tuple(conststr(), conststr());
  else
    return std::make_tuple(str.substr(begin, str.size()), conststr());
}


}  // xml_compiler

#endif /* XML_COMPILER_CONSTSTR_H_ */
