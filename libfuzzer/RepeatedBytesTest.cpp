#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

// Simple test for a fuzzer. The fuzzer must find repeated bytes.
#include <assert.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <ostream>

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {

  assert(Data);
  // Looking for AAAAAAAAAAAAAAAAAAAAAA or some such.
  size_t CurA = 0, MaxA = 0;
  for (size_t i = 0; i < Size; i++) {

    // Make sure there are no conditionals in the loop so that
    // coverage can't help the fuzzer.
    int EQ = Data[i] == 'A';
    CurA = EQ * (CurA + 1);
    int GT = CurA > MaxA;
    MaxA = GT * CurA + (!GT) * MaxA;

  }

  if (MaxA >= 20) {

    std::cout << "BINGO; Found the target (Max: " << MaxA << "), exiting\n"
              << std::flush;
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

