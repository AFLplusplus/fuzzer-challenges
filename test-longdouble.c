#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <math.h>

#define bail(msg)                 \
  while (1) {                     \
    fprintf(stderr, "%s\n", msg); \
    return 0;                     \
  }

int LLVMFuzzerTestOneInput(uint8_t *buf, size_t len) {
  long double lesser, greater;
  long double *vals = (long double *)buf;

  if (len < sizeof(long double[4])) bail("too short");

  lesser = 1000000.01;
  greater = 1000010.99;
  if (vals[0] < lesser || vals[0] > greater) bail("wrong long double (vals[0])");
  if (!isnormal(vals[0])) bail("not normal (vals[0])");

  lesser = 101.9;
  greater = 109.0;
  if (vals[1] < lesser || vals[1] > greater) bail("wrong long double (vals[1])");
  if (!isnormal(vals[1])) bail("not normal (vals[1])");

  lesser = 22222221.9;
  greater = 22222225.1;
  if (vals[2] < lesser || vals[2] > greater) bail("wrong long double (vals[2])");
  if (!isnormal(vals[2])) bail("not normal (vals[2])");

  // no fuzzer can solve long double arithmetic, so we leave the harder
  // testcases out

  // exact
  if (vals[3] != 3.141592653589793116) bail("wrong long double (vals[3])");
  if (!isnormal(vals[3])) bail("not normal (vals[3])");

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

