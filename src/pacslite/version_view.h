#ifndef PACSLITE_VERSION_VIEW_H_
#define PACSLITE_VERSION_VIEW_H_

#include "webcc/response_builder.h"
#include "webcc/server.h"

namespace pacslite {

class VersionView : public webcc::View {
public:
  webcc::ResponsePtr Handle(webcc::RequestPtr request) override {
    return Get(request);
  }

private:
  webcc::ResponsePtr Get(webcc::RequestPtr request);
};

}  // namespace pacslite

#endif  // PACSLITE_VERSION_VIEW_H_
