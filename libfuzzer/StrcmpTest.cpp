#include <unistd.h>
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

// Break through a series of strcmp.
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

bool Eq(const uint8_t *Data, size_t Size, const char *Str) {
  char Buff[1024];
  size_t Len = strlen(Str);
  if (Size < Len) return false;
  if (Len >= sizeof(Buff)) return false;
  memcpy(Buff, (const char*)Data, Len);
  Buff[Len] = 0;
  int res = strcmp(Buff, Str);
  return res == 0;
}

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
  if (Eq(Data, Size, "ABC") &&
      Size >= 3 && Eq(Data + 3, Size - 3, "QWER") &&
      Size >= 7 && Eq(Data + 7, Size - 7, "ZXCVN")) {
    fprintf(stderr, "BINGO\n");
    abort();
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
