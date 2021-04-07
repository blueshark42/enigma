#include "encrypt.h"

//void Crypt::Encrypt(std::string &input) {
//  std::ostringstream ostream;
//  ostream << key;
//  Stream::WriteLog(ostream.str(), 0x0, false);
//}

int Crypt::GenerateInt(int min, int max) {
  static thread_local std::mt19937_64 mt(std::random_device{}());
  std::uniform_int_distribution<int> dist(min, max);

  return dist(mt);
}

std::string Crypt::GenerateKey(u_short keyLen) {
  std::string genKey;

  for (int i = 0; i < keyLen; i++) {
	genKey += GenerateInt(33, 126);
  }
  return genKey;
}
