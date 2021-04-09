#ifndef ENIGMA_SRC_DEBUG_H_
#define ENIGMA_SRC_DEBUG_H_

//#define DEBUG_BUILD
#define DEBUG_FUNCT

#ifdef DEBUG_FUNCT
#define debn(...) std::cout << "[" << #__VA_ARGS__ << " - " << __VA_ARGS__ << "]" << std::endl;
#define deb(...) std::cout << __VA_ARGS__ << std::endl;
#define deba(x) for(auto &a : x) { deb(a) }

#else
#define debn(x)
#define deb(x)
#define deba(x)

#endif
#endif