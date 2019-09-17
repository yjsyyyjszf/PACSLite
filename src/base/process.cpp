#include "base/process.h"

#include <cassert>

namespace base {

static void ReleaseProcessInformation(
    PROCESS_INFORMATION& process_information) {
  if (process_information.hThread != NULL) {
    CloseHandle(process_information.hThread);
  }

  if (process_information.hProcess != NULL) {
    CloseHandle(process_information.hProcess);
  }
}

unsigned long ExecuteCommandLine(const std::wstring& cmd_line,
                                 bool create_new_process_group,
                                 bool wait) {
  STARTUPINFOW si;
  PROCESS_INFORMATION pi;

  ZeroMemory(&si, sizeof(si));
  si.cb = sizeof(si);
  ZeroMemory(&pi, sizeof(pi));

  DWORD create_flags = CREATE_NEW_CONSOLE;
  if (create_new_process_group) {
    create_flags |= CREATE_NEW_PROCESS_GROUP;
  }

  const BOOL ret = CreateProcessW(NULL,
                                  (LPWSTR)cmd_line.c_str(),
                                  NULL,
                                  NULL,
                                  FALSE,
                                  create_flags,
                                  NULL,
                                  NULL,
                                  &si,
                                  &pi);

  if (ret == FALSE) {
    return 0;
  }

  if (wait) {
    // Wait for the process to finish
    unsigned long rc = WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return 0;
  }

  return pi.dwProcessId;
}

void KillProcessById(unsigned long pid) {
  const HANDLE handle = OpenProcess(PROCESS_TERMINATE, 0, pid);
  TerminateProcess(handle, 0);
  CloseHandle(handle);
}

bool ExecuteCmdAndWait(const std::wstring& cmd_line, unsigned long time_out,
                       bool optional_show_window) {
  PROCESS_INFORMATION procInfo = {0};

  if (CreateChildProcess(procInfo, cmd_line, optional_show_window)) {
    // Wait for the process to finish
    unsigned long rc = WaitForSingleObject(procInfo.hProcess,  // process handle
                                           time_out);
    ReleaseProcessInformation(procInfo);
    return rc == WAIT_OBJECT_0;
  }

  return false;
}

bool ExecuteCmdAndWaitUntilDone(const std::wstring& cmd_line,
                                bool optional_show_window) {
  return ExecuteCmdAndWait(cmd_line, INFINITE, optional_show_window);
}

bool CreateChildProcess(PROCESS_INFORMATION& process_information,
                        const std::wstring& cmd_line,
                        const bool optional_show_window) {
  STARTUPINFOW siStartInfo = {0};

  // Set up members of STARTUPINFO structure.
  siStartInfo.cb = sizeof(STARTUPINFO);

  if (!optional_show_window) {
    siStartInfo.dwFlags = STARTF_USESHOWWINDOW;
    siStartInfo.wShowWindow = SW_HIDE;
  }

  // Create the child process.
  BOOL ret =
      CreateProcessW(NULL, ((LPWSTR)cmd_line.c_str()),
                     NULL,          // process security attributes
                     NULL,          // primary thread security attributes
                     0,             // handles are inherited
                     0,             // creation flags
                     NULL,          // use parent's environment
                     NULL,          // use parent's current directory
                     &siStartInfo,  // STARTUPINFO pointer
                     &process_information);  // receives PROCESS_INFORMATION
  return ret != 0;
}
}  // namespace base
