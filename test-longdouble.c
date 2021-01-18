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
  long double *p80, lesser, greater;
  uint32_t     i;

  if (len < 50) bail("too short", 0);

  for (i = 0; i < 3; i++) {
    p80 = (long double *)(buf + i * 10);
    lesser = 1000000.01 + 10011.0 * i + i;
    greater = 1000010.99 + 10011.0 * i - i;
    if (*p80 < lesser || *p80 > greater) bail("wrong long double", (i * 10));
    if (!isnormal(*p80)) bail("not normal", (i * 10));
  }

  // no fuzzer can solve long double arithmetic, so we leave the harder
  // testcases out

  // exact
  p80 = (long double *)(buf + 30);
  if (*p80 != 3.141592653589793116) bail("wrong long double", 30);
  if (!isnormal(*p80)) bail("not normal", 30);

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
