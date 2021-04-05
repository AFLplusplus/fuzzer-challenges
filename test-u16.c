#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define bail(msg, pos)                                         \
  while (1) {                                                  \
                                                               \
    fprintf(stderr, "%s at %u\n", (char *)msg, (uint32_t)pos); \
    return 0;                                                  \
                                                               \
  }

int LLVMFuzzerTestOneInput(uint8_t *buf, size_t len) {

  uint16_t *p16;

  if (len < 16) bail("too short", 0);

  p16 = (uint16_t *)(buf);
  if (*p16 != 0x1122) bail("wrong u16", 0);

  p16 = (uint16_t *)(buf + 2);
  if (*p16 != 0x3344) bail("wrong u16", 2);

  p16 = (uint16_t *)(buf + 4);
  if (*p16 != 0x5566) bail("wrong u16", 4);

  p16 = (uint16_t *)(buf + 6);
  if (*p16 != 0x7788) bail("wrong u16", 6);

  p16 = (uint16_t *)(buf + 8);
  if (*p16 != 0xa0a1) bail("wrong u16", 8);

  p16 = (uint16_t *)(buf + 10);
  if (*p16 != 0xa2a3) bail("wrong u16", 10);

  p16 = (uint16_t *)(buf + 12);
  if (*p16 != 0x1234) bail("wrong u16", 12);

  p16 = (uint16_t *)(buf + 14);
  if (*p16 != 0xaabb) bail("wrong u16", 14);

  abort();

  return 0;

}

#ifdef __AFL_COMPILER
int main(int argc, char **argv) {

  unsigned char buf[16];
  ssize_t       len;
  int           fd = 0;
  if (argc > 1) fd = open(argv[1], O_RDONLY);

  if ((len = read(fd, buf, sizeof(buf))) <= 0) exit(0);

  LLVMFuzzerTestOneInput(buf, len);
  exit(0);

}

#endif

