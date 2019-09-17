#ifndef BASE_STRING_UTIL_H_
#define BASE_STRING_UTIL_H_

#include <string>
#include <cassert>

#include "boost/lexical_cast.hpp"

namespace base {

// Usage:
//   LexicalCast<int>("123", 0);
//   LexicalCast<std::string>(123, "");
template <typename To, typename From>
To LexicalCast(const From& input, const To& default_output) {
  try {
    return boost::lexical_cast<To>(input);
  } catch (boost::bad_lexical_cast&) {
    return default_output;
  }
}

// Convert string "false"/"true" to bool variable.
bool StrToBool(const std::string& value, bool default_value);

int GetHashCode(const std::string& str);

}  // namespace base

#endif // BASE_STRING_UTIL_H_
