#ifndef DEFS_ERROR_CODE_H_
#define DEFS_ERROR_CODE_H_

#include <string>

namespace defs {

class Error {
public:
  enum class Code {
    kOK = 0,
    kUnknown = 1,
  };

public:
  Error(Code code = Code::kOK)
    : code_(code)
    , msg_(GetErrorMsg(code)) {
  }

  Code code() const {
    return code_;
  }

  const std::string& msg() const {
    return msg_;
  }

  operator bool() const {
    return code_ != Code::kOK;
  }

private:
  std::string GetErrorMsg(Error::Code code) {
    if (code == Error::Code::kUnknown) {
      return "Unknown";
    }

    if (code == Error::Code::kOK) {
      return "OK";
    }

    return "";
  }

private:
  Code code_;
  std::string msg_;
};

}  // namespace defs

#endif  // DEFS_ERROR_CODE_H_
