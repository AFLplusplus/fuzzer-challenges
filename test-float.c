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
  float *  p32, lesser, greater;
  uint32_t i;

  if (len < 20) bail("too short", 0);

  // easy
  for (i = 0; i < 2; i++) {
    p32 = (float *)(buf + i * 4);
    lesser = 10002.1 + 1000.0 * i + i;
    greater = 10008.9 + 1000.0 * i - i;
    if (*p32 < lesser || *p32 > greater) bail("wrong float", (i * 4));
    if (!isnormal(*p32)) bail("not normal float", (i * 4));
  }

  // harder
  for (i = 2; i < 4; i++) {
    p32 = (float *)(buf + i * 4);
    lesser = 100000.25 + 2222.0 * i + i;
    greater = 100015.75 + 2222.0 * i - i;
    if (*p32 <= lesser || *p32 >= greater) bail("wrong float", (i * 4));
    if (!isnormal(*p32)) bail("not normal float", (i * 4));
  }

  // exact
  p32 = (float *)(buf + 16);
  if (*p32 != 31337.3125) bail("wrong float", 16);
  if (!isnormal(*p32)) bail("not normal float", 16);

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
