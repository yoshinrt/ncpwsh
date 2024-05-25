/*****************************************************************************

	no cosole powershell

*****************************************************************************/

#include <windows.h>
#include <tchar.h>
#include <string>
#include <thread>

/*** constant definition ****************************************************/

#define WINDOW_TITLE	_T( "No Console Powershell" )		// NULL 可
#define WINDOW_CLASS	_T( "NcpwshAppClass" )

#define IsQuote(c)	((c) == _T('"') || (c) == _T('\''))

/*** gloval var definition **************************************************/

const TCHAR* szWinClassName = WINDOW_CLASS;

HINSTANCE	g_hInst;
HWND		g_hWnd;

/*** Create process thread **************************************************/

typedef struct {
	HWND	hWnd;
	int		iCmdShow;
} CPT_ARG_t;

void CreateProcessThread(
	CPT_ARG_t	*pCptARg
) {
	// Find exe file name

	LPTSTR	pCmdLine = GetCommandLine();

	TCHAR cQuote = _T('\0');

	LPTSTR pArg;
	LPTSTR pBackslash = nullptr;
	LPTSTR pPeriod = nullptr;
	LPTSTR pExecTail = nullptr;

	while (*pCmdLine == _T(' ')) ++pCmdLine;

	for (pArg = pCmdLine; *pArg; ++pArg) {
		if (*pArg == _T('.'))		pPeriod = pArg;
		else if (*pArg == _T('\\'))	pBackslash = pArg;

		if (!cQuote) {
			// End of execute file name
			if (*pArg == _T(' ')) break;

			// Start of "..." or '...'
			if (IsQuote(*pArg)) {
				cQuote = *pArg;
			}
		}
		else if (*pArg == cQuote) {
			// End of "..." or '...'
			cQuote = _T('\0');
		}
	}

	// Change ext.

	if (pPeriod && (!pBackslash || pBackslash < pPeriod)) {
		pExecTail = pPeriod;
	}
	else {
		pExecTail = pArg;
		if (pArg > pCmdLine && IsQuote(pArg[-1])) --pExecTail;
	}

	// Create new command line

	std::wstring	strNewCmdLine(_T("C:\\Windows\\System32\\WindowsPowerShell\\v1.0\\powershell.exe -NoProfile -NoLogo -ExecutionPolicy RemoteSigned -File "));
	strNewCmdLine.append(pCmdLine, pExecTail - pCmdLine);
	strNewCmdLine += _T(".ps1");
	if (IsQuote(*pCmdLine)) strNewCmdLine += *pCmdLine;

	// Cat argument
	strNewCmdLine += pArg;

	//MessageBox(nullptr, strNewCmdLine.c_str(), GetCommandLine(), 0);

	//*** cleate a new process ***********************************************

	STARTUPINFO 		si;
	PROCESS_INFORMATION	pi;

	memset(&si, 0, sizeof(si));
	si.cb = sizeof(si);
	si.wShowWindow =
		pCptARg->iCmdShow == SW_SHOWMINIMIZED ||
		pCptARg->iCmdShow == SW_MINIMIZE ||
		pCptARg->iCmdShow == SW_SHOWMINNOACTIVE ? pCptARg->iCmdShow : SW_HIDE;
	si.dwFlags = STARTF_USESHOWWINDOW;

	if (CreateProcess(nullptr, &strNewCmdLine[0], nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si, &pi)) {
		WaitForSingleObject(pi.hProcess, INFINITE);
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
	}

	PostMessage(pCptARg->hWnd, WM_CLOSE, 0, 0);
}

/*** window procedure *******************************************************/

LRESULT CALLBACK WindowProc(
	HWND	hWnd,
	UINT	Message,
	WPARAM	wParam,
	LPARAM	lParam
) {

	switch (Message) {
	case WM_DESTROY:				/* terminated by user					*/
		PostQuitMessage(1);
		break;

	default:
		return(DefWindowProc(hWnd, Message, wParam, lParam));
	}
	return(0);
}

/*** main procedure *********************************************************/

int WINAPI _tWinMain(
	HINSTANCE	hInst,
	HINSTANCE	hPrevInst,
	LPTSTR		lpCmdLine,
	int			iCmdShow
) {
	/* for creating window */

	HWND		hWnd;
	MSG			Msg;
	WNDCLASS	wcl;

	/*** cleate a window ****************************************************/

	g_hInst = hInst;

	/* define a window class */

	wcl.hInstance = hInst;
	wcl.lpszClassName = szWinClassName;
	wcl.lpfnWndProc = WindowProc;
	wcl.style = 0;

	wcl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcl.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcl.lpszMenuName = NULL;

	wcl.cbClsExtra = 0;
	wcl.cbWndExtra = 0;

	wcl.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);

	/* register the window class */

	if (!RegisterClass(&wcl)) return(1);

	g_hWnd = hWnd = CreateWindow(
		szWinClassName,
		WINDOW_TITLE,			// window title
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,			// x
		CW_USEDEFAULT,			// y
		CW_USEDEFAULT,			// w
		CW_USEDEFAULT,			// h
		HWND_DESKTOP,
		NULL,
		hInst,
		NULL
	);

	/* display the window */

	//ShowWindow(hWnd, iCmdShow);
	//UpdateWindow(hWnd);

	// create process

	CPT_ARG_t Arg = {
		g_hWnd,
		iCmdShow
	};
	
	std::thread th(CreateProcessThread, &Arg);

	/* create the message loop */

	while (GetMessage(&Msg, NULL, 0, 0)) {
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}

	th.join();

	return 0;
}
