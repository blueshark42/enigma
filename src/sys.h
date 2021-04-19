#ifndef ENIGMA_SRC_SYS_H_
#define ENIGMA_SRC_SYS_H_

#include <string>

#include <Windows.h>
#include <sddl.h>

namespace Sys {
int AddToRegistry();
int RemoveFromRegistry();
bool CheckForVirtualMachine();
int CreateShell(LPSTR command);
BOOL IsProcessElevated();
}

#endif //ENIGMA_SRC_SYS_H_
