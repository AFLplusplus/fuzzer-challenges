#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int LLVMFuzzerTestOneInput(uint8_t *buf, size_t len) {
  const size_t SORTED_LEN = 64;

  if (len < SORTED_LEN) {
    fprintf(stderr, "wrong size\n");
    return 0;
  }

  for (size_t i = 1; i < SORTED_LEN; i++) {
    if (buf[i] <= buf[i - 1]) {
        fprintf(stderr, "item %u isn't greater than previous item\n", (uint32_t)i);
        return 0;
    }
  }

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

