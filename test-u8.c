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
  if (len < 12) bail("too short", 0);

  if (buf[0] != 'A') bail("wrong char", 0);
  if (buf[1] != 'C') bail("wrong char", 1);
  if (buf[2] != 'E') bail("wrong char", 2);
  if (buf[3] != 'G') bail("wrong char", 3);
  if (buf[4] != 'I') bail("wrong char", 4);
  if (buf[5] != 'K') bail("wrong char", 5);
  if (buf[6] != 'M') bail("wrong char", 6);
  if (buf[7] != 'Z') bail("wrong char", 7);
  if (buf[8] != 'Y') bail("wrong char", 8);
  if (buf[9] != 0) bail("wrong char", 9);
  if (buf[10] != 1) bail("wrong char", 10);
  if (buf[11] != 128) bail("wrong char", 11);

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
