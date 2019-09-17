#ifndef PACSLITE_UTILITY_H_
#define PACSLITE_UTILITY_H_

#include <string>

namespace pacslite {

std::wstring GetOrCreateOutputDir();
std::wstring GetOrCreateLogDir();

void SetupMiniDump();

}  // namespace pacslite

#endif  // PACSLITE_UTILITY_H_
