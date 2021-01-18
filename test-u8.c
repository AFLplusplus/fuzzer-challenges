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

  if (len < 8) bail("too short", 0);

  if (buf[0] != 'A')  bail("wrong char", 0);
  if (buf[1] != 'C')  bail("wrong char", 1);
  if (buf[2] != 'E')  bail("wrong char", 2);
  if (buf[3] != 'G')  bail("wrong char", 3);
  if (buf[4] != 'I')  bail("wrong char", 4);
  if (buf[5] != 'K')  bail("wrong char", 5);
  if (buf[6] != 'M')  bail("wrong char", 6);
  if (buf[7] != 'Z')  bail("wrong char", 7);

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
