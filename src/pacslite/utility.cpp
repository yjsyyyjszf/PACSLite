#include "pacslite/utility.h"

#include <ShlObj.h>
#include <DbgHelp.h>
#include <Windows.h>

#include "base/file_system.h"
#include "base/encoding.h"

#include "spdlog/spdlog.h"

namespace pacslite {

static int GenerateMiniDump(PEXCEPTION_POINTERS exception_pointer) {
  typedef BOOL(WINAPI * MiniDumpWriteDumpT)(
    HANDLE,
    DWORD,
    HANDLE,
    MINIDUMP_TYPE,
    PMINIDUMP_EXCEPTION_INFORMATION,
    PMINIDUMP_USER_STREAM_INFORMATION,
    PMINIDUMP_CALLBACK_INFORMATION
    );

  // Get function MiniDumpWriteDump from "DbgHelp.dll"
  MiniDumpWriteDumpT pfn_mini_dump_write_dump = NULL;
  HMODULE dbg_help_dll = LoadLibrary(L"DbgHelp.dll");
  if (dbg_help_dll== NULL) {
    return EXCEPTION_CONTINUE_EXECUTION;
  }

  pfn_mini_dump_write_dump = (MiniDumpWriteDumpT)GetProcAddress(
    dbg_help_dll, "MiniDumpWriteDump");

  if (pfn_mini_dump_write_dump == NULL) {
    FreeLibrary(dbg_help_dll);
    return EXCEPTION_CONTINUE_EXECUTION;
  }

  // Create dump file
  TCHAR file_name[MAX_PATH] = { 0 };
  SYSTEMTIME local_time;
  GetLocalTime(&local_time);
  wsprintf(file_name, L"pacslite-%04d%02d%02d-%02d%02d%02d.dmp",
           local_time.wYear, local_time.wMonth, local_time.wDay,
           local_time.wHour, local_time.wMinute, local_time.wSecond);

  std::wstring file_path = base::JoinPath(GetOrCreateLogDir(), file_name);

  HANDLE dump_file = CreateFile(file_path.c_str(),
                                GENERIC_READ | GENERIC_WRITE,
                                FILE_SHARE_WRITE | FILE_SHARE_READ, 0,
                                CREATE_ALWAYS, 0, 0);
  if (dump_file == INVALID_HANDLE_VALUE) {
    FreeLibrary(dbg_help_dll);
    return EXCEPTION_CONTINUE_EXECUTION;
  }

  // write dump file
  MINIDUMP_EXCEPTION_INFORMATION exp_param;
  exp_param.ThreadId = GetCurrentThreadId();
  exp_param.ExceptionPointers = exception_pointer;
  exp_param.ClientPointers = FALSE;
  pfn_mini_dump_write_dump(GetCurrentProcess(), GetCurrentProcessId(),
                           dump_file, MiniDumpWithDataSegs,
                           (exception_pointer ? &exp_param : NULL), NULL, NULL);

  // free files
  CloseHandle(dump_file);
  FreeLibrary(dbg_help_dll);

  return EXCEPTION_EXECUTE_HANDLER;
}

static LONG WINAPI ExceptionFilter(LPEXCEPTION_POINTERS exception_info_p) {
  if (IsDebuggerPresent()) {
    return EXCEPTION_CONTINUE_SEARCH;
  }

  return GenerateMiniDump(exception_info_p);
}

void SetupMiniDump() {
  SetUnhandledExceptionFilter(ExceptionFilter);
}

std::wstring GetOrCreateOutputDir() {
  std::wstring output_dir = L"C:\\pacslite";
  if (!base::PathExists(output_dir)) {
    std::string error_msg;
    if (!base::CreateDir(output_dir, &error_msg)) {
      SPDLOG_ERROR("Create output dir failed, path:{}, error_msg:{}", base::Utf16ToUtf8(output_dir), error_msg);
      return L"";
    }
  }
  return output_dir;
}

std::wstring GetOrCreateLogDir() {
  std::wstring output_dir = GetOrCreateOutputDir();
  if (output_dir.empty()) {
    return L"";
  }

  std::wstring log_dir = base::JoinPath(output_dir, L"log");

  if (!base::PathExists(log_dir)) {
    std::string error_msg;
    if (!base::CreateDir(log_dir, &error_msg)) {
      SPDLOG_ERROR("Create log dir failed, path:{}, error_msg:{}", base::Utf16ToUtf8(log_dir), error_msg);
      return L"";
    }
  }
  return log_dir;

}
}  // namespace pacslite
