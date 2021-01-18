#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#define bail(msg, pos)                                         \
  while (1) {                                                  \
    fprintf(stderr, "%s at %u\n", (char *)msg, (uint32_t)pos); \
    return 0;                                                  \
  }

int LLVMFuzzerTestOneInput(uint8_t *buf, size_t len) {
  float *p32, lesser, greater;

  if (len < 16) bail("too short", 0);

  p32 = (float *)(buf);
  lesser = 1000000.01;
  greater = 1000010.99;
  if (*p32 < lesser || *p32 > greater) bail("wrong float", 0);
  if (!isnormal(*p32)) bail("not normal", 0);

  p32 = (float *)(buf + 4);
  lesser = 101.9;
  greater = 109.0;
  if (*p32 < lesser || *p32 > greater) bail("wrong float", 4);
  if (!isnormal(*p32)) bail("not normal", 4);

  p32 = (float *)(buf + 8);
  lesser = 22222221.9;
  greater = 22222225.1;
  if (*p32 < lesser || *p32 > greater) bail("wrong float", 8);
  if (!isnormal(*p32)) bail("not normal", 8);

  abort();

  return 0;
}

#ifdef __AFL_COMPILER
int main() {
  unsigned char buf[64];
  ssize_t       len;

  if ((len = read(0, buf, sizeof(buf))) <= 0) exit(0);

  LLVMFuzzerTestOneInput(buf, len);
  exit(0);
}
#endif
