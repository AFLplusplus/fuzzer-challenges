#include <unistd.h>
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

// The fuzzer must find several constants with swapped bytes.
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
  if (Size < 14) return 0;
  uint64_t x = 0;
  uint32_t y = 0;
  uint32_t z = 0;
  memcpy(&x, Data, sizeof(x));
  memcpy(&y, Data + Size / 2, sizeof(y));
  memcpy(&z, Data + Size - sizeof(z), sizeof(z));

  x = __builtin_bswap64(x);
  y = __builtin_bswap32(y);
  z = __builtin_bswap32(z);
  const bool k32bit = sizeof(void*) == 4;

  if ((k32bit || x == 0x46555A5A5A5A5546ULL) &&
      z == 0x4F4B &&
      y == 0x66757A7A &&
      true
      ) {
    if (Data[Size - 5] == 'z') {
      fprintf(stderr, "BINGO; Found the target\n");
      abort();
    }
  }
  return 0;
}

#ifdef __AFL_COMPILER
int main() {
  unsigned char buf[64];
  ssize_t len;

  if ((len = read(0, buf, sizeof(buf))) <= 0)
    return -1;

  LLVMFuzzerTestOneInput(buf, len);
  return 0;

}
#endif
