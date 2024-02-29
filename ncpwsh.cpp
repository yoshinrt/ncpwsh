/*****************************************************************************

	no cosole powershell

*****************************************************************************/

#include <windows.h>
#include <tchar.h>
#include <string>

/*** constant definition ****************************************************/

/*** gloval var definition **************************************************/

#define IsQuote(c)	((c) == _T('"') || (c) == _T('\''))

/*** main procedure *********************************************************/

int WINAPI _tWinMain(
	HINSTANCE	hInst,
	HINSTANCE	hPrevInst,
	LPTSTR		lpCmdLine,
	int			iCmdShow
){
	// Find exe file name

	LPTSTR	pCmdLine = GetCommandLine();

	TCHAR cQuote = _T('\0');

	LPTSTR pArg;
	LPTSTR pBackslash	= nullptr;
	LPTSTR pPeriod		= nullptr;
	LPTSTR pExecTail	= nullptr;

	while(*pCmdLine == _T(' ')) ++pCmdLine;
	
	for(pArg = pCmdLine; *pArg; ++pArg){
		if(*pArg == _T('.'))		pPeriod		= pArg;
		else if(*pArg == _T('\\'))	pBackslash	= pArg;

		if(!cQuote){
			// End of execute file name
			if(*pArg == _T(' ')) break;

			// Start of "..." or '...'
			if(IsQuote(*pArg)){
				cQuote = *pArg;
			}
		}
		else if(*pArg == cQuote){
			// End of "..." or '...'
			cQuote = _T('\0');
		}
	}

	// Change ext.

	if(pPeriod && (!pBackslash || pBackslash < pPeriod)){
		pExecTail = pPeriod;
	}else{
		pExecTail = pArg;
		if(pArg > pCmdLine && IsQuote(pArg[-1])) --pExecTail;
	}

	// Create new command line

	std::wstring	strNewCmdLine(_T("powershell -NoLogo -ExecutionPolicy RemoteSigned -File "));
	strNewCmdLine.append(pCmdLine, pExecTail - pCmdLine);
	strNewCmdLine += _T(".ps1");
	if(IsQuote(*pCmdLine)) strNewCmdLine += *pCmdLine;

	// Cat argument
	strNewCmdLine += pArg;

	//MessageBox(nullptr, strNewCmdLine.c_str(), GetCommandLine(), 0);

	//*** cleate a new process ***********************************************

	STARTUPINFO 		si;
	PROCESS_INFORMATION	pi;

	memset(&si, 0, sizeof(si));
	si.cb = sizeof(si);
	si.wShowWindow	= SW_HIDE;
	si.dwFlags		= STARTF_USESHOWWINDOW;

	if(CreateProcess(nullptr, &strNewCmdLine[0], nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si, &pi)){
		WaitForSingleObject(pi.hProcess, INFINITE);
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
	}

	return(0);
}
