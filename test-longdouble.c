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

int LLVMFuzzerTestOneInput(uint8_t *buf, size_t len) {
  long double *p80, lesser, greater;
  uint64_t     i;

  if (len < 50) bail("too short", 0);

  // easy
  for (i = 0; i < 2; i++) {
    p80 = (long double *)(buf + i * 10);
    lesser = 1000000.1 + 10011.0 * i + i;
    greater = 1000009.9 + 10011.0 * i - i;
    if (*p80 < lesser || *p80 > greater) bail("wrong long double", (i * 10));
  }

  // harder
  for (i = 2; i < 4; i++) {
    p80 = (long double *)(buf + i * 10);
    lesser = 300000.1 + 31050.0 * i + i;
    greater = 300009.9 + 31050.0 * i - i;
    if (*p80 <= lesser || *p80 >= greater) bail("wrong long double", (i * 10));
  }

  // exact
  p80 = (long double *)(buf + 40);
  if (*p80 != 3.141592653589793116) bail("wrong long double", 40);

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
