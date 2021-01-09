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
  uint8_t * p8;
  uint16_t *p16, *p16_2;
  uint32_t *p32, *p32_2, i;

  if (len < 32) bail("too short", 0);

  for (i = 0; i < 8; i++) {
    p32 = (uint32_t *)(buf + i * 4);
    if (*p32 != (0x10000 * i) + 'A' + i) bail("wrong u32", (i * 4));
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
