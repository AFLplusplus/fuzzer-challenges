#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <math.h>

#define bail(msg, pos)                                         \
  while (1) {                                                  \
                                                               \
    fprintf(stderr, "%s at %u\n", (char *)msg, (uint32_t)pos); \
    return 0;                                                  \
                                                               \
  }

int LLVMFuzzerTestOneInput(uint8_t *buf, size_t len) {

  long double *p80, lesser, greater;

  if (len < 40) bail("too short", 0);

  p80 = (long double *)(buf);
  lesser = 1000000.01;
  greater = 1000010.99;
  if (*p80 < lesser || *p80 > greater) bail("wrong long double", 0);
  if (!isnormal(*p80)) bail("not normal", 0);

  p80 = (long double *)(buf + 10);
  lesser = 101.9;
  greater = 109.0;
  if (*p80 < lesser || *p80 > greater) bail("wrong long double", 10);
  if (!isnormal(*p80)) bail("not normal", 10);

  p80 = (long double *)(buf + 20);
  lesser = 22222221.9;
  greater = 22222225.1;
  if (*p80 < lesser || *p80 > greater) bail("wrong long double", 20);
  if (!isnormal(*p80)) bail("not normal", 20);

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
int main(int argc, char **argv) {

  unsigned char buf[64];
  ssize_t       len;
  int           fd = 0;
  if (argc > 1) fd = open(argv[1], O_RDONLY);

  if ((len = read(fd, buf, sizeof(buf))) <= 0) exit(0);

  LLVMFuzzerTestOneInput(buf, len);
  exit(0);

}

#endif

