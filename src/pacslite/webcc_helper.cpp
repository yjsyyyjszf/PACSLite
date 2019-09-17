#include "pacslite/webcc_helper.h"

#include "defs/schema.h"

#include "base/json_helper.h"

#include "pacslite/logging.h"

namespace pacslite {

std::string ResponseToJsonStr(const defs::Error& error,
                              const Json::Value& data) {
  Json::Value json;
  json[defs::schema::response::kCode] = static_cast<int>(error.code());
  json[defs::schema::response::kMessage] = error.msg();

  if (!data.isNull()) {
    json[defs::schema::response::kData] = data;
  }

  std::string str;
  base::JsonToString(json, &str);

  return str;
}
}  // namespace pacslite
