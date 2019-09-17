#ifndef PACSLITE_LOGGING_H_
#define PACSLITE_LOGGING_H_

#include <string>

#include "spdlog/spdlog.h"

#define SPDLOG_TRACK_METHOD pacslite::TrackMethodLog track_method_log(__FUNCTION__)

namespace pacslite {

// Initialize the log file to the following file path:
// "C:\pacslite_log"
bool InitSpdLog();

// Log level: "trace", "debug", "info", "warn", "error", "fatal".
void SetLogLevel(const std::string& level_name);

class TrackMethodLog {
public:
  explicit TrackMethodLog(const std::string& method) : method_(method) {
    SPDLOG_INFO("Enter : {}()", method_);
  }

  ~TrackMethodLog() {
    SPDLOG_INFO("Leave : {}()", method_);
  }

private:
  std::string method_;
};

}  // namespace pacslite

#endif  // PACSLITE_LOGGING_H_
