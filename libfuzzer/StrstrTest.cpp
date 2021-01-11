#include <unistd.h>
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

// Test strstr and strcasestr hooks.
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <string.h>
#include <string>

// Windows does not have strcasestr and memmem, so we are not testing them.
#ifdef _WIN32
  #define strcasestr strstr
  #define memmem(a, b, c, d) true
#endif

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
  if (Size < 4) return 0;
  std::string s(reinterpret_cast<const char *>(Data), Size);
  if (strstr(s.c_str(), "FUZZ") && strcasestr(s.c_str(), "aBcD") &&
      memmem(s.data(), s.size(), "kuku", 4)) {
    fprintf(stderr, "BINGO\n");
    abort();
  }
  return 0;
}

#ifdef __AFL_COMPILER
int main() {
  unsigned char buf[64];
  ssize_t       len;

  if ((len = read(0, buf, sizeof(buf))) <= 0) return -1;

  LLVMFuzzerTestOneInput(buf, len);
  return 0;
}
#endif
