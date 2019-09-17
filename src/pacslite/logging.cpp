#include "pacslite/logging.h"

#include "base/file_system.h"

#include "spdlog/sinks/rotating_file_sink.h"

#include "pacslite/utility.h"

namespace pacslite {

static spdlog::level::level_enum GetLogLevel(const std::string& level_name) {
  if (level_name == "trace") {
    return spdlog::level::trace;
  }

  if (level_name == "debug") {
    return spdlog::level::debug;
  }

  if (level_name == "info") {
    return spdlog::level::info;
  }

  if (level_name == "warn") {
    return spdlog::level::warn;
  }

  if (level_name == "error") {
    return spdlog::level::err;
  }

  if (level_name == "fatal") {
    return spdlog::level::critical;
  }

  return spdlog::level::debug;
}

bool InitSpdLog() {
  std::wstring log_dir = GetOrCreateLogDir();
  if (log_dir.empty()) {
    return false;
  }

  std::wstring log_path = base::JoinPath(log_dir, L"pacslite.log");

  // Create a file rotating logger with 5MB size max and 300 rotated files.
  auto file_logger = spdlog::rotating_logger_mt("file_logger",
                                                log_path,
                                                1024 * 1024 * 5,
                                                300);

  file_logger->set_level(spdlog::level::trace);
  file_logger->flush_on(spdlog::level::trace);
  file_logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e %z] [T:%-5t] [P:%-5P] "
                           "[%-5l] [%=30@] %v");

  spdlog::set_default_logger(file_logger);

  return true;
}

void SetLogLevel(const std::string& level_name) {
  auto log_level = GetLogLevel(level_name);
  spdlog::set_level(log_level);
  spdlog::flush_on(log_level);
}

}  // namespace pacslite
