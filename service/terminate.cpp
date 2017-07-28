//
// The MIT License (MIT)
//
// Copyright (c) 2016 GoDaddy Operating Company, LLC.
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to permit
// persons to whom the Software is furnished to do so, subject to the
// following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//

#include "warning.hpp"

#include "terminate.hpp"

#include "ProcFilterEvents.h"


bool
TerminateProcessByPid(DWORD dwProcessId, bool bLog, const WCHAR *lpszFileName, const WCHAR *lpszFileBlockRuleNames, const WCHAR *lpszMemoryBlockRuleNames)
{
	DWORD dwErrorCode = ERROR_SUCCESS;
	bool rv = false;
	HANDLE h = OpenProcess(PROCESS_TERMINATE, FALSE, dwProcessId);
	if (h) {
		rv = TerminateProcess(h, 'ARAY') == TRUE;
		dwErrorCode = GetLastError();
		CloseHandle(h);
	}

	if (bLog) {
		if (rv) {
			EventWritePROCESS_TERMINATED(dwProcessId, lpszFileName, lpszFileBlockRuleNames, lpszMemoryBlockRuleNames);
		} else if (dwErrorCode != ERROR_ACCESS_DENIED) {
			// Only log if access wasn't denied, often access is denied if a process create event blocks a process before a dll load event does
			// since DLL load events still happen even if the process is to be blocked by the kernel
			Warning(L"Unable to terminate process with Process ID %u and filename \"%ls\": %d", dwProcessId, lpszFileName ? lpszFileName : L"None", dwErrorCode);
		}
	}

	return rv;
}

