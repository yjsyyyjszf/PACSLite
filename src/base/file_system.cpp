#include "base/file_system.h"

#include <cstdlib>
#include <sstream>
#include <fstream>

#include "Windows.h"

#include "base/encoding.h"

#include "boost/filesystem.hpp"
#include "boost/system/error_code.hpp"

namespace base {

std::wstring JoinPath(const std::wstring& parent_path,
                      const std::wstring& child_path) {
  boost::filesystem::path path(parent_path);
  path /= child_path;

  return path.wstring();
}

std::string JoinPath(const std::string& utf8_parent_path,
                     const std::string& utf8_child_path) {
  std::wstring parent_path = Utf8ToUtf16(utf8_parent_path);
  std::wstring child_path = Utf8ToUtf16(utf8_child_path);

  std::wstring path = JoinPath(parent_path, child_path);

  return base::Utf16ToUtf8(path);
}

bool PathExists(const std::wstring& path) {
  boost::system::error_code ec;
  return boost::filesystem::exists(path, ec);
}

bool PathExists(const std::string& utf8_path) {
  std::wstring utf16_path = base::Utf8ToUtf16(utf8_path);
  return PathExists(utf16_path);
}

bool CreateDir(const std::wstring& dir, std::string* error_msg) {
  boost::system::error_code ec;
  if (!boost::filesystem::create_directories(dir, ec)) {
    if (error_msg != nullptr) {
      *error_msg = ec.message();
    }
    return false;
  }

  return true;
}

bool CreateDir(const std::string& utf8_dir, std::string* error_msg) {
  std::wstring dir = Utf8ToUtf16(utf8_dir);

  return CreateDir(dir);
}

bool RemoveDir(const std::wstring& dir, std::string* error_msg) {
  boost::system::error_code ec;
  if (!boost::filesystem::remove_all(dir, ec)) {
    if (error_msg != nullptr) {
      *error_msg = ec.message();
    }
    return false;
  }

  return true;
}

bool RemoveDir(const std::string& utf8_dir, std::string* error_msg) {
  std::wstring dir = Utf8ToUtf16(utf8_dir);

  return RemoveDir(dir, error_msg);
}

std::wstring GetParentPath(const std::wstring& path) {
  boost::filesystem::path temp_path(path);
  return temp_path.parent_path().wstring();
}

bool ReadTextFile(const std::wstring& file_path, std::string* file_content,
                  bool remove_eol) {
  assert(file_content != nullptr);

  std::ifstream file_stream;
  file_stream.open(file_path);
  if (file_stream.fail()) {
    return false;
  }

  std::stringstream str_stream;

  if (remove_eol) {
    std::string line;
    while (!file_stream.eof()) {
      std::getline(file_stream, line);
      str_stream << line;
    }
  } else {
    str_stream << file_stream.rdbuf();
  }

  if (str_stream.fail()) {
    return false;
  }

  *file_content = str_stream.str();

  return true;
}

}  // namespace base
