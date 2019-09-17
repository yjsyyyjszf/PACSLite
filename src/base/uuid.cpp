#include "base/uuid.h"

#include <sstream>

#include "boost/uuid/random_generator.hpp"
#include "boost/uuid/uuid_io.hpp"

namespace base {

std::string RandomUuid() {
  boost::uuids::uuid u = boost::uuids::random_generator()();
  std::stringstream ss;
  ss << u;
  return ss.str();
}

}  // namespace base
