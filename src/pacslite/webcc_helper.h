#ifndef PACSLITE_WEBCC_HELPER_H_
#define PACSLITE_WEBCC_HELPER_H_

#include "json/json.h"

#include "defs/error_code.h"

namespace pacslite {

std::string ResponseToJsonStr(const defs::Error& error,
                              const Json::Value& data = Json::Value());

}  // namespace css

#endif  // PACSLITE_WEBCC_HELPER_H_
