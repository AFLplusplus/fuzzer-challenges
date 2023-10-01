TARGETS = test-crc32 test-u8 test-u16 test-u32 test-u64 test-float test-double test-longdouble test-u128 test-u32-cmp test-memcmp test-strcmp test-transform test-extint test-transform-t1 test-transform-t2 test-transform-t3 test-transform-t4 test-transform-t5 test-transform-t6 test-transform-t7 test-transform-t8

all:
	@echo Use test.sh to perform the test.
	@echo To compile with CC and CFLAGS yourself do \"make compile\", however test.sh will overwrite these.

compile:	$(TARGETS)

%:	%.c
	-$(CC) $(CFLAGS) -o $@ $@.c -lm

clean:
	rm -f $(TARGETS) core* *.plain *~ *.data *.log HONGGFUZZ.REPORT.TXT SIG* afl++.dic
	rm -rf out-* in *.dir mcore_*
