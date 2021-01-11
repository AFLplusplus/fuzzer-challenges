#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <stdint.h>

#define bail(msg, pos)                                         \
  while (1) {                                                  \
    fprintf(stderr, "%s at %u\n", (char *)msg, (uint32_t)pos); \
    return 0;                                                  \
  }

typedef _ExtInt(24) uint24_t;
typedef _ExtInt(40) uint40_t;
typedef _ExtInt(56) uint56_t;

int LLVMFuzzerTestOneInput(uint8_t *buf, size_t len) {
  uint24_t *p24, v24;
  uint40_t *p40, v40;
  uint56_t *p56, v56;
  int       i = 0;

  if (len < 16) bail("too short", 0);

  p24 = (uint24_t *)buf;
  v24 = (uint24_t)0x123456;
  if (*p24 != v24) bail("wrong uint24_t", 0);

  p40 = (uint40_t *)(buf + 3);
  v40 = (uint40_t)0x1234554321;
  if (*p40 != v40) bail("wrong uint40_t", 8);

  p56 = (uint56_t *)(buf + 8);
  v56 = (uint56_t)0x77665544332211;
  if (*p56 != v56) bail("wrong uint56_t", 16);

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
