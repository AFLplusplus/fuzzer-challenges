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

  double *p64, lesser, greater;

  if (len < 40) bail("too short", 0);

  p64 = (double *)(buf);
  lesser = 1000000.01;
  greater = 1000010.99;
  if (*p64 < lesser || *p64 > greater) bail("wrong double", 0);
  if (!isnormal(*p64)) bail("not normal", 0);

  p64 = (double *)(buf + 8);
  lesser = 101.9;
  greater = 109.0;
  if (*p64 < lesser || *p64 > greater) bail("wrong double", 8);
  if (!isnormal(*p64)) bail("not normal", 8);

  p64 = (double *)(buf + 16);
  lesser = 22222221.9;
  greater = 22222225.1;
  if (*p64 < lesser || *p64 > greater) bail("wrong double", 16);
  if (!isnormal(*p64)) bail("not normal", 16);

  // no fuzzer can solve double arithmetic, so we leave the harder
  // testcases out

  // exact
  p64 = (double *)(buf + 24);
  if (*p64 != 3.141592653589793116) bail("wrong double", 24);
  if (!isnormal(*p64)) bail("not normal", 24);

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

