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

typedef unsigned __int128 uint128_t;

int LLVMFuzzerTestOneInput(uint8_t *buf, size_t len) {

  uint128_t *p128, v128, i;
  uint8_t *  p = (uint8_t *)&v128;

  if (len < 48) bail("too short", 48);

  for (i = 0; i < 2; i++) {

    if (len < 16 + i * 16) bail("too short", i * 16);
    p128 = (uint128_t *)(buf + i * 16);
    memset((char *)&v128, 'A' + i * 5, sizeof(v128));
    if (*p128 != v128) bail("wrong u128", (i * 16));

  }

  for (i = 0; i < sizeof(v128); i++)
    p[i] = '0' + i * 2;
  p128 = (uint128_t *)(buf + 32);
  if (*p128 != v128) bail("wrong u128", 32);

  abort();

  return 0;

}

#ifdef __AFL_COMPILER
int main(int argc, char **argv) {

  unsigned char buf[128];
  ssize_t       len;
  int           fd = 0;
  if (argc > 1) fd = open(argv[1], O_RDONLY);

  if ((len = read(fd, buf, sizeof(buf))) <= 0) exit(0);

  LLVMFuzzerTestOneInput(buf, len);
  exit(0);

}

#endif

