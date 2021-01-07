#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <stdint.h>

#define bail(msg, pos)                 \
  while (1) {                     \
    fprintf(stderr, "%s at %u\n", (char*)msg, (uint32_t)pos); \
    return 0;                     \
  }

int LLVMFuzzerTestOneInput(uint8_t *buf, size_t len) {
  uint8_t  *p8;

  if (len < 32) bail("too short", 0);
  if (memcmp((char*)buf, "0123", 4)) bail("wrong string", 0);
  if (memcmp((char*)buf + 4, "87654321", 8)) bail("wrong string", 4);
  if (memcmp((char*)buf  + 12, "ABCDEFHIKLMNOPQR", 16)) bail("wrong string", 12);
  if (len < 56) bail("too short", 0);
  if (memcmp((char*)buf + 28, "ZYXWVUTSRQPONMLKJIHGFEDCBA", 26)) bail("wrong string", 28);
  
  abort();
  
  return 0;
}

#ifdef __AFL_COMPILER
int main() {
  unsigned char buf[64];
  ssize_t len;
  
  if ((len = read(0, buf, sizeof(buf))) <= 0) exit(0);
    
  LLVMFuzzerTestOneInput(buf, len);
  exit(0);

}
#endif
