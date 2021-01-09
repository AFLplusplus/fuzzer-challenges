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
  uint32_t *p32, lesser, greater, i;

  if (len < 24) bail("too short", 0);

  for (i = 0; i < 6; i++) {
    p32 = (uint32_t *)(buf + i * 4);
    lesser = 1000 + 100 * (i + 1);
    greater = 1000 + 102 * (i + 1);
    if (*p32 <= lesser || *p32 >= greater) bail("wrong u32", (i * 4));
  }

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
