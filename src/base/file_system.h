#ifndef BASE_FILE_SYSTEM_H_
#define BASE_FILE_SYSTEM_H_

#include <string>

namespace base {

std::wstring JoinPath(const std::wstring& parent_path,
                      const std::wstring& child_path);

std::string JoinPath(const std::string& utf8_parent_path,
                     const std::string& utf8_child_path);

bool PathExists(const std::wstring& path);
bool PathExists(const std::string& utf8_path);

bool CreateDir(const std::wstring& dir, std::string* error_msg = nullptr);
bool CreateDir(const std::string& utf8_dir, std::string* error_msg = nullptr);

bool RemoveDir(const std::wstring& dir, std::string* error_msg = nullptr);
bool RemoveDir(const std::string& utf8_dir, std::string* error_msg = nullptr);

std::wstring GetParentPath(const std::wstring& path);

bool ReadTextFile(const std::wstring& file_path, std::string* file_content,
                  bool remove_eol = false);

}  // namespace base

#endif  // BASE_FILE_SYSTEM_H_
