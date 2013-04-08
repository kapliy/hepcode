//  emulate CERNLIB vzero
#include <cstring>
extern "C" void vzero_(float* x, int* n) {
  memset(x,0,(*n)*sizeof(float));
}
