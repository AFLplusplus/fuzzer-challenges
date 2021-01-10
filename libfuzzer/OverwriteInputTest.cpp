#include <unistd.h>
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

// Simple test for a fuzzer. Make sure we abort if Data is overwritten.
#include <cstdint>
#include <iostream>

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
  if (Size)
    *const_cast<uint8_t*>(Data) = 1;
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
