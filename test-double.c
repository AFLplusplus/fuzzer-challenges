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
  double *p64, lesser, greater;
  uint64_t i;

  if (len < 32) bail("too short", 0);
  
  // easy
  for (i = 0; i < 4; i++) {
    p64 = (double*)(buf + i*8);
    lesser = 1000000.0 + 10011.0*i;
    greater = 1000010.0 + 10011.0*i;
    if (*p64 < lesser || *p64 > greater) bail("wrong double", (i * 8));
  }

  if (len < 64) bail("too short", 0);

  // hard
  for (i = 4; i < 8; i++) {
    p64 = (double*)(buf + i*8);
    lesser = 300001.0 + 31050.0*i;
    greater = 300005.0 + 31050.0*i;
    if (*p64 <= lesser || *p64 >= greater) bail("wrong double", (i * 8));
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
