#include "base/date_time.h"

#include "boost/date_time/gregorian/gregorian.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"

namespace base {

// Date string format: YYYY-MM-DD.
bool DateStrToTm(const std::string& date_str, std::tm* tm_date,
                 std::string* error_msg) {
  assert(tm_date != nullptr);

  try {
    boost::gregorian::date date = boost::gregorian::from_string(date_str);
    *tm_date = boost::gregorian::to_tm(date);
  } catch (const std::exception& e) {
    if (error_msg != nullptr) {
      *error_msg = e.what();
    }
    return false;
  }

  return true;
}

// Date string format: YYYY-MM-DD.
bool TmToDateStr(const std::tm& tm_date, std::string* date_str,
                 std::string* error_msg) {
  assert(date_str != nullptr);

  try {
    boost::gregorian::date date = boost::gregorian::date_from_tm(tm_date);
    *date_str = boost::gregorian::to_iso_extended_string(date);
  } catch (const std::exception& e) {
    if (error_msg != nullptr) {
      *error_msg = e.what();
    }
    return false;
  }

  return true;
}

bool DateTimeStrToTm(const std::string& date_time_str, std::tm* tm_date, std::string* error_msg) {
  assert(tm_date != nullptr);

  try {
    boost::posix_time::ptime  date_time = boost::posix_time::time_from_string(date_time_str);
    *tm_date = boost::posix_time::to_tm(date_time);
  } catch (const std::exception& e) {
    if (error_msg != nullptr) {
      *error_msg = e.what();
    }
    return false;
  }

  return true;
}

std::tm GetTmDateTime() {
  boost::posix_time::ptime time = boost::posix_time::second_clock::local_time();
  std::tm tm = boost::posix_time::to_tm(time);

  return tm;
}

}  // namespace base
