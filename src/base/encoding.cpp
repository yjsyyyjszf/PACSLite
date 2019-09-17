#include "base/encoding.h"

#include "windows.h"

namespace base {

// Wrapper for Windows API MultiByteToWideChar.
static std::wstring MB2WC(const std::string& input, unsigned int code_page) {
  if (input.empty()) {
    return L"";
  }

  int length = ::MultiByteToWideChar(code_page, 0, &input[0],
                                     static_cast<int>(input.size()),
                                     NULL, 0);

  std::wstring output(length, '\0');

  ::MultiByteToWideChar(code_page, 0, &input[0], static_cast<int>(input.size()),
                        &output[0], static_cast<int>(output.size()));

  return output;
}

// Wrapper for Windows API WideCharToMultiByte.
static std::string WC2MB(const std::wstring& input, unsigned int code_page) {
  if (input.empty()) {
    return "";
  }

  // There do have other code pages which require the flags to be 0, e.g.,
  // 50220, 50211, and so on. But they are not included in our charset
  // dictionary. So, only consider 65001 (UTF-8) and 54936 (GB18030).
  DWORD flags = 0;
  if (code_page != 65001 && code_page != 54936) {
    flags = WC_NO_BEST_FIT_CHARS | WC_COMPOSITECHECK | WC_DEFAULTCHAR;
  }

  int length = ::WideCharToMultiByte(code_page, flags, &input[0],
                                     static_cast<int>(input.size()), NULL, 0,
                                     NULL, NULL);

  std::string output(length, '\0');

  ::WideCharToMultiByte(code_page, flags, &input[0],
                        static_cast<int>(input.size()), &output[0],
                        static_cast<int>(output.size()), NULL, NULL);

  return output;
}

std::string Utf16ToUtf8(const std::wstring& utf16_string) {
  return WC2MB(utf16_string, CP_UTF8);
}

std::wstring Utf8ToUtf16(const std::string& utf8_string) {
  return MB2WC(utf8_string, CP_UTF8);
}

std::string Utf16ToAnsi(const std::wstring& utf16_string) {
  return WC2MB(utf16_string, CP_ACP);
}

std::string Utf8ToAnsi(const std::string& utf8_string) {
  return WC2MB(MB2WC(utf8_string, CP_UTF8), CP_ACP);
}

std::wstring AnsiToUtf16(const std::string& ansi_string) {
  return MB2WC(ansi_string, CP_ACP);
}

}  // namespace base
