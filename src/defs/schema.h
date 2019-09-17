#ifndef DEFS_SCHEMA_H_
#define DEFS_SCHEMA_H_

// Schema definition for RESTful APIs.

namespace defs {

namespace schema {

const char* const kVersion = "version";

namespace response {
const char* const kCode = "code";
const char* const kMessage = "message";
const char* const kData = "data";
}  // response

}  // namespace schema

}  // namespace defs

#endif  // DEFS_SCHEMA_H_
