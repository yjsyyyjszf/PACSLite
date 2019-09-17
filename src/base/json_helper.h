#ifndef BASE_JSON_HELPER_H_
#define BASE_JSON_HELPER_H_

#include <string>

#include "json/json.h"

namespace base {

void JsonToString(const Json::Value& json, std::string* str);

bool StringToJson(const std::string& str, Json::Value* json,
                  std::string* error_msg = nullptr);

bool LoadJsonFromFile(const std::wstring& file_path, Json::Value* json,
                      std::string* error_msg = nullptr);

bool SaveJsonToFile(const std::wstring& file_path, const Json::Value& json);

}  // namespace base

#endif  // BASE_JSON_HELPER_H_
