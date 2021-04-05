#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

// Simple test for a fuzzer. The fuzzer must find the string "FUZZ".
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <iostream>

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {

  int bits = 0;
  if (Size > 0 && Data[0] == 'F') bits |= 1;
  if (Size > 1 && Data[1] == 'U') bits |= 2;
  if (Size > 2 && Data[2] == 'Z') bits |= 4;
  if (Size > 3 && Data[3] == 'Z') bits |= 8;
  if (bits == 15) {

    std::cerr << "BINGO!\n";
    abort();

  }

  return 0;

}

#ifdef __AFL_COMPILER
int main(int argc, char **argv) {

  unsigned char buf[64];
  ssize_t       len;
  int           fd = 0;
  if (argc > 1) fd = open(argv[1], O_RDONLY);

  if ((len = read(0, buf, sizeof(buf))) <= 0) return -1;

  LLVMFuzzerTestOneInput(buf, len);
  return 0;

}

#endif

