#ifndef ENIGMA_SRC_ENCRYPT_H_
#define ENIGMA_SRC_ENCRYPT_H_

#include <string>
#include <random>
#include <vector>

#include "stream.h"

namespace Crypt {
static std::string key;

int GenerateInt(int min, int max);
std::string GenerateKey(u_short keyLen);
}

#endif //ENIGMA_SRC_ENCRYPT_H_
