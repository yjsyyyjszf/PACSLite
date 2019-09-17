#include "base/string_util.h"

#include "boost/algorithm/string.hpp"

namespace base {

bool StrToBool(const std::string& value, bool default_value) {
  if (boost::iequals(value, "false")) {
    return false;
  }

  if (boost::iequals(value, "true")) {
    return true;
  }

  return default_value;
}

int GetHashCode(const std::string& str) {
  std::size_t hash_code = std::hash<std::string>{}(str);;
  return static_cast<int>(hash_code);
}

}  // namespace base
