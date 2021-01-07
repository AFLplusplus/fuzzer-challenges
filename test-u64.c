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
  uint64_t *p64, v64, i;

  for (i = 0; i < 4; i++) {
    if (len < 8 + i*8) bail("too short", i*8);
    p64 = (uint64_t*)(buf + i*8);
    memset((char*)&v64, 'A' + i, sizeof(v64));
    if (*p64 != v64) bail("wrong u64", (i * 8));
  }

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
