TARGETS = test-crc32 test-u16 test-u32 test-u64 test-u8 test-float test-double test-u128 test-u32-cmp test-memcmp test-strcmp

all:	$(TARGETS)

%:	%.c
	$(CC) $(CFLAGS) -o $@ $@.c

clean:
	rm -f $(TARGETS) core* *~ *.log
	rm -rf out-* in
