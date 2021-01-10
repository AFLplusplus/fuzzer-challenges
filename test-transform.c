#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>

#define bail(msg, pos)                                         \
  while (1) {                                                  \
    fprintf(stderr, "%s at %u\n", (char *)msg, (uint32_t)pos); \
    return 0;                                                  \
  }

int LLVMFuzzerTestOneInput(uint8_t *buf, size_t len) {
  uint8_t *p8;
  int      i;
  uint8_t  buff[32];

  if (len < 31) bail("too short", 0);

  // libfuzzer workaround
  memcpy(buff, buf, 31);
  buff[31] = 0;

  // string to int
  if (atoi((char *)buff) != 66766) bail("wrong string", 0);

  // red hering
  if (strncasecmp((char *)buff + 6, "ABCD", 4)) bail("wrong string", 6);
  for (i = 0; i < 10; i++)
    buff[i + 6] = tolower(buff[i + 6]);
  if (strncmp((char *)buff + 6, "abcdefghik", 10)) bail("wrong string", 6);

  // cesar cipher
  for (i = 0; i < 4; i++)
    buff[i + 20] = buff[i + 20] + 4;
  if (memcmp(buff + 20, "EFGH", 4)) bail("wrong string", 20);

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
