// Define WIN32_LEAN_AND_MEAN so that Boost.Asio won't complain
// WinSock.h already included.
// See: https://stackoverflow.com/q/9750344
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "base/win_service.h"
#include "base/encoding.h"

#include "webcc/server.h"

#include "pacslite/logging.h"
#include "pacslite/utility.h"
#include "pacslite/version_view.h"

using namespace pacslite;

// The service name doesn't have to be the EXE name.
// The service name will be used to identify the service.
// Use a pretty long name to avoid name conflict.
// Won't be displayed to the end user.
LPCTSTR const kServiceName = L"pacslite";

LPCTSTR const kServiceDisplayName = L"PACSLite";

// TODO
LPCTSTR const kServiceDescription = L"A light pacs service.";

// service handle.
SERVICE_STATUS_HANDLE g_service_handle;

// service status.
SERVICE_STATUS g_service_status;

//------------------------------------------------------------------------------

void StartServer(const std::wstring& db_file_path = L"",
                 const std::wstring& config_file_path = L"") {
  SPDLOG_TRACK_METHOD;

  // Reset log level according to config.
  //TODO: Need to be configured.
  std::string log_level = "debug";
  SetLogLevel(log_level);
  SPDLOG_INFO("Set Log level:{}", log_level);

  try {
    //TODO: Port need to be configured.
    int port = 52030;
    webcc::Server server(port);
    SPDLOG_INFO("Port:{}", port);

    server.Route("/version",
                 std::make_shared<VersionView>(),
                 {"GET"});
    SPDLOG_INFO("Enable version view");

    //TODO: Need to be configured.
    int worker_thread_count = 2;
    server.Run(worker_thread_count);
    SPDLOG_INFO("Worker thread count:{}", worker_thread_count);

  } catch (const std::exception& e) {
    SPDLOG_CRITICAL("Server error:{}.", e.what());
    return;
  }
}

void SetSerivceStop() {
  g_service_status.dwWin32ExitCode = 0;
  g_service_status.dwCurrentState = SERVICE_STOPPED;
  g_service_status.dwCheckPoint = 0;
  g_service_status.dwWaitHint = 0;

  SetServiceStatus(g_service_handle, &g_service_status);
}

void WINAPI ServiceCtrlHandler(DWORD op_code) {
  switch (op_code) {
    case SERVICE_CONTROL_PAUSE:
      g_service_status.dwCurrentState = SERVICE_PAUSED;
      break;

    case SERVICE_CONTROL_CONTINUE:
      g_service_status.dwCurrentState = SERVICE_RUNNING;
      break;

    case SERVICE_CONTROL_STOP:
      SetSerivceStop();
      return;

    case SERVICE_CONTROL_INTERROGATE:
      break;

    default:
      break;
  }

  SetServiceStatus(g_service_handle, &g_service_status);
}

void WINAPI ServiceMain(DWORD argc, LPTSTR* argv) {
  g_service_handle = RegisterServiceCtrlHandler(kServiceName,
                                                &ServiceCtrlHandler);
  if (!g_service_handle) {
    return;
  }

  g_service_status.dwServiceType = SERVICE_WIN32;
  g_service_status.dwCurrentState = SERVICE_START_PENDING;
  g_service_status.dwControlsAccepted = SERVICE_ACCEPT_STOP;
  g_service_status.dwWin32ExitCode = 0;
  g_service_status.dwServiceSpecificExitCode = 0;
  g_service_status.dwCurrentState = SERVICE_RUNNING;
  g_service_status.dwCheckPoint = 0;
  g_service_status.dwWaitHint = 0;

  SetServiceStatus(g_service_handle, &g_service_status);

  StartServer();
}

int wmain(int argc, wchar_t* argv[]) {
  if (!InitSpdLog()) {
    SPDLOG_INFO("Init logging failed.");
    return -1;
  }
  SPDLOG_INFO("Init logging successfully.");

  SetupMiniDump();

  SPDLOG_INFO("--------------------------------------------------------------------------------");

  std::wstring start_mode;
  if (argc == 2) {
    start_mode = argv[1];
  }

  std::wstring db_file_path;
  std::wstring config_file_path;
  if (argc == 4) {
    start_mode = argv[1];
    db_file_path = argv[2];
    config_file_path = argv[3];
  }

  SPDLOG_DEBUG("Cmd param: {}", base::Utf16ToUtf8(start_mode));

  if (start_mode == L"--install") {
    bool success = base::win_service::Install(kServiceName, kServiceDisplayName, kServiceDescription);
    SPDLOG_DEBUG("Install service result: {}", success);

  } else if (start_mode == L"--remove") {
    bool success = base::win_service::Remove(kServiceName);
    SPDLOG_DEBUG("Remove service result: {}", success);

  } else if (start_mode == L"--start") {
    bool success = base::win_service::Start(kServiceName);
    SPDLOG_DEBUG("Start service result: {}", success);

  } else if (start_mode == L"--stop") {
    bool success = base::win_service::Stop(kServiceName);
    SPDLOG_DEBUG("Stop service result: {}", success);

  } else if (start_mode == L"--exec") {
    StartServer(db_file_path, config_file_path);

  } else {
    SERVICE_TABLE_ENTRY dispatch_table[] = {
      {(LPTSTR)kServiceName, &ServiceMain},
      {NULL, NULL}
    };

    StartServiceCtrlDispatcher(dispatch_table);
  }

  return 0;
}

