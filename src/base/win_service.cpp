#include "base/win_service.h"

namespace base {

namespace win_service {

bool IsInstalled(LPCTSTR svc_name) {
  bool found = false;
  SC_HANDLE scm = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
  if (scm != NULL) {
    SC_HANDLE svc = OpenService(scm, svc_name, SERVICE_QUERY_STATUS);
    if (svc != NULL) {
      found = true;
      CloseServiceHandle(svc);
    }
    CloseServiceHandle(scm);
  }
  return found;
}

bool IsStarted(LPCTSTR svc_name) {
  SC_HANDLE scm = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
  if (scm == NULL) {
    return false;
  }

  SC_HANDLE svc = OpenService(scm, svc_name, SERVICE_QUERY_STATUS);
  if (svc == NULL) {
    CloseServiceHandle(scm);
    return false;
  }

  bool is_started = false;

  SERVICE_STATUS_PROCESS svc_status;
  DWORD byte_needed = 0;

  BOOL query_ok = QueryServiceStatusEx(svc,
                                       SC_STATUS_PROCESS_INFO,
                                       (LPBYTE)&svc_status,
                                       sizeof(SERVICE_STATUS_PROCESS),
                                       &byte_needed);
  if (query_ok == TRUE) {
    if (svc_status.dwCurrentState == SERVICE_RUNNING ||
        svc_status.dwCurrentState == SERVICE_START_PENDING) {
      is_started = true;
    }
  }

  CloseServiceHandle(svc);
  CloseServiceHandle(scm);

  return is_started;
}

bool Install(LPCTSTR svc_name,
             LPCTSTR svc_display_name,
             LPCTSTR svc_description) {
  SC_HANDLE scm = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
  if (scm == NULL) {
    return false;
  }

  // Get full path of the EXE.
  TCHAR path[MAX_PATH];
  if (!GetModuleFileName(NULL, path, MAX_PATH)) {
    return false;
  }

  SC_HANDLE svc = CreateService(scm,
                                svc_name,
                                svc_display_name,
                                SERVICE_ALL_ACCESS,
                                SERVICE_WIN32_OWN_PROCESS,
                                SERVICE_AUTO_START,
                                SERVICE_ERROR_NORMAL,
                                path,
                                NULL,
                                NULL,
                                NULL,
                                NULL,
                                NULL);
  if (svc == NULL) {
    CloseServiceHandle(scm);;
    return false;
  }

  SERVICE_DESCRIPTION sd;
  sd.lpDescription = (LPTSTR)svc_description;
  ChangeServiceConfig2(svc, SERVICE_CONFIG_DESCRIPTION, &sd);

  CloseServiceHandle(svc);
  CloseServiceHandle(scm);
  return true;
}

bool Remove(LPCTSTR svc_name) {
  SC_HANDLE scm = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
  if (scm == NULL) {
    return false;
  }

  SC_HANDLE svc = OpenService(scm, svc_name, DELETE);
  if (svc == NULL) {
    CloseServiceHandle(scm);
    return false;
  }

  BOOL delete_ok = DeleteService(svc);

  CloseServiceHandle(svc);
  CloseServiceHandle(scm);

  return !!delete_ok;
}

bool Start(LPCTSTR svc_name) {
  SC_HANDLE scm = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
  if (scm == NULL) {
    return false;
  }

  SC_HANDLE svc = OpenService(scm, svc_name,
                              SERVICE_QUERY_STATUS | SERVICE_START);
  if (svc == NULL) {
    CloseServiceHandle(scm);
    return false;
  }

  SERVICE_STATUS_PROCESS svc_status;
  DWORD byte_needed = 0;

  do {  // while(false)
    BOOL query_ok = QueryServiceStatusEx(svc,
                                         SC_STATUS_PROCESS_INFO,
                                         (LPBYTE)&svc_status,
                                         sizeof(SERVICE_STATUS_PROCESS),
                                         &byte_needed);
    if (!query_ok) {
      break;
    }

    if (svc_status.dwCurrentState == SERVICE_RUNNING) {
      break;
    }

    if (svc_status.dwCurrentState == SERVICE_START_PENDING) {
      // Do not wait longer than the wait hint. A good interval is 
      // one-tenth the wait hint, but no less than 1 second and no 
      // more than 10 seconds. 

      DWORD wait_time = svc_status.dwWaitHint / 10;

      if (wait_time < 1000) {
        wait_time = 1000;
      } else if (wait_time > 10000) {
        wait_time = 10000;
      }

      Sleep(wait_time);
    }

    if (!StartService(svc, 0, NULL)) {
      break;
    }
  } while (false);

  CloseServiceHandle(svc);
  CloseServiceHandle(scm);

  return true;
}

bool Stop(LPCTSTR svc_name) {
  SC_HANDLE scm = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
  if (scm == NULL) {
    return false;
  }

  SC_HANDLE svc = OpenService(scm,
                              svc_name,
                              SERVICE_STOP | SERVICE_QUERY_STATUS);
  if (svc == NULL) {
    CloseServiceHandle(scm);
    return false;
  }

  bool stopped = false;

  SERVICE_STATUS_PROCESS ssp;
  DWORD byte_needed = 0;

  DWORD wait_time = 0;

  for (int retry_times = 0; retry_times < 10; ++retry_times) {
    BOOL query_ok = QueryServiceStatusEx(svc,
                                         SC_STATUS_PROCESS_INFO,
                                         (LPBYTE)&ssp,
                                         sizeof(SERVICE_STATUS_PROCESS),
                                         &byte_needed);

    if (!query_ok) {
      break;
    }

    if (ssp.dwCurrentState == SERVICE_STOPPED) {
      stopped = true;
      break;
    }

    if (ssp.dwCurrentState == SERVICE_STOP_PENDING) {
      // For simplicity, don't use ssp.dwWaitHint.
      // ssp.dwWaitHint is 0 in most cases.
      Sleep(1000);  // Sleep 1 second.
      continue;
    }

    // Send a stop code to the service.
    if (!ControlService(svc, SERVICE_CONTROL_STOP, (LPSERVICE_STATUS)& ssp)) {
      break;
    }
  };

  CloseServiceHandle(svc);
  CloseServiceHandle(scm);
  return stopped;
}

bool Restart(LPCTSTR svc_name) {
  if (Stop(svc_name)) {
    return Start(svc_name);
  }
  return false;
}

}  // win_service

}  // namespace base
