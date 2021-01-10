TARGETS = AbsNegAndConstant64Test AbsNegAndConstantTest BufferOverflowOnInput CallerCalleeTest CleanseTest CounterTest CxxStringEqTest DeepRecursionTest FourIndependentBranchesTest FullCoverageSetTest InitializeTest KeepSeedTest Labels20Test Memcmp64BytesTest MemcmpTest MultipleConstraintsOnSmallInputTest NthRunCrashTest OnlySomeBytesTest OverwriteInputTest RepeatedBytesTest RepeatedMemcmp SimpleCmpTest SimpleDictionaryTest SimpleHashTest SimpleTest SimpleTestStdio SingleByteInputTest SingleMemcmpTest SingleStrcmpTest SingleStrncmpTest StrcmpTest StrncmpTest StrstrTest SwapCmpTest Switch2Test Switch3Test SwitchTest TableLookupTest ThreeBytes ThreeFunctionsTest

all:
	@echo Use test.sh to perform the test.
	@echo To compile with CC and CFLAGS yourself do \"make compile\", however test.sh will overwrite these.

compile:	$(TARGETS)

%:	%.c
	$(CXX) $(CXXFLAGS) -o $@ $@.c

clean:
	rm -f $(TARGETS) core* *~ *.log
	rm -rf out-* in
