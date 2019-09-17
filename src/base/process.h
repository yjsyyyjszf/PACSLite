#ifndef BASE_PROCESS_H_
#define BASE_PROCESS_H_

#include <Windows.h>

#include <cmath>
#include <string>

namespace base {

// Execute a command line, return process ID.
unsigned long ExecuteCommandLine(const std::wstring& cmd_line,
                                 bool create_new_process_group = false,
                                 bool wait = false);

void KillProcessById(unsigned long pid);

// Execute command line in sync mode, and specify a certain timeout (in
// millisec)
bool ExecuteCmdAndWait(const std::wstring& cmd_line, unsigned long time_out,
                       bool optional_show_window = false);

// Execute command line in sync mode, and will wait until the command return
bool ExecuteCmdAndWaitUntilDone(const std::wstring& cmd_line,
                                bool optional_show_window = false);

// Create a child process to execute the given command line.
bool CreateChildProcess(PROCESS_INFORMATION& process_info,
                        const std::wstring& cmd_line,
                        bool optional_show_window = false);

}  // namespace base

#endif  // BASE_PROCESS_H_
