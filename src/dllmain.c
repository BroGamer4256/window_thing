#include "helpers.h"

HWND DIVAMIXP = 0;

int CALLBACK
enumWindows (HWND handle, LPARAM param) {
	char buf[64];
	GetClassName (handle, buf, 64);
	if (!strcmp (buf, "DIVAMIXP")) {
		DIVAMIXP = handle;
		return 0;
	}
	return 1;
}

HOOK_DYNAMIC (HWND, __stdcall, ForegroundWindow) {
	if (!DIVAMIXP) {
		EnumWindows (enumWindows, 0);
	}
	return DIVAMIXP;
}

HOOK_DYNAMIC (i16, __stdcall, AsyncKeyState, i32 key) {
	if (originalForegroundWindow () == DIVAMIXP) {
		return originalAsyncKeyState (key);
	}
	return 0;
}

#ifdef __cplusplus
extern "C" {
#endif
__declspec(dllexport) void Init () {
	INSTALL_HOOK_DYNAMIC (ForegroundWindow,
						  PROC_ADDRESS ("user32.dll", "GetForegroundWindow"));
	INSTALL_HOOK_DYNAMIC (AsyncKeyState,
						  PROC_ADDRESS ("user32.dll", "GetAsyncKeyState"));
}
#ifdef __cplusplus
}
#endif