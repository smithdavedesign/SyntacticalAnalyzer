# Scheme compiler front end: lexical + syntactical analyzer (C++)

The front end of a compiler for a subset of Scheme, written for CS 460 (compiler construction), 2016–2017:

1. **Lexical analyzer** (`LexicalAnalyzer.cpp`): tokenizes a `.ss` source file into 32 token types (identifiers, numeric literals, keywords such as `define` / `if` / `cons`, predicates such as `null?`, list operations `c[ad]+r`, and operators).
2. **Syntactical analyzer** (`SyntacticalAnalyzer.cpp`): a recursive-descent parser for the LL(1) grammar in [`grammar.txt`](grammar.txt), one function per nonterminal, with error recovery. It reports every rule it applies.

A later phase translated the checked Scheme into C++. This repo is the analysis half.

## Build and run

Any C++17 compiler and `make`:

```sh
make
./p2 tests/test.ss      # writes test.lst, test.p1, test.p2 and test.dbg next to the input
make test               # runs the 5 course test programs and checks every output byte for byte
```

| Output | Contents |
|---|---|
| `.lst` | listing: numbered source lines, errors where they occur, error count |
| `.p1` | the token stream: token type and lexeme |
| `.p2` | the parse: each nonterminal entered and left, and the grammar rule used |
| `.dbg` | everything interleaved, for debugging |

```
Starting <program>. Current token = LPAREN_T
Using rule 1
Starting <define>. Current token = LPAREN_T
Using rule 2
```

An invalid character is reported where it is: `Error at 113,8: Invalid character found: %`. The parse carries on.

## Tests

`tests/` holds 5 programs from the course. `tests/expected/` holds the outputs the 2016 build produced for them. `make test` reruns each one and compares all four outputs. Two of the programs were written by classmates as shared test files (see their headers).

## History

- **2016–17:** the course supplied the lexical analyzer to the syntactical-analyzer phase as a prebuilt object file. Only that object (`LexicalAnalyzer.save`) was ever committed.
- **2026:** that object is an x86-64 Linux build using the pre-C++11 `std::string` ABI, so it can't link on macOS or a current toolchain. The lexer was rewritten from `LexicalAnalyzer.h` and checked against the recorded outputs. All 20 files (5 programs × `.lst`, `.p1`, `.p2` and `.dbg`) match byte for byte, including the invalid-character case. The resource limits (`SetLimits`) came over from the archived [Lexical-Anylazer-compiler-construction-](https://github.com/smithdavedesign/Lexical-Anylazer-compiler-construction-) repo along with the test programs. That repo is now folded into this one.

More C++ from the same years: [data-structures-and-algorithms](https://github.com/smithdavedesign/data-structures-and-algorithms) and [cpp-coursework](https://github.com/smithdavedesign/cpp-coursework).
