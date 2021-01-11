#include <unistd.h>
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

// Test for a fuzzer: must find the case where a particular basic block is
// executed many times.
#include <iostream>

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
  int Num = 0;
  for (size_t i = 0; i < Size; i++)
    if (Data[i] == 'A' + i) Num++;
  if (Num >= 4) {
    std::cerr << "BINGO!\n";
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
