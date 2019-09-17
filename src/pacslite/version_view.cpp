#include "pacslite/version_view.h"

#include <string>

#include "json/json.h"

#include "defs/error_code.h"

#include "base/json_helper.h"

#include "pacslite/webcc_helper.h"

namespace pacslite {

webcc::ResponsePtr VersionView::Get(webcc::RequestPtr request) {
  Json::Value response_json(Json::objectValue);
  response_json["version"] = "0.1.0";

  std::string data;
  base::JsonToString(response_json, &data);

  defs::Error error = defs::Error::Code::kOK;
  std::string response = ResponseToJsonStr(error, data);

  return webcc::ResponseBuilder{}.OK().Body(std::move(response)).Json().Utf8()();
}

}  // namespace pacslite
