TARGETS = test-crc32 test-u8 test-u16 test-u32 test-u64 test-float test-double test-u128 test-u32-cmp test-memcmp test-strcmp

all:
	@echo Use test.sh to perform the test.
	@echo To compile with CC and CFLAGS yourself do \"make compile\", however test.sh will overwrite these.

compile:	$(TARGETS)

%:	%.c
	$(CC) $(CFLAGS) -o $@ $@.c

clean:
	rm -f $(TARGETS) core* *~ *.log
	rm -rf out-* in
