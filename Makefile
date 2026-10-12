# make        build ./p2 (lexical + syntactical analyzer)
# make test   run the course test programs and compare every output with tests/expected/
# make clean  remove build and test output
CXX ?= c++
CXXFLAGS ?= -std=c++17 -O2 -Wall
SRCS := Project2.cpp SetLimits.cpp LexicalAnalyzer.cpp SyntacticalAnalyzer.cpp
TESTS := test test2 mytest TeamAP2 TeamHJP2test

p2: $(SRCS) $(wildcard *.h)
	$(CXX) $(CXXFLAGS) -o $@ $(SRCS)

test: p2
	@rm -rf build/test && mkdir -p build/test && cp tests/*.ss build/test/
	@cd build/test && for t in $(TESTS); do \
	  ../../p2 $$t.ss > /dev/null || exit 1; \
	  for e in lst p1 dbg p2; do \
	    cmp -s $$t.$$e ../../tests/expected/$$t.$$e || { echo "FAIL $$t.$$e"; diff $$t.$$e ../../tests/expected/$$t.$$e | head -20; exit 1; }; \
	  done; echo "ok   $$t (lst, p1, dbg, p2 match)"; \
	done

clean:
	rm -rf p2 build

.PHONY: test clean
