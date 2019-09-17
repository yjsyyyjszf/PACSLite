#ifndef BASE_WIN_SERVICE_H_
#define BASE_WIN_SERVICE_H_

#include <Windows.h>

namespace base {

namespace win_service {

bool IsInstalled(LPCTSTR svc_name);

bool IsStarted(LPCTSTR svc_name);

bool Install(LPCTSTR svc_name,
             LPCTSTR svc_display_name,
             LPCTSTR svc_description);

bool Remove(LPCTSTR svc_name);

bool Start(LPCTSTR svc_name);

bool Stop(LPCTSTR svc_name);

bool Restart(LPCTSTR svc_name);

}  // namespace win_service

}  // namespace base

#endif  // BASE_WIN_SERVICE_H_
