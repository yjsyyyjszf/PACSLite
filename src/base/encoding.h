#ifndef BASE_ENCODING_H_
#define BASE_ENCODING_H_

#include <string>

namespace base {

// Convert UTF16 to UTF8.
std::string Utf16ToUtf8(const std::wstring& utf16_string);

// Convert UTF8 to UTF16.
std::wstring Utf8ToUtf16(const std::string& utf8_string);

// Convert UTF16 to ANSI.
// Characters might be lost during the conversion.
std::string Utf16ToAnsi(const std::wstring& utf16_string);

// Convert UTF8 to ANSI.
// Characters might be lost during the conversion.
std::string Utf8ToAnsi(const std::string& utf8_string);

std::wstring AnsiToUtf16(const std::string& ansi_string);

}  // namespace base

#endif  // BASE_ENCODING_H_
