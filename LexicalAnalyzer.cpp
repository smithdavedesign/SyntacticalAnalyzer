/********************************************************************************/
/* Project: Compiler construction, phase 1: lexical analyzer                    */
/* Author: David Smith                                                          */
/* Description: Tokenizes a Scheme subset (.ss) for the syntactical analyzer.   */
/*                                                                              */
/* Rewritten in 2026 from LexicalAnalyzer.h. The repo only had the 2016 build   */
/* (LexicalAnalyzer.save, an x86-64 Linux object using the pre-C++11 string     */
/* ABI), which can't link on macOS or a current toolchain. This version         */
/* reproduces the recorded listing (.lst), token (.p1) and debug (.dbg) output  */
/* of the course test programs byte for byte: see tests/ and `make test`.       */
/********************************************************************************/
#include <cctype>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <string>
#include "LexicalAnalyzer.h"

using namespace std;

string token_names[] = {
	"LAMBDA", "IDENT_T", "NUMLIT_T", "CONS_T", "IF_T", "DISPLAY_T", "NEWLINE_T",
	"LISTOP_T", "AND_T", "OR_T", "NOT_T", "DEFINE_T", "NUMBERP_T", "SYMBOLP_T", "LISTP_T",
	"ZEROP_T", "NULLP_T", "CHARP_T", "STRINGP_T", "PLUS_T", "MINUS_T", "DIV_T", "MULT_T",
	"EQUALTO_T", "GT_T", "LT_T", "GTE_T", "LTE_T", "LPAREN_T", "RPAREN_T", "QUOTE_T", "ERROR_T",
	"EOF_T"
};

// Reserved words. Anything else made of letters, digits and '_' (with an optional
// trailing '?') is an identifier, except c[ad]+r, which is a list operation.
static const struct { const char * word; token_type token; } KEYWORDS[] = {
	{"cons", CONS_T}, {"if", IF_T}, {"display", DISPLAY_T}, {"newline", NEWLINE_T},
	{"and", AND_T}, {"or", OR_T}, {"not", NOT_T}, {"define", DEFINE_T},
	{"number?", NUMBERP_T}, {"symbol?", SYMBOLP_T}, {"list?", LISTP_T}, {"zero?", ZEROP_T},
	{"null?", NULLP_T}, {"char?", CHARP_T}, {"string?", STRINGP_T},
};

static bool IsListOp (const string & word)
{ // car, cdr, cadr, cddr, caddr, ...: 'c', then one or more 'a' or 'd', then 'r'
	if (word.size () < 3 || word[0] != 'c' || word[word.size () - 1] != 'r')
		return false;
	for (size_t i = 1; i + 1 < word.size (); i++)
		if (word[i] != 'a' && word[i] != 'd')
			return false;
	return true;
}

static string BaseName (const string & filename)
{ // test.ss -> test, so the outputs are test.lst, test.p1 and test.dbg
	size_t dot = filename.rfind ('.');
	return dot == string::npos ? filename : filename.substr (0, dot);
}

LexicalAnalyzer::LexicalAnalyzer (char * filename)
{
	input.open (filename);
	if (!input)
	{
		cerr << "Can't open " << filename << endl;
		exit (1);
	}
	string base = BaseName (filename);
	listing.open ((base + ".lst").c_str ());
	p1file.open ((base + ".p1").c_str ());
	debug.open ((base + ".dbg").c_str ());
	listing << "Input file: " << filename << endl;
	debug << "Input file: " << filename << endl;
	linenum = 0;
	pos = 0;
	errors = 0;
}

LexicalAnalyzer::~LexicalAnalyzer ()
{
	listing << errors << " errors found in input file\n";
	debug << errors << " errors found in input file\n";
	input.close ();
	listing.close ();
	p1file.close ();
	debug.close ();
}

token_type LexicalAnalyzer::GetToken ()
{
	// Skip whitespace and ; comments, reading (and listing) new lines as needed.
	while (true)
	{
		while (pos < (int) line.size () && isspace ((unsigned char) line[pos]))
			pos++;
		if (pos < (int) line.size () && line[pos] != ';')
			break;
		if (!getline (input, line))
		{
			lexeme = "";
			return EOF_T;
		}
		linenum++;
		pos = 0;
		listing << setw (4) << linenum << ": " << line << endl;
		debug << setw (4) << linenum << ": " << line << endl;
	}

	token_type token = ERROR_T;
	int start = pos;
	char c = line[pos];
	char next = pos + 1 < (int) line.size () ? line[pos + 1] : '\0';

	if (isdigit ((unsigned char) c) || ((c == '+' || c == '-') && isdigit ((unsigned char) next)))
	{ // numeric literal: optional sign, digits, optional fraction
		pos++;
		while (pos < (int) line.size () && isdigit ((unsigned char) line[pos]))
			pos++;
		if (pos + 1 < (int) line.size () && line[pos] == '.' && isdigit ((unsigned char) line[pos + 1]))
		{
			pos++;
			while (pos < (int) line.size () && isdigit ((unsigned char) line[pos]))
				pos++;
		}
		token = NUMLIT_T;
	}
	else if (isalpha ((unsigned char) c))
	{ // identifier, keyword or list operation
		while (pos < (int) line.size () && (isalnum ((unsigned char) line[pos]) || line[pos] == '_'))
			pos++;
		if (pos < (int) line.size () && line[pos] == '?')
			pos++;
		string word = line.substr (start, pos - start);
		token = IDENT_T;
		for (size_t k = 0; k < sizeof KEYWORDS / sizeof KEYWORDS[0]; k++)
			if (word == KEYWORDS[k].word)
				token = KEYWORDS[k].token;
		if (token == IDENT_T && IsListOp (word))
			token = LISTOP_T;
	}
	else
	{
		pos++;
		switch (c)
		{
			case '(': token = LPAREN_T; break;
			case ')': token = RPAREN_T; break;
			case '\'': token = QUOTE_T; break;
			case '+': token = PLUS_T; break;
			case '-': token = MINUS_T; break;
			case '*': token = MULT_T; break;
			case '/': token = DIV_T; break;
			case '=': token = EQUALTO_T; break;
			case '>':
			case '<':
				if (next == '=')
				{
					pos++;
					token = c == '>' ? GTE_T : LTE_T;
				}
				else
					token = c == '>' ? GT_T : LT_T;
				break;
			default: token = ERROR_T; break;
		}
	}

	lexeme = line.substr (start, pos - start);
	if (token == ERROR_T)
	{
		int at = pos;
		pos = start;  // the error is reported at the character's own column
		ReportError ("Invalid character found: " + lexeme);
		pos = at;
	}
	p1file << '\t' << left << setw (16) << token_names[token] << lexeme << endl;
	debug << '\t' << left << setw (16) << token_names[token] << lexeme << endl;
	debug << right;
	return token;
}

string LexicalAnalyzer::GetTokenName (token_type t) const
{
	if (t < 0 || t >= MAX_TOKENS)
		return "NONE";
	return token_names[t];
}

string LexicalAnalyzer::GetLexeme () const
{
	return lexeme;
}

void LexicalAnalyzer::ReportError (const string & msg)
{
	errors++;
	listing << "Error at " << linenum << ',' << pos + 1 << ": " << msg << endl;
	debug << "Error at " << linenum << ',' << pos + 1 << ": " << msg << endl;
}
