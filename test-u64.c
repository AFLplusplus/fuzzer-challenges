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

  uint64_t *p64;

  if (len < 32) bail("too short", 0);

  p64 = (uint64_t *)(buf);
  if (*p64 != 0x1122334455667788) bail("wrong u64", 0);

  p64 = (uint64_t *)(buf + 8);
  if (*p64 != 0xa0a1a2a3a4a5a6a7) bail("wrong u64", 8);

  p64 = (uint64_t *)(buf + 16);
  if (*p64 != 0x1234aabbccddeeff) bail("wrong u64", 16);

  p64 = (uint64_t *)(buf + 24);
  if (*p64 != 0x0f1f2f3f4f5f6f7f) bail("wrong u64", 24);

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

