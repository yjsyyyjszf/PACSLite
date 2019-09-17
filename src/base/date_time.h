#ifndef BASE_DATE_TIME_H_
#define BASE_DATE_TIME_H_

#include <string>

namespace base {

// Date string format: YYYY-MM-DD.
bool DateStrToTm(const std::string& date_str, std::tm* tm_date,
                 std::string* error_msg = nullptr);

// Date string format: YYYY-MM-DD.
bool TmToDateStr(const std::tm& tm_date, std::string* date_str,
                 std::string* error_msg = nullptr);

// Date time string format: YYYY-MM-DD HH:MM:SS
bool DateTimeStrToTm(const std::string& date_time_str, std::tm* tm_date, std::string* error_msg = nullptr);

std::tm GetTmDateTime();

}  // namespace base

#endif  // BASE_DATE_TIME_H_
