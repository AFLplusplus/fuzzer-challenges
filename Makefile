TARGETS = test-crc32 test-u8 test-u16 test-u32 test-u64 test-float test-double test-longdouble test-u128 test-extint test-u32-cmp test-memcmp test-strcmp test-transform

all:
	@echo Use test.sh to perform the test.
	@echo To compile with CC and CFLAGS yourself do \"make compile\", however test.sh will overwrite these.

compile:	$(TARGETS)

%:	%.c
	$(CC) $(CFLAGS) -o $@ $@.c -lm

clean:
	rm -f $(TARGETS) core* *~ *.log HONGGFUZZ.REPORT.TXT SIG* afl++.dic
	rm -rf out-* in
