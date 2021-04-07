#ifndef ENIGMA_SRC_SCREEN_H_
#define ENIGMA_SRC_SCREEN_H_

#include <string>
#include <sstream>

#include <Windows.h>
#include <gdiplus.h>

#pragma comment(lib, "gdiplus.lib")

namespace Screen {
static int picId = 0;
int GetEncoderClsId(const WCHAR *format, CLSID *clsId);
void CaptureScreen(const std::string &path,
				   const std::string &name,
				   bool loopForever = false,
				   int delay = 0,
				   int loopAmount = 0);
}

#endif //ENIGMA_SRC_SCREEN_H_
